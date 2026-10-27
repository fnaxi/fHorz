// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "LogVerbosity.h"
#include "Containers/Name.h"

/** 
 * A macro to declare a logging category as a C++ "extern", declared in the header and paired
 * with DEFINE_LOG_CATEGORY in the source. Accessible by all files that include the header.
 */
#define DECLARE_LOG_CATEGORY(CategoryName, DefaultVerbosity) \
	extern struct SLogCategory_##CategoryName : public SLogCategoryBase \
	{ \
		SLogCategory_##CategoryName() : SLogCategoryBase(#CategoryName, ELogVerbosity::DefaultVerbosity) {} \
	} CategoryName;

/** A macro to define a logging category, paired with DECLARE_LOG_CATEGORY from the header. */
#define DEFINE_LOG_CATEGORY(CategoryName) SLogCategory_##CategoryName CategoryName;

/** A macro to define a logging category as a C++ "static". This should ONLY be declared in a source file. Only accessible in that single file. */
#define DEFINE_PRIVATE_LOG_CATEGORY(CategoryName, DefaultVerbosity) \
	static struct SLogCategory_##CategoryName : public SLogCategoryBase \
	{ \
		SLogCategory_##CategoryName() : SLogCategoryBase(#CategoryName, ELogVerbosity::DefaultVerbosity) {} \
	} CategoryName;

/**
 * Base class for all log categories.
 */
struct SLogCategoryBase
{
	SLogCategoryBase(const TCHAR* InName, ELogVerbosity InDefaultVerbosity);

	[[nodiscard]] const SName& GetName() const arrow (Name)
	
	[[nodiscard]] ELogVerbosity GetVerbosity() const arrow (Verbosity)
	void SetVerbosity(const ELogVerbosity NewVerbosity) { Verbosity = NewVerbosity; }

	FORCEINLINE bool IsSuppressed(const ELogVerbosity VerbosityToTest) const arrow( VerbosityToTest >= Verbosity )
	
private:
	SName Name;
	
	ELogVerbosity Verbosity;
	ELogVerbosity DefaultVerbosity;
};
