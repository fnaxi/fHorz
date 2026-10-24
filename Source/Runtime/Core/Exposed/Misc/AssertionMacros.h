// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <cstdlib>

#include "Platform/Platform.h"

#if CONFIGURATION_DEBUG || CONFIGURATION_DEBUG_GAME
	#define SLOW_ENSURE(Expression) ENSURE(Expression)
	#define SLOW_ENSURE_M(Expression) SLOW_ENSURE(Expression)
#else
	#define SLOW_ENSURE(Expression)
	#define SLOW_ENSURE_M(Expression)
#endif

#if !CONFIGURATION_SHIPPING
	/** Evaluates expression. On failure, triggers a breakpoint once, then continues execution. */
	#define ENSURE(Expression) \
		([](bool bResult) -> bool { \
			if (!bResult) \
			{ \
				static bool bExecuted = false; \
				if (!bExecuted) \
				{ \
					bExecuted = true; \
					DEBUG_BREAK(); \
				} \
			} \
			return bResult; \
		} (!!(Expression)));
	#define ENSURE_M(Expression, Message, ...) ENSURE(Expression)

	#define ENSURE_NO_ENTRY() ENSURE(!"ENSURE_NO_ENTRY!")

	/** Evaluates expression. On failure, triggers a breakpoint and immediately crashes the program. */
	#define STRICT_ENSURE(Expression) \
		do { \
			if (!(Expression)) \
			{ \
				DEBUG_BREAK(); \
				std::abort(); \
			} \
		} while(0);
	#define STRICT_ENSURE_M(Expression, Message, ...) ENSURE(Expression)
#else
	#define STRICT_ENSURE(Expression)
	#define STRICT_ENSURE_M(Expression, Message, ...)
#endif

//@TODO: assertion macros
