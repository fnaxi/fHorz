// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Generate]
public class EngineModule() : RuntimeModule("Engine")
{
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);
		
		// Exposed dependencies
		{
			conf.AddPublicDependency<CoreModule>(target);
		}
		
		// Hidden dependencies
		{
			// ...
		}
	}
}
