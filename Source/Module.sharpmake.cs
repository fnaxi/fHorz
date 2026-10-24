// CopyRight FlareHorz Team. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using Sharpmake;
using System.IO;
using System.Linq;

[Fragment, Flags]
public enum TargetType
{
	Game				= 1,
	Toolkit				= 2
}

[Fragment, Flags]
public enum Conf
{
	Debug				= 1,
	DebugGame			= 2,
	Development			= 4,
	Shipping			= 8
}

public class FlareTarget : ITarget
{
	public Platform		Platform;
	public DevEnv		DevEnv;
	public Conf			conf;
	public TargetType	type;
}

public enum ModuleType
{
	Plain = 0,
	Core = 1,
	Launch = 2
}

public enum ModuleGroup
{
	Runtime,
	ThirdParty
}

public static class Utils
{
	public static List<ITarget> GetTargets() => [
		new FlareTarget
		{
			Platform	= Platform.win64,
			DevEnv		= DevEnv.vs2022,
			conf		= Conf.Debug | Conf.DebugGame | Conf.Development,
			type		= TargetType.Game | TargetType.Toolkit
		},
		new FlareTarget
		{
			Platform	= Platform.win64,
			DevEnv		= DevEnv.vs2022,
			conf		= Conf.Shipping,
			type		= TargetType.Game
		}
	];
}

public abstract class ModuleBase : Project
{
	protected ModuleType mod_type;
	
	public ModuleBase(string in_name, ModuleGroup group, ModuleType in_mod_type) : base(typeof(FlareTarget))
	{
		Name = in_name;
		mod_type = in_mod_type;
		
		SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Source", group.ToString(), Name);
		
		IsFileNameToLower = false;
		IsTargetFileNameToLower = false;

		AddTargets( Utils.GetTargets().ToArray() );
	}
	
	public virtual void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		switch (mod_type)
		{
			case ModuleType.Plain: //@TODO: dlls
				conf.Output = Configuration.OutputType.Lib;
				#if DEAD_CODE
				conf.Defines.Add($"{Name.ToUpper()}_API=__declspec(dllexport)");
				conf.ExportDefines.Add($"{Name.ToUpper()}_API=__declspec(dllimport)");
				#else
				conf.Defines.Add($"{Name.ToUpper()}_API=");
				conf.ExportDefines.Add($"{Name.ToUpper()}_API=");
				#endif
				break;
			case ModuleType.Core:
				conf.Output = Configuration.OutputType.Lib;
				conf.Defines.Add($"{Name.ToUpper()}_API=");
				conf.ExportDefines.Add($"{Name.ToUpper()}_API=");
				break;
			case ModuleType.Launch:
				conf.Output = Configuration.OutputType.Exe;
				break;
		}
		
		// .vcxproj files
		conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Intermediate");

		conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Binaries");
		conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Intermediate", "[target.Name]", "[project.Name]");

		conf.TargetFileName = "fHorz[project.Name]-[target.conf]-[target.type]-[target.Platform]";
		conf.IncludePaths.Add(Path.Combine(SourceRootPath, "Exposed"));
		conf.IncludePrivatePaths.Add(Path.Combine(SourceRootPath, "Hidden"));

		conf.Options.Add(Options.Vc.General.WarningLevel.Level3);
		conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
		// conf.Options.Add(Options.Vc.Compiler.Exceptions.Disable);
		// conf.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.Latest);
		 
		string SourceDirectory = Path.Combine(Directory.GetCurrentDirectory(), "Source");
		if (mod_type == ModuleType.Launch)
		{
			List<string> Categories = Directory.GetDirectories(SourceDirectory, "*", SearchOption.TopDirectoryOnly)
				.Where(x => !x.EndsWith("ModuleTemplate")).ToList();
			foreach (string Category in Categories)
			{
				string[] Modules = Directory.GetDirectories(Category, "*", SearchOption.TopDirectoryOnly);
				foreach (string ModulePath in Modules)
				{
					string ModuleName = ModulePath.Split('\\').Last();
					if (ModuleName == "Launch") continue;
					
					conf.AdditionalLinkerOptions.Add($"/INCLUDE:Z_Implement{ModuleName}Module");
				}
			}
		}

		conf.Defines.Add($"FH_MODULE_NAME={Name}");
		
		conf.Defines.Add(BoolDefine("WITH_TOOLKIT",					target.type == TargetType.Toolkit));

		conf.Defines.Add(BoolDefine("CONFIGURATION_DEBUG",			target.conf == Conf.Debug));
		conf.Defines.Add(BoolDefine("CONFIGURATION_DEBUG_GAME",		target.conf == Conf.DebugGame));
		conf.Defines.Add(BoolDefine("CONFIGURATION_DEVELOPMENT",	target.conf == Conf.Development));
		conf.Defines.Add(BoolDefine("CONFIGURATION_SHIPPING",		target.conf == Conf.Shipping));
		
		switch (target.conf)
		{
			case Conf.Debug: case Conf.DebugGame:
				conf.Options.Add(Options.Vc.Compiler.Optimization.Disable);
				conf.Options.Add(Options.Vc.General.DebugInformation.ProgramDatabase);
				conf.Options.Add(Options.Vc.Linker.GenerateDebugInformation.Enable);
				conf.Options.Add(Options.Vc.Linker.GenerateFullProgramDatabaseFile.Enable);
				conf.Options.Add(Options.Vc.Linker.AssemblyDebug.RuntimeTrackingAndDisableOptimizations);
				break;
			case Conf.Development:
				conf.Options.Add(Options.Vc.Compiler.Optimization.MaximizeSpeed);
				conf.Options.Add(Options.Vc.General.DebugInformation.ProgramDatabase);
				conf.Options.Add(Options.Vc.Linker.GenerateDebugInformation.Enable);
				conf.Options.Add(Options.Vc.Linker.AssemblyDebug.NoRuntimeTrackingAndEnableOptimizations);
				break;
			case Conf.Shipping:
				conf.Options.Add(Options.Vc.Compiler.Optimization.FullOptimization);
				conf.Options.Add(Options.Vc.General.DebugInformation.Disable);
				conf.Options.Add(Options.Vc.Linker.GenerateDebugInformation.Disable);
				conf.Options.Add(Options.Vc.Linker.AssemblyDebug.NoDebuggableAttributeEmitted);
				break;
		}
		switch (target.Platform)
		{
			case Platform.win32:
				conf.Defines.Add(BoolDefine("PLATFORM_WINDOWS", true));
				conf.Defines.Add(BoolDefine("PLATFORM_X64", false));
				conf.Defines.Add("PLATFORM_NAME=Windows");
				break;
			case Platform.win64:
				conf.Defines.Add(BoolDefine("PLATFORM_WINDOWS", true));
				conf.Defines.Add(BoolDefine("PLATFORM_X64", true));
				conf.Defines.Add("PLATFORM_NAME=Windows");
				break;
			default:
				Debug.Assert(false);
				break;
		}
	}

	protected string BoolDefine(string Define, bool bValue) => $"{Define}=" + (bValue ? "1" : "0");
}

public abstract class RuntimeModule(string InName, ModuleType InModType = ModuleType.Plain)
	: ModuleBase(InName, ModuleGroup.Runtime, InModType)
{
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);

		conf.SolutionFolder = "Runtime";
	}
}

public abstract class ThirdPartyModule(string InName, ModuleType InModType = ModuleType.Plain)
	: ModuleBase(InName, ModuleGroup.ThirdParty, InModType)
{
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);

		conf.SolutionFolder = "ThirdParty";
	}
}
