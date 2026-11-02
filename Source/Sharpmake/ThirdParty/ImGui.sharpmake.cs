// CopyRight FlareHorz Team. All Rights Reserved.

using System.Diagnostics;
using System.IO;
using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Generate]
public class ImGuiModule() : ThirdPartyModule("ImGui")
{
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);
		
		conf.IncludePaths.Add( SourceRootPath );
	}
}
