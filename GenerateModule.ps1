# CopyRight FlareHorz Team. All Rights Reserved.

param(
	[Parameter(Position=0)]
	[string]$ModuleName,

	[Parameter(Position=1)]
	[string]$Category
)

# remove '-' symbol from the arguments
if ($ModuleName -and $ModuleName.StartsWith("-")) { $ModuleName = $ModuleName.Substring(1) }
if ($Category -and $Category.StartsWith("-")) { $Category = $Category.Substring(1) }

if ([string]::IsNullOrWhiteSpace($ModuleName)) {
	$ModuleName = Read-Host "Enter module name"
}

if ([string]::IsNullOrWhiteSpace($Category)) {
	$Category = Read-Host "Enter category"
}

if ([string]::IsNullOrWhiteSpace($ModuleName) -or [string]::IsNullOrWhiteSpace($Category)) {
	Write-Error "ModuleName and Category arguments are required!"
	exit 1
}

$ScriptRoot = $PSScriptRoot

Write-Host "Generating module '$ModuleName' under category '$Category'..." -ForegroundColor Cyan

# ----------------------------------------------------------------------------
#	Create sharpmake file
# ----------------------------------------------------------------------------

$SharpmakeDir = Join-Path $ScriptRoot "Source\Sharpmake\$Category"
if (-not (Test-Path $SharpmakeDir)) {
	New-Item -ItemType Directory -Force -Path $SharpmakeDir | Out-Null
}

$SharpmakeFilePath = Join-Path $SharpmakeDir "$ModuleName.sharpmake.cs"
$SharpmakeContent = @"
// CopyRight FlareHorz Team. All Rights Reserved.

using Sharpmake;

[module: Include("../Module.sharpmake.cs")]

[Generate]
public class ${ModuleName}Module() : ${Category}Module("${ModuleName}")
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
			// ...
		}
	}
}
"@
Set-Content -Path $SharpmakeFilePath -Value $SharpmakeContent -Encoding UTF8
Write-Host "Created Source/Sharpmake/$Category/$ModuleName.sharpmake.cs" -ForegroundColor Green

# ----------------------------------------------------------------------------
#	Modify Main.sharpmake.cs
# ----------------------------------------------------------------------------

$MainSharpmakePath = Join-Path $ScriptRoot "Source\Sharpmake\Main.sharpmake.cs"
if (Test-Path $MainSharpmakePath) {
	$MainContent = Get-Content -Path $MainSharpmakePath -Raw

	# Add 'include module' directive after last 'using' directive
	$IncludeLine = "[module: Include(""$Category/$ModuleName.sharpmake.cs"")]`n"
	if ($MainContent -notmatch [regex]::Escape($IncludeLine)) {
		# Find last 'using ...;'
		if ($MainContent -match '(?ms)(using\s+[^;]+;\s*)+') {
			$lastUsing = $Matches[0]
			$MainContent = $MainContent -replace [regex]::Escape($lastUsing), "$lastUsing`n$IncludeLine"
		}
	}
	
	# Modify RegisterProjects() method
	$CategoryComment = "// $Category"
	$ProjectRegistration = "conf.AddProject<${ModuleName}Module>(target);"

	if ($MainContent -match [regex]::Escape($CategoryComment)) {
		# Category block already exists
		
		$pattern = "(?s)($([regex]::Escape($CategoryComment))\s*\{\s*[^}]*?)(\s*\})"
		if ($MainContent -match $pattern) {
			$MainContent = [System.Text.RegularExpressions.Regex]::Replace($MainContent, $pattern, "`$1`n			$ProjectRegistration`$2")
		}
	} else {
		$registerProjectsPattern = "(?s)(public void RegisterProjects\(Solution\.Configuration conf, FlareTarget target\)\s*\{[^}]*?)(\s*\})"
		if ($MainContent -match $registerProjectsPattern) {
			$NewCategoryBlock = "`n		// $Category`n		{`n			$ProjectRegistration`n		}`n"
			$MainContent = [System.Text.RegularExpressions.Regex]::Replace($MainContent, $registerProjectsPattern, "`$1$NewCategoryBlock`$2")
		}
	}

	Set-Content -Path $MainSharpmakePath -Value $MainContent -Encoding UTF8
	Write-Host "Modified Source/Sharpmake/Main.sharpmake.cs" -ForegroundColor Green
} else {
	Write-Warning "Main.sharpmake.cs not found at '$MainSharpmakePath'! Skipping modification..."
}

# ----------------------------------------------------------------------------
#	Create C++ files
# ----------------------------------------------------------------------------

$CppBaseDir = Join-Path $ScriptRoot "Source\$Category\$ModuleName"
$ExposedDir = Join-Path $CppBaseDir "Exposed"
$HiddenDir = Join-Path $CppBaseDir "Hidden"

New-Item -ItemType Directory -Force -Path $ExposedDir | Out-Null
New-Item -ItemType Directory -Force -Path $HiddenDir | Out-Null

$HeaderPath = Join-Path $ExposedDir "$ModuleName.h"
$HeaderContent = @"
// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "ModuleSystem/ModuleInterface.h"

class C${ModuleName}Module : public IModuleInterface
{
public:
	virtual void MountModule() override;
	virtual void DetachModule() override;
};
"@
Set-Content -Path $HeaderPath -Value $HeaderContent -Encoding UTF8
Write-Host "Created: Source/$Category/$ModuleName/Exposed/$ModuleName.h" -ForegroundColor Green


$SourcePath = Join-Path $HiddenDir "$ModuleName.cpp"
$SourceContent = @"
// CopyRight FlareHorz Team. All Rights Reserved.


#include "${ModuleName}.h"

#include "ModuleSystem/ModuleManager.h"

void C${ModuleName}Module::MountModule()
{
	// ...
}

void C${ModuleName}Module::DetachModule()
{
	// ...
}

IMPLEMENT_MODULE(C${ModuleName}Module, ${ModuleName})

"@
Set-Content -Path $SourcePath -Value $SourceContent -Encoding UTF8
Write-Host "Created Source/$Category/$ModuleName/Hidden/$ModuleName.cpp" -ForegroundColor Green

Write-Host "Module '$ModuleName' generated successfully!" -ForegroundColor Cyan
