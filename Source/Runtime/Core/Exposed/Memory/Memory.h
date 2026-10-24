// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Misc/Atomic.h"
#include "Misc/CoreDefines.h"
#include "Platform/Platform.h"

#if CONFIGURATION_SHIPPING
	#define WITH_MEMORY_TRACKING 0
#else
	#define WITH_MEMORY_TRACKING 1
#endif

enum : uint8
{
	DEFAULT_MEMORY_ALIGNMENT = 16
};

/*enum class EMemoryCategory : uint8
{
	General,
	Renderer,
	Physics,
	Audio,
	UserInterface,

	Internal_Num
};*/

/** A pointer-only wrapper around static_cast. */
template <typename NewType, typename OldType>
NewType* Cast(OldType* Source) arrow( static_cast<NewType*>(Source) )

struct SMemory
{
	SMemory()							= delete;
	~SMemory()							= delete;
	SMemory(const SMemory&)				= delete;
	SMemory& operator=(const SMemory&)	= delete;

	/*----------------------------------------------------------------------------
		Memory allocation
	----------------------------------------------------------------------------*/
	
	static void* Malloc(SIZE_T Count, SIZE_T Alignment = DEFAULT_MEMORY_ALIGNMENT);

	template <typename Type>
	static Type* Malloc(SIZE_T Count, SIZE_T Alignment = DEFAULT_MEMORY_ALIGNMENT) arrow( Cast<Type>(Malloc(Count * sizeof(Type), Alignment)) )
	
	static void* Realloc(void* Original, SIZE_T Count, SIZE_T Alignment = DEFAULT_MEMORY_ALIGNMENT);
	
	static void Free(void* Original);
	
	/*----------------------------------------------------------------------------
		Memory manipulation
	----------------------------------------------------------------------------*/

	static void* Memmove(void* Destination, const void* Source, SIZE_T Count);
	static int32 Memcmp(const void* BufferA, const void* BufferB, SIZE_T Count);

	static void* Memset(void* Destination, uint8 Char, SIZE_T Count);
	
	template <class Type>
	static FORCEINLINE void Memset( Type& Source, uint8 Char )
	{
		SMemory::Memset( &Source, Char, sizeof(Type) );
	}
	
	static void* Memzero(void* Destination, SIZE_T Count);
	
	template <class Type> 
	static FORCEINLINE void Memzero( Type& Source )
	{
		SMemory::Memzero( &Source, sizeof(Type) );
	}

	static void* Memcpy(void* Destination, const void* Source, SIZE_T Count);
	
	template <class Type> 
	static FORCEINLINE void Memcpy( Type& Destination, const Type& Source )
	{
		SMemory::Memcpy( &Destination, &Source, sizeof(Type) );
	}
	
	/*----------------------------------------------------------------------------
		Memory tracking
	----------------------------------------------------------------------------*/

#if WITH_MEMORY_TRACKING
	static double GetTotalMemoryUsed()
	{
		return static_cast<double>(TotalMemoryUsed.load(std::memory_order_relaxed)) / (1024.0 * 1024.0);
	}
#endif

	/*----------------------------------------------------------------------------
		Misc
	----------------------------------------------------------------------------*/
	
	template <typename Type>
	static constexpr std::remove_reference_t<Type>&& MoveTemp(Type&& Object) noexcept
	{
		using CastType = std::remove_reference_t<Type>;
		
		static_assert(std::is_lvalue_reference_v<Type>, "SMemory::MoveTemp() called on an rvalue");
		static_assert(!std::is_same_v<CastType&, const CastType&>, "SMemory::MoveTemp() called on a const object");
		
		return (CastType&&)Object;
	}

	template <typename Type>
	static constexpr void Swap(Type& Left, Type& Right) noexcept(std::is_nothrow_move_constructible_v<Type> && std::is_nothrow_move_assignable_v<Type>)
	{
		static_assert(std::is_move_constructible_v<Type>, "SMemory::Swap() called on a type that is not move constructible");
		static_assert(std::is_move_assignable_v<Type>, "SMemory::Swap() called on a type that is not move assignable");

		Type Temp = MoveTemp(Left);
		
		Left = MoveTemp(Right);
		Right = MoveTemp(Temp);
	}

	template <typename Type, SIZE_T SizeType>
	static constexpr void Swap(Type (&Left)[SizeType], Type (&Right)[SizeType]) noexcept(noexcept(Swap(Left[0], Right[0])))
	{
		if (&Left == &Right) return; // Handle self-swap as a no-op

		for (SIZE_T i = 0; i < SizeType; ++i)
		{
			Swap(Left[i], Right[i]);
		}
	}

	template <typename Type>
	[[nodiscard]] static constexpr Type&& Forward(std::remove_reference_t<Type>& Object) noexcept
	{
		return static_cast<Type&&>(Object);
	}
	
	template <typename Type>
	[[nodiscard]] static constexpr Type&& Forward(std::remove_reference_t<Type>&& Object) noexcept
	{
		static_assert(!std::is_lvalue_reference_v<Type>, "SMemory::Forward() called on an rvalue with an lvalue reference type!");
		return static_cast<Type&&>(Object);
	}
	
private:
	/** Ensure alignment is at least the pointer size and a valid power of 2. */
	static void EnsureMinimumAlignment(SIZE_T& Alignment);

#if WITH_MEMORY_TRACKING
	/** Total memory used in bytes. */
	inline static TAtomic<SIZE_T> TotalMemoryUsed {0};

	// inline static TAtomic<SIZE_T> MemoryUsed[static_cast<SIZE_T>(EMemoryCategory::Internal_Num)] = {};

	/**
	 * Tracks metadata about allocation to allow memory tracking.
	 */
	struct SAllocationHeader
	{
		SIZE_T Size;
		
		// EMemoryCategory Category;
	};
#endif
};

template <typename Type, typename... ArgTypes>
Type* NewObject(ArgTypes&&... Args)
{
	void* Memory = SMemory::Malloc(sizeof(Type));
	
	return ::new (Memory) Type(SMemory::Forward<ArgTypes>(Args)...);
}

template <typename Type>
void DestroyObject(Type* Source)
{
	if (Source)
	{
		Source->~Type();
		
		SMemory::Free(Source);
	}
}
