// CopyRight FlareHorz Team. All Rights Reserved.


#include "Memory/Memory.h"

#include <memory>

#include "Math/Math.h"
#include "Logging/LogCategory.h"
#include "Logging/Logger.h"

#define LOG_CATEGORY "Memory"

void* SMemory::Malloc(const SIZE_T Count, SIZE_T Alignment)
{
	EnsureMinimumAlignment(Alignment);
	
#if WITH_MEMORY_TRACKING
	const SIZE_T Size = Count + sizeof(SAllocationHeader);
	
	void* Source = malloc(Size); // _aligned_malloc(Size, Alignment)
	if (Source == nullptr)
	{
		FH_MISC_LOG(Error, "Out of memory! Failed to allocate %llu bytes! (Total used: %f MBs)", Size, GetTotalMemoryUsed())
		return nullptr;
	}

	if (SAllocationHeader* Header = Cast<SAllocationHeader>(Source))
	{
		Header->Size = Size;
	}
	
	TotalMemoryUsed.fetch_add(Size, std::memory_order_relaxed);
	
	FH_MISC_LOG(Trace, "Allocated %llu bytes (Total used: %f MBs)", Size, GetTotalMemoryUsed())
	
	return Cast<uint8>(Source) + sizeof(SAllocationHeader);
#else
	return malloc(Count); // _aligned_malloc(Count, Alignment)
#endif
}

void* SMemory::Realloc(void* Original, SIZE_T Count, SIZE_T Alignment)
{
	if (!Original)
	{
		return Malloc(Count, Alignment);
	}

	if (Count == 0)
	{
		Free(Original);
		return nullptr;
	}
	
	EnsureMinimumAlignment(Alignment);

#if WITH_MEMORY_TRACKING
	// Old source
	uint8* OldSource = Cast<uint8>(Original) - sizeof(SAllocationHeader);

	SAllocationHeader OldHeader;
	Memcpy(&OldHeader, OldSource, sizeof(SAllocationHeader));

	const SIZE_T OldSize = OldHeader.Size;
	
	// New Source
	const SIZE_T NewSize = Count + sizeof(SAllocationHeader);

	void* NewSource = realloc(OldSource, NewSize); // _aligned_realloc(OldSource, NewSize, Alignment)
	if (NewSource == nullptr)
	{
		FH_MISC_LOG(Error, "Memory: Re-allocation has failed! (Total used: %f MBs)", GetTotalMemoryUsed())
		return nullptr;
	}

	if (SAllocationHeader* NewHeader = Cast<SAllocationHeader>(NewSource))
	{
		NewHeader->Size = NewSize;
	}

	TotalMemoryUsed.fetch_add(NewSize - OldSize, std::memory_order_relaxed);
	
	FH_MISC_LOG(Trace, "Re-allocated from %llu to %llu bytes (Total used: %f MBs)", OldSize, NewSize, GetTotalMemoryUsed())

	return static_cast<uint8*>(NewSource) + sizeof(SAllocationHeader);
#else
	return realloc(Original, Count); // _aligned_realloc(Original, Count, Alignment)
#endif
}

void SMemory::Free(void* Original)
{
	if (!Original) return;
	
#if WITH_MEMORY_TRACKING
	void* Source = Cast<uint8>(Original) - sizeof(SAllocationHeader);
	if (!Source) return;

	if (const SAllocationHeader* Header = Cast<SAllocationHeader>(Source))
	{
		TotalMemoryUsed.fetch_sub(Header->Size, std::memory_order_relaxed);

		FH_MISC_LOG(Trace, "Freed %llu bytes (Total used: %f MBs)", Header->Size, GetTotalMemoryUsed())
	}

	free(Source); // _aligned_free(Source)
#else
	free(Original); // _aligned_free(Original)
#endif
}

void* SMemory::Memmove(void* Destination, const void* Source, SIZE_T Count)
{
	return std::memmove(Destination, Source, Count);
}

int32 SMemory::Memcmp(const void* BufferA, const void* BufferB, SIZE_T Count)
{
	return std::memcmp(BufferA, BufferB, Count);
}

void* SMemory::Memset(void* Destination, uint8 Char, SIZE_T Count)
{
	return std::memset(Destination, Char, Count);
}

void* SMemory::Memzero(void* Destination, SIZE_T Count)
{
	return std::memset(Destination, 0, Count);
}

void* SMemory::Memcpy(void* Destination, const void* Source, SIZE_T Count)
{
	return std::memcpy(Destination, Source, Count);
}

void SMemory::EnsureMinimumAlignment(SIZE_T& Alignment)
{
	Alignment = SMath::Max(Alignment, sizeof(void*));
}

#undef LOG_CATEGORY

