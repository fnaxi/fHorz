// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Platform/Platform.h"

/** 
 * Enum that defines the verbosity levels of the logging system.
 */
enum class ELogVerbosity : uint8
{
	/** Disables logging for a specific log category. */
	NoLogging		= 0,
	
	/** Prints an error to log file and crashes the application (even if logging is disabled). */
	Fatal			= 1,
	
	/** Prints an error to log file. */
	Error			= 2,

	/** Prints a warning to log file. */
	Warn			= 3,

	/** Prints a message to a log file. */
	Info			= 4,

	/** Prints a verbose message to a log file (usually used for detailed logging). */
	Debug			= 5,

	/** Prints a verbose message to a log file (usually used for detailed logging that would otherwise spam output). */
	Trace			= 6,

	/** Enables all logging verbosity for a category. */
	All				= Trace
};

/** Converts verbosity to a string. */
const TCHAR* LogVerbosityToString(ELogVerbosity Verbosity);
