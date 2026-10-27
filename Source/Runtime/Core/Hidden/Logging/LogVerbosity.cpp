// CopyRight FlareHorz Team. All Rights Reserved.


#include "Logging/LogVerbosity.h"

const TCHAR* LogVerbosityToString(const ELogVerbosity Verbosity)
{
	switch (Verbosity)
	{
		case ELogVerbosity::Fatal:			return "Fatal ";
		case ELogVerbosity::Error:			return "Error ";
		case ELogVerbosity::Warn:			return "Warn  ";
		case ELogVerbosity::Info:			return "Info  ";
		case ELogVerbosity::Debug:			return "Debug ";
		case ELogVerbosity::Trace:			return "Trace ";
		default:							return "Unknown";
	}
}

