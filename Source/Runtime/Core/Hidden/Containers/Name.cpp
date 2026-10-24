// CopyRight FlareHorz Team. All Rights Reserved.


#include "Containers/Name.h"

#include "Containers/String.h"
#include "Logging/Logger.h"

#define LOG_CATEGORY "NameTable"

CNameTable::CNameTable()
{
	#define REGISTER_NAME(Index, Name) \
		Entries.Insert(SNameEntry(#Name), Index); \
		FH_MISC_LOG(Trace, "Registered name[%d]: %s", Index, #Name);

	#include "Containers/FlareNames.inl"

	#undef REGISTER_NAME
}

CNameTable& CNameTable::Get()
{
	static CNameTable Instance = CNameTable();
	return Instance;
}

int32 CNameTable::FindOrAdd(const TCHAR* Text)
{
	// check if name already exists
	for (int32 i = 0; i < Entries.Num(); ++i)
	{
		if (CTextHelpers::Compare(Entries[i].Name, Text, ETextCase::IgnoreCase) == 0)
		{
			return i;
		}
	}
	
	Entries.Add(SNameEntry(Text));
	FH_MISC_LOG(Debug, "Added name[%d] to the table: %s", Entries.Num() - 1, Text)
	
	return Entries.Num() - 1;
}

const TCHAR* CNameTable::GetName(const int32 Index) const
{
	if (Index < Entries.Num())
	{
		return Entries[Index].Name;
	}
	return Entries[0].Name;
}

SName::SName(EName Name)
{
	DisplayIndex = static_cast<int32>(Name);
}

SName::SName(const TCHAR* Text)
{
	DisplayIndex = CNameTable::Get().FindOrAdd(Text);
}

SName::SName(const CString& String)
{
	DisplayIndex = CNameTable::Get().FindOrAdd(*String);
}

#undef LOG_CATEGORY

