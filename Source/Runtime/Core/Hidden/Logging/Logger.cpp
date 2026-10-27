// CopyRight FlareHorz Team. All Rights Reserved.


#include "Logging/Logger.h"

#include <cstdarg>
#include <cstdio>

#include "Logging/LogCategory.h"
#include "Containers/String.h"

void Flare::Core::Private::CategorizedLogV(const ELogVerbosity LogVerbosity, const SLogCategoryBase& Category, const TCHAR* Format, const va_list Args)
{
	BasicLogV(LogVerbosity, *Category.GetName(), Format, Args);
}

void Flare::Core::Private::CategorizedLog(const ELogVerbosity LogVerbosity, const SLogCategoryBase& Category, const TCHAR* Format, ...)
{
	va_list Args;
	va_start(Args, Format);
	CategorizedLogV(LogVerbosity, Category, Format, Args);
	va_end(Args);
}

void Flare::Core::Private::BasicLogV(const ELogVerbosity LogVerbosity, const TCHAR* Category, const TCHAR* Format, const va_list Args)
{
	if (Category && Category[0] == 'l')
	{
		printf("%s, %-*s> ", LogVerbosityToString(LogVerbosity), GCategoryPadding, Category);
	}
	else
	{
		printf("%s,  %-*s> ", LogVerbosityToString(LogVerbosity), GCategoryPadding - 1, Category);
	}
	
	vprintf(Format, Args);
	printf("\n");

	if (LogVerbosity == ELogVerbosity::Fatal)
	{
		Abort();
	}
}

void Flare::Core::Private::BasicLog(const ELogVerbosity LogVerbosity, const TCHAR* Category, const TCHAR* Format, ...)
{
	va_list Args;
	va_start(Args, Format);
	BasicLogV(LogVerbosity, Category, Format, Args);
	va_end(Args);
}

