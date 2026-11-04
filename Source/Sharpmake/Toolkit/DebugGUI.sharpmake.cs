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
			conf.AddExposedDependency<CoreModule>(target);
		}
		
		// Private dependencies
		{
			conf.AddHiddenDependency<EngineModule>(target);
			conf.AddHiddenDependency<RendererModule>(target);

			conf.AddHiddenDependency<ImGuiModule>(target);
			conf.AddHiddenDependency<SDL3Module>(target);
		}
	}
}
