// CopyRight FlareHorz Team. All Rights Reserved.

using System.Diagnostics;
using System.IO;
using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Export]
public class freetypeModule() : ThirdPartyModule("freetype")
{
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);
		
		conf.IncludePaths.Add(Path.Combine(SourceRootPath, "include"));
		
		string LibPath = "";
		if (target.Platform == Platform.win64)
		{
			LibPath = Path.Combine(SourceRootPath, "release_static/vs2026", "x64");
		}
		else if (target.Platform == Platform.win32)
		{
			LibPath = Path.Combine(SourceRootPath, "release_static/vs2026", "x86");
		}
		else
		{
			Trace.Assert(false);
		}

		conf.LibraryPaths.Add(LibPath);
		conf.LibraryFiles.Add("freetype");

		// conf.TargetCopyFiles.Add(Path.Combine(LibPath, "freetype.dll"));
	}
}
