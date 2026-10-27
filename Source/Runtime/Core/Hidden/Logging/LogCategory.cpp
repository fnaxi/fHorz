// CopyRight FlareHorz Team. All Rights Reserved.


#include "Logging/LogCategory.h"

SLogCategoryBase::SLogCategoryBase(const TCHAR* InName, const ELogVerbosity InDefaultVerbosity)
{
	Name = InName;
	Verbosity = InDefaultVerbosity;
	DefaultVerbosity = InDefaultVerbosity;
}

