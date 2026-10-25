// CopyRight FlareHorz Team. All Rights Reserved.

using System;
using System.Collections.Generic;
using Sharpmake;

[Fragment, Flags]
public enum EBuild
{
	Game			= 1,
	Toolkit			= 2
}

[Fragment, Flags]
public enum EConf
{
	Debug			= 1,
	DebugGame		= 2,
	Development		= 4,
	Shipping		= 8
}

public class FlareTarget : ITarget
{
	public Platform		Platform;
	public DevEnv		DevEnv;
	public EConf		Conf;
	public EBuild		Build;
}

public enum EModuleType
{
	Plain	= 0,
	Core	= 1,
	Launch	= 2
}

public enum EModuleGroup
{
	Runtime,
	ThirdParty
}

public static class Utils
{
	public static List<ITarget> GetTargets() => [
		new FlareTarget
		{
			Platform	= Platform.win64,
			DevEnv		= DevEnv.vs2022,
			Conf		= EConf.Debug | EConf.DebugGame | EConf.Development,
			Build		= EBuild.Game | EBuild.Toolkit
		},
		new FlareTarget
		{
			Platform	= Platform.win64,
			DevEnv		= DevEnv.vs2022,
			Conf		= EConf.Shipping,
			Build		= EBuild.Game
		}
	];
}
