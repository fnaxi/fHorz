// CopyRight FlareHorz Team. All Rights Reserved.

using System.IO;
using Sharpmake;

[module: Include("Runtime/Core.sharpmake.cs")]
[module: Include("Runtime/Launch.sharpmake.cs")]

[module: Include("Runtime/Engine.sharpmake.cs")]

[module: Include("Rules.sharpmake.cs")]

[Generate]
public class FlareHorzSolution : Solution
{
	public FlareHorzSolution() : base(typeof(FlareTarget))
	{
		Name = "fHorz";
		IsFileNameToLower = false;
		
		AddTargets( Utils.GetTargets().ToArray() );
		
		ExtraItems.Add("Solution", new Strings
		(
			"README.md", ".gitignore"
		));
		ExtraItems.Add("Solution/Images", new Strings
		(
			"Images/Logo.png", "Images/Logo.ico"
		));
	}

	[Configure]
	public void ConfigureAll(Solution.Configuration conf, FlareTarget target)
	{
		conf.SolutionPath = Path.Combine("[solution.SharpmakeCsPath]", "..", "..");
		conf.Name = "[target.Conf] / [target.Build]";
		conf.PlatformName = "Win64";

		// Runtime
		{
			conf.AddProject<CoreModule>(target);
			conf.AddProject<LaunchModule>(target);

			conf.AddProject<EngineModule>(target);
		}
		
		// Third-Party
		{
			// conf.AddProject<SDL3Module>(target);
		}
	}
}

public static class Main
{
	[Sharpmake.Main]
	public static void SharpmakeMain(Arguments arguments)
	{
		KitsRootPaths.SetUseKitsRootForDevEnv(DevEnv.vs2022, KitsRootEnum.KitsRoot10, Options.Vc.General.WindowsTargetPlatformVersion.Latest);
		
		arguments.Generate<FlareHorzSolution>();
		arguments.Generate<RulesSolution>();
	}
}
