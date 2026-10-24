// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

// [Generate]
public class ModuleTemplate : RuntimeModule
{
	public ModuleTemplate() : base("Core") { }
	
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);
	}
}
