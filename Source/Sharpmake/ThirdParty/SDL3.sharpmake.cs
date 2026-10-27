// CopyRight FlareHorz Team. All Rights Reserved.

using System.Diagnostics;
using System.IO;
using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Export]
public class SDL3Module() : ThirdPartyModule("SDL3", "include")
{
	[Configure]
	public override void ConfigureAll(Project.Configuration conf, FlareTarget target)
	{
		base.ConfigureAll(conf, target);

		string LibPath = "";
		if (target.Platform == Platform.win64)
		{
			LibPath = Path.Combine(SourceRootPath, "lib", "x64");
		}
		else if (target.Platform == Platform.win32)
		{
			LibPath = Path.Combine(SourceRootPath, "lib", "x86");
		}
		else
		{
			Trace.Assert(false);
		}

		conf.LibraryPaths.Add(LibPath);
		conf.LibraryFiles.Add("SDL3");

		conf.TargetCopyFiles.Add(Path.Combine(LibPath, "SDL3.dll"));
	}
}
