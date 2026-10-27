// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Platform/Platform.h"
#include "Platform/PlatformMisc.h"
#include "Containers/TextHelpers.h"

namespace Flare::Core::Private
{
	[[noreturn]] void Abort();
	void AssertionLog(bool bFatal, const TCHAR* Format, ...);
}

#if CONFIGURATION_DEBUG || CONFIGURATION_DEBUG_GAME
	#define SLOW_ENSURE(Expression) ENSURE(Expression)
	#define SLOW_ENSURE_M(Expression, Message, ...) ENSURE_M(Expression, Message, ##__VA_ARGS__)
#else
	#define SLOW_ENSURE(Expression)
	#define SLOW_ENSURE_M(Expression, Message, ...)
#endif

#define ENSURE_NO_ENTRY() ENSURE(!"ENSURE_NO_ENTRY!");

#if !CONFIGURATION_SHIPPING
	/** Triggers a breakpoint once, then continues execution. */
	#define ENSURE(Expression) \
		([](bool bResult) -> bool { \
			if (!bResult) \
			{ \
				static bool bExecuted = false; \
				if (!bExecuted) \
				{ \
					bExecuted = true; \
					Flare::Core::Private::AssertionLog(false, "ENSURE failed: \"%s\" at %s:%d", #Expression, __FILE__, __LINE__); \
					if (SPlatformMisc::IsDebuggerPresent()) \
					{ \
						PLATFORM_BREAK(); \
					} \
				} \
			} \
			return bResult; \
		} (!!(Expression)))
	#define ENSURE_M(Expression, Message, ...) \
		([](bool bResult) -> bool { \
			if (!bResult) \
			{ \
				static bool bExecuted = false; \
				if (!bExecuted) \
				{ \
					bExecuted = true; \
					\
					TCHAR FormattedMessage[1024]; \
					STextHelpers::Format(FormattedMessage, 1024, Message, ##__VA_ARGS__); \
					\
					Flare::Core::Private::AssertionLog(false, "ENSURE failed: \"%s\" at %s:%d / Message: %s", #Expression, __FILE__, __LINE__, FormattedMessage, ##__VA_ARGS__); \
					if (SPlatformMisc::IsDebuggerPresent()) \
					{ \
						PLATFORM_BREAK(); \
					} \
				} \
			} \
			return bResult; \
		} (!!(Expression)))

	/** Triggers a breakpoint and crashes the program. */
	#define STRICT_ENSURE(Expression) \
		do { \
			if (!(Expression)) \
			{ \
				if (SPlatformMisc::IsDebuggerPresent()) \
				{ \
					PLATFORM_BREAK(); \
				} \
				Flare::Core::Private::AssertionLog(true, "STRICT_ENSURE failed: \"%s\" at %s:%d", #Expression, __FILE__, __LINE__); \
			} \
		} while(0)
	#define STRICT_ENSURE_M(Expression, Message, ...) \
		do { \
			if (!(Expression)) \
			{ \
				if (SPlatformMisc::IsDebuggerPresent()) \
				{ \
					PLATFORM_BREAK(); \
				} \
				\
				TCHAR FormattedMessage[1024]; \
				STextHelpers::Format(FormattedMessage, 1024, Message, ##__VA_ARGS__); \
				\
				Flare::Core::Private::AssertionLog(true, "STRICT_ENSURE failed: \"%s\" at %s:%d / Message: %s", #Expression, __FILE__, __LINE__, FormattedMessage); \
			} \
		} while(0)
#else
	#define ENSURE(Expression) (!!(Expression))
	#define ENSURE_M(Expression, Message, ...) (!!(Expression))
	#define STRICT_ENSURE(Expression)
	#define STRICT_ENSURE_M(Expression, Message, ...)
#endif
