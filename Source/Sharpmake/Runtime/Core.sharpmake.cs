// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Generate]
public class CoreModule() : RuntimeModule("Core", EModuleType.Core)
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
			// ...
		}
	}
}
