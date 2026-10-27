// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Array.h"
#include "FlareNames.h"

/**
 * Holds only text but wrapped in a struct to prevent TArray from manipulating on "const TCHAR*" directly.
 */
struct SNameEntry
{
	SNameEntry(const TCHAR* InName) : Name(InName) { }

	const TCHAR* Name;
};

class CNameTable
{
	CNameTable();
	
public:
	static CNameTable& Get();

	/** Finds or registers a string and returns its unique index. */
	int32 FindOrAdd(const TCHAR* Text);

	[[nodiscard]] const TCHAR* GetName(int32 Index) const;

private:
	TArray<SNameEntry> Entries;
};

struct SName
{
	SName() : DisplayIndex(0) { }
	SName(EName Name);
	SName(const TCHAR* Text);
	SName(const class CString& String);

	[[nodiscard]] FORCEINLINE const TCHAR* operator*() const arrow( ToString() )

	[[nodiscard]] const TCHAR* ToString() const
	{
		return CNameTable::Get().GetName(DisplayIndex);
	}
	
	[[nodiscard]] int32 GetIndex() const arrow (DisplayIndex)
	
	bool operator==(const SName& Other) const
	{
		return DisplayIndex == Other.DisplayIndex;
	}

	bool operator!=(const SName& Other) const
	{
		return DisplayIndex != Other.DisplayIndex;
	}
	
private:
	int32 DisplayIndex;
};
