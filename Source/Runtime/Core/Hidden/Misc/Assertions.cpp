// CopyRight FlareHorz Team. All Rights Reserved.


#include "Misc/Assertions.h"

#include "Logging/LogCategory.h"
#include "Logging/Logger.h"

DEFINE_PRIVATE_LOG_CATEGORY(lAssertion, Error)

void Flare::Core::Private::Abort()
{
	std::abort();
}

void Flare::Core::Private::AssertionLog(bool bFatal, const TCHAR* Format, ...)
{
	va_list Args;
	va_start(Args, Format);
	CategorizedLogV(bFatal ? ELogVerbosity::Fatal : ELogVerbosity::Error, lAssertion, Format, Args);
	va_end(Args);
}

