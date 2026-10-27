// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <cstdarg>

#include "LogVerbosity.h"
#include "Platform/Platform.h"

#define ENABLE_MISC_LOGS 1

struct SLogCategoryBase;

namespace Flare::Core::Private
{
	void CategorizedLogV(ELogVerbosity LogVerbosity, const SLogCategoryBase& Category, const TCHAR* Format, va_list Args);
	void CategorizedLog(ELogVerbosity LogVerbosity, const SLogCategoryBase& Category, const TCHAR* Format, ...);
	
	void BasicLogV(ELogVerbosity LogVerbosity, const TCHAR* Category, const TCHAR* Format, va_list Args);
	void BasicLog(ELogVerbosity LogVerbosity, const TCHAR* Category, const TCHAR* Format, ...);

#if ENABLE_MISC_LOGS
	constexpr ELogVerbosity GMiscLogsSeverity = ELogVerbosity::Debug;
#endif

	constexpr int32 GCategoryPadding = 15;
}

#define FH_LOG(Verbosity, Category, Format, ...) \
	{ \
		if( !Category.IsSuppressed(ELogVerbosity::Verbosity) ) \
		{ \
			Flare::Core::Private::CategorizedLog(ELogVerbosity::Verbosity, Category, Format, ##__VA_ARGS__); \
		} \
	}

#if ENABLE_MISC_LOGS
	#define FH_MISC_LOG(Verbosity, Format, ...) \
	{ \
		if constexpr (ELogVerbosity::Verbosity <= Flare::Core::Private::GMiscLogsSeverity) \
		{ \
			Flare::Core::Private::BasicLog(ELogVerbosity::Verbosity, LOG_CATEGORY, Format, ##__VA_ARGS__); \
		} \
	}
#else
	#define FH_MISC_LOG(Verbosity, Format, ...)
#endif
