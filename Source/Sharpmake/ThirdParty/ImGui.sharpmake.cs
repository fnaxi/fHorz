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
		
		conf.AddPrivateDependency<SDL3Module>(target);
		conf.AddPrivateDependency<freetypeModule>(target);
		
		conf.IncludePaths.Add( SourceRootPath );
        
        conf.TargetFileName = "[project.Name]-[target.Platform]";
	}
}
