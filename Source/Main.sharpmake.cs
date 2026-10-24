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
	}

	[Configure]
	public void ConfigureAll(Solution.Configuration conf, FlareTarget target)
	{
		conf.SolutionPath = Path.Combine("[solution.SharpmakeCsPath]", "..");
		conf.Name = "[target.conf] / [target.type]";
		conf.PlatformName = "Win64";

		conf.AddProject<CoreModule>(target);
		conf.AddProject<LaunchModule>(target);

		conf.AddProject<EngineModule>(target);
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
