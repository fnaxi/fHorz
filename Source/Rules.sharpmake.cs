// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;
using System.IO;

[Generate]
public class RulesSolution : CSharpSolution
{
	public RulesSolution()
	{
		Name = "fRules";
		IsFileNameToLower = false;
		
		AddTargets(new Target
		{
			Platform		= Platform.win64,
			DevEnv			= DevEnv.vs2022,
			Optimization	= Optimization.Debug,
			Framework		= DotNetFramework.net9_0
		});

		ExtraItems.Add("Solution", new Strings
		(
			"GenerateProjectFiles.bat"
		));
	}

	[Configure]
	public void ConfigureAll(Solution.Configuration conf, Target target)
	{
		conf.SolutionPath = Path.Combine("[solution.SharpmakeCsPath]", "..");
	
		conf.AddProject<RulesProject>(target);
	}
}

[Generate]
public class RulesProject : CSharpProject
{
	public RulesProject()
	{
		Name = "FlareRules";
		IsFileNameToLower = false;
		
		SourceRootPath = Path.Combine("[project.SharpmakeCsPath]");

		AddTargets(new Target
		{
			Platform		= Platform.win64,
			DevEnv			= DevEnv.vs2022,
			Optimization	= Optimization.Debug | Optimization.Release,
			Framework		= DotNetFramework.net9_0
		});
	}

	[Configure]
	public void ConfigureAll(Configuration conf, Target target)
	{
		conf.Output = Configuration.OutputType.DotNetClassLibrary;
		
		conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]", "..", "Intermediate");

		conf.TargetPath = Path.Combine("[project.SharpmakeCsPath]", "..", "Binaries");
		conf.IntermediatePath = Path.Combine("[project.SharpmakeCsPath]", "..", "Intermediate");

		conf.ReferencesByPath.Add(Path.Combine("[project.SharpmakeCsPath]", "..", "Tools", "Sharpmake", "Sharpmake.dll"));
		conf.ReferencesByPath.Add(Path.Combine("[project.SharpmakeCsPath]", "..", "Tools", "Sharpmake", "Sharpmake.Application.exe"));
	}
}
