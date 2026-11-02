// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Generate]
public class DebugGUIModule() : ToolkitModule("DebugGUI")
{
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);
		
		// Public dependencies
		{
			conf.AddPublicDependency<CoreModule>(target);
		}
		
		// Private dependencies
		{
			conf.AddPrivateDependency<SDL3Module>(target);
		}
	}
}
