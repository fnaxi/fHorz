// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Generate]
public class LaunchModule : RuntimeModule
{
	public LaunchModule() : base("Launch", ModuleType.Launch) { }
	
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);

		// Exposed dependencies
		{
			// ...
		}
		
		// Hidden dependencies
		{
			conf.AddPrivateDependency<EngineModule>(target);
		}
		
		conf.TargetFileName = "fHorz-[target.conf]-[target.type]-[target.Platform]";
	}
}
