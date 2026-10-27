// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Generate]
public class LaunchModule() : RuntimeModule("Launch", EModuleType.Launch)
{
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);

		// Public dependencies
		{
			// ...
		}
		
		// Private dependencies
		{
			conf.AddPrivateDependency<EngineModule>(target);
		}
		
		conf.TargetFileName = "fHorz-[target.Conf]-[target.Build]-[target.Platform]";
	}
}
