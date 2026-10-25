// CopyRight FlareHorz Team. All Rights Reserved.

using System.Collections.Generic;
using System.Diagnostics;
using Sharpmake;
using System.IO;
using System.Linq;

[module: Include("Misc.sharpmake.cs")]

public abstract class FlareProject : Project
{
	public FlareProject(string InName) : base(typeof(FlareTarget))
	{
		Name = InName;
		
		IsFileNameToLower = false;
		IsTargetFileNameToLower = false;

		AddTargets( Utils.GetTargets().ToArray() );
	}
	
	public virtual void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		// .vcxproj files
		conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Intermediate");

		// Binaries / Intermediate
		conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Binaries");
		conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Intermediate", "[target.Name]", "[project.Name]");
	}

	protected string BoolDefine(string Define, bool bValue) => $"{Define}=" + (bValue ? "1" : "0");
}

public abstract class ModuleBase : FlareProject
{
	protected EModuleType ModType;
	
	public ModuleBase(string InName, EModuleGroup group, EModuleType InModType = EModuleType.Plain)
		: base(InName)
	{
		ModType = InModType;
		
		SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Source", group.ToString(), Name);
	}

	public override void ConfigureAll(Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);
		
		conf.TargetFileName = "fHorz[project.Name]-[target.Conf]-[target.Build]-[target.Platform]";
		
		switch (ModType)
		{
			case EModuleType.Plain: //@TODO: dlls
				conf.Output = Configuration.OutputType.Lib;
#if TODO_DLLS
				conf.Defines.Add($"{Name.ToUpper()}_API=__declspec(dllexport)");
				conf.ExportDefines.Add($"{Name.ToUpper()}_API=__declspec(dllimport)");
#else
				conf.Defines.Add($"{Name.ToUpper()}_API=");
				conf.ExportDefines.Add($"{Name.ToUpper()}_API=");
#endif
				break;
			case EModuleType.Core:
				conf.Output = Configuration.OutputType.Lib;
				conf.Defines.Add($"{Name.ToUpper()}_API=");
				conf.ExportDefines.Add($"{Name.ToUpper()}_API=");
				break;
			case EModuleType.Launch:
				conf.Output = Configuration.OutputType.Exe;
				break;
		}
		
		conf.IncludePaths.Add(Path.Combine(SourceRootPath, "Exposed"));
		conf.IncludePrivatePaths.Add(Path.Combine(SourceRootPath, "Hidden"));
		 
		// Force modules registration
		string SourceDirectory = Path.Combine(Directory.GetCurrentDirectory(), "Source");
		if (ModType == EModuleType.Launch)
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
		
		conf.Defines.Add(BoolDefine("WITH_TOOLKIT",					target.Build == EBuild.Toolkit));

		conf.Defines.Add(BoolDefine("CONFIGURATION_DEBUG",			target.Conf == EConf.Debug));
		conf.Defines.Add(BoolDefine("CONFIGURATION_DEBUG_GAME",		target.Conf == EConf.DebugGame));
		conf.Defines.Add(BoolDefine("CONFIGURATION_DEVELOPMENT",	target.Conf == EConf.Development));
		conf.Defines.Add(BoolDefine("CONFIGURATION_SHIPPING",		target.Conf == EConf.Shipping));
		
		conf.Options.Add(Options.Vc.General.WarningLevel.Level3);
		conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
		// conf.Options.Add(Options.Vc.Compiler.Exceptions.Disable);
		// conf.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.Latest);
		
		switch (target.Conf)
		{
			case EConf.Debug: case EConf.DebugGame:
				conf.Options.Add(Options.Vc.Compiler.Optimization.Disable);
				conf.Options.Add(Options.Vc.General.DebugInformation.ProgramDatabase);
				conf.Options.Add(Options.Vc.Linker.GenerateDebugInformation.Enable);
				conf.Options.Add(Options.Vc.Linker.GenerateFullProgramDatabaseFile.Enable);
				conf.Options.Add(Options.Vc.Linker.AssemblyDebug.RuntimeTrackingAndDisableOptimizations);
				break;
			case EConf.Development:
				conf.Options.Add(Options.Vc.Compiler.Optimization.MaximizeSpeed);
				conf.Options.Add(Options.Vc.General.DebugInformation.ProgramDatabase);
				conf.Options.Add(Options.Vc.Linker.GenerateDebugInformation.Enable);
				conf.Options.Add(Options.Vc.Linker.AssemblyDebug.NoRuntimeTrackingAndEnableOptimizations);
				break;
			case EConf.Shipping:
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
}

public abstract class RuntimeModule(string InName, EModuleType InModType = EModuleType.Plain)
	: ModuleBase(InName, EModuleGroup.Runtime, InModType)
{
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);

		conf.SolutionFolder = "Runtime";
	}
}

public abstract class ThirdPartyModule : FlareProject
{
	public ThirdPartyModule(string InName) : base(InName)
	{
		SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "..", "..", "Source", "ThirdParty", Name);
	}
	
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);

		conf.SolutionFolder = "ThirdParty";
		
		conf.IncludePaths.Add(Path.Combine(SourceRootPath, "source"));
		conf.IncludePrivatePaths.Add(Path.Combine(SourceRootPath, "include"));
	}
}
