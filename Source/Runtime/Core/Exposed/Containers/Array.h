// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <algorithm>
#include <initializer_list>

#include "Memory/Memory.h"
#include "Misc/Assertions.h"

using ArraySizeType = int32;
static_assert(std::is_signed_v<ArraySizeType>, "TArray only supports signed index types");

enum : int8  { INDEX_NONE	= -1 };

/**
 * A dynamically sized array of typed elements.
 */
template <typename ElementType>
class TArray
{
	enum : uint8 { ARRAY_INITIAL_SIZE = 4 };

public:
	
	TArray() : Data(nullptr), ArrayNum(0), ArrayMax(0)
	{
		Data = SMemory::Malloc<ElementType>(ARRAY_INITIAL_SIZE);
	}
	
	explicit TArray(const ArraySizeType Size) : ArrayNum(0), ArrayMax(Size)
	{
		Data = SMemory::Malloc<ElementType>(Size > 0 ? ArrayMax : ARRAY_INITIAL_SIZE);
	}

	TArray(std::initializer_list<ElementType> InitList) : Data(nullptr)
	{
		ArrayNum = static_cast<ArraySizeType>(InitList.size());
		ArrayMax = ArrayNum;

		if (ArrayMax > 0)
		{
			Data = SMemory::Malloc<ElementType>(ArrayMax);

			ArraySizeType i = 0;
			for (const ElementType& Item : InitList)
			{
				::new (Data + i) ElementType(Item);
				i++;
			}
		}
	}

	// Copy constructor
	TArray(const TArray& Other) : ArrayNum(Other.ArrayNum), ArrayMax(Other.ArrayMax)
	{
		Data = ArrayMax > 0 ? SMemory::Malloc<ElementType>(ArrayMax) : nullptr;
		for (ArraySizeType i = 0; i < ArrayNum; ++i)
		{
			::new (Data + i) ElementType(Other.Data[i]);
		}
	}

	// Copy assignment
	TArray& operator=(const TArray& Other)
	{
		TArray(Other).Swap(*this); //@TODO: revisit this
		return *this;
	}

	// Move constructor
	TArray(TArray&& Other) noexcept : Data(Other.Data), ArrayNum(Other.ArrayNum), ArrayMax(Other.ArrayMax)
	{
		Other.Data = nullptr;
		Other.ArrayNum = 0;
		Other.ArrayMax = 0;
	}

	// Move assignment
	TArray& operator=(TArray&& Other) noexcept
	{
		if (this != &Other)
		{
			Clear();
			SMemory::Free(Data);

			Data = Other.Data;
			ArrayNum = Other.ArrayNum;
			ArrayMax = Other.ArrayMax;

			Other.Data = nullptr;
			Other.ArrayNum = 0;
			Other.ArrayMax = 0;
		}
		return *this;
	}

	~TArray()
	{
		Clear();
		SMemory::Free(Data);
	}

	void Clear() noexcept
	{		
		DestructItems();
		ArrayNum = 0;
	}

	void Add(const ElementType& Value)
	{
		if (ArrayNum == ArrayMax)
		{
			Reserve(ArrayMax == 0 ? 1 : ArrayMax * 2);
		}

		::new (Data + ArrayNum) ElementType(Value);
		ArrayNum++;
	}

	void Append(std::initializer_list<ElementType> InitList)
	{
		const ArraySizeType NumToAdd = static_cast<ArraySizeType>(InitList.size());
		if (NumToAdd == 0) return;
		
		if (ArrayNum + NumToAdd > ArrayMax)
		{
			Reserve(ArrayNum + NumToAdd); 
		}

		for (const ElementType& Item : InitList)
		{
			::new (Data + ArrayNum) ElementType(Item);
			ArrayNum++;
		}
	}

	/**
	 * Inserts a given element into the array at given location.
	 *
	 * @returns Location at which the insert was done.
	 */
	ArraySizeType Insert(const ElementType& Item, ArraySizeType Index)
	{
		if (ArrayNum == ArrayMax)
		{
			Reserve(ArrayMax == 0 ? 1 : ArrayMax * 2);
		}
		
		::new(GetData() + Index) ElementType(Item);
		ArrayNum++;
		return Index;
	}
	
	/**
	 * Removes an element at given location.
	 *
	 * @returns True if element was removed, false otherwise.
	 */
	void RemoveAt(const ArraySizeType Index)
	{
		if (!IsValidIndex(Index)) return;

		// Shift elements
		for (ArraySizeType i = Index; i < ArrayNum - 1; ++i)
		{
			Data[i] = SMemory::MoveTemp(Data[i + 1]);
		}

		Data[ArrayNum - 1].~ElementType();
		ArrayNum--;
	}

	/**
	 * Removes as many instances of Item as there are in the array, maintaining order but not indices.
	 *
	 * @returns The number of items removed.
	 */
	ArraySizeType Remove(const ElementType& Item)
	{
		ArraySizeType RemovedCount = 0;
		for( ArraySizeType i = 0; i < ArrayNum; )
		{
			if (Data[i] == Item)
			{
				RemoveAt(i);
				++RemovedCount;
			}
			else
			{
				++i;
			}
		}
		return RemovedCount;
	}

	/**
	 * Finds element within the array.
	 *
	 * @returns Index of the found element. INDEX_NONE otherwise.
	 */
	[[nodiscard]] ArraySizeType Find(const ElementType& Item) const
	{
		for (ArraySizeType i = 0; i < ArrayNum; ++i)
		{
			if (Data[i] == Item)
			{
				return i;
			}
		}
		return INDEX_NONE;
	}
	
	/**
	 * Finds element within the array.
	 *
	 * @param Item Item to look for.
	 * @param Index Will contain the found index.
	 * @returns True if found. False otherwise.
	 */
	[[nodiscard]] bool Find(const ElementType& Item, ArraySizeType& Index) const
	{
		Index = Find(Item);
		return Index != INDEX_NONE;
	}
	
	/**
	 * Finds an element which matches a predicate functor.
	 *
	 * @param Predicate The functor to apply to each element. true, or nullptr if none is found.
	 */
	template <typename PredicateType>
	[[nodiscard]] ElementType* FindByPredicate(PredicateType Predicate) const
	{
		for (ArraySizeType i = 0; i < ArrayNum; ++i)
		{
			if (Predicate(Data[i]))
			{
				return &Data[i];
			}
		}
		return nullptr;
	}

	template <typename PredicateType>
	[[nodiscard]] ElementType* FindByPredicate(PredicateType Predicate) arrow(FindByPredicate(Predicate))
	
	void SetNum(const ArraySizeType NewNum)
	{
		if (NewNum == ArrayNum) return;

		if (NewNum > ArrayNum)
		{
			if (NewNum > ArrayMax)
			{
				Reserve(NewNum);
			}

			for (ArraySizeType i = ArrayNum; i < NewNum; ++i)
			{
				::new (Data + i) ElementType();
			}
		}
		else
		{
			if constexpr (!std::is_trivially_destructible_v<ElementType>)
			{
				for (ArraySizeType i = NewNum; i < ArrayNum; ++i)
				{
					Data[i].~ElementType();
				}
			}
		}
	}

	/** Returns the first element. */
	[[nodiscard]] ElementType& First() arrow( GetElement(0) )

	/** Returns the last element. */
	[[nodiscard]] ElementType& Last() arrow( GetElement(ArrayNum - 1) )

	[[nodiscard]] FORCEINLINE int32 LastIndex() const arrow (ArrayNum - 1)
	
	/** Tests if index is valid, i.e. greater than or equal to zero, and less than the number of elements in the array. */
	[[nodiscard]] FORCEINLINE bool IsValidIndex(const ArraySizeType Index) const
	{
		return Index >= 0 && Index < ArrayNum;
	}
	
	void Shrink()
	{
		if (ArrayNum == ArrayMax) return;
		
		if (IsEmpty()) // free the memory completely if empty
		{
			SMemory::Free(Data);
			Data = nullptr;
			ArrayMax = 0;
			return;
		}

		Reallocate(ArrayNum);
	}
	
	/** Returns the amount of slack in this array in elements. */
	[[nodiscard]] FORCEINLINE ArraySizeType GetSlack() const arrow( ArrayMax - ArrayNum )
	
	/** Returns true if the array is empty and contains no elements. */
	[[nodiscard]] bool IsEmpty() const arrow (ArrayNum == 0)

	/** Returns current number of elements in the array. */
	[[nodiscard]] FORCEINLINE ArraySizeType Num() const arrow (ArrayNum)

	/** Returns maximum number of elements in array. */
	[[nodiscard]] FORCEINLINE ArraySizeType Max() const arrow (ArrayMax)

	/** Returns a typed pointer to the first array entry. */
	[[nodiscard]] FORCEINLINE ElementType* GetData() arrow (Data)
	[[nodiscard]] FORCEINLINE const ElementType* GetData() const arrow (Data)
	
private:
	
	ElementType* Data;

	/** Current number of elements in the array. */
	ArraySizeType ArrayNum;
	
	/** Current allocated size of the array. */
	ArraySizeType ArrayMax;
	
	void Reserve(const ArraySizeType Size)
	{
		if (Size <= ArrayMax) return;
		
		Reallocate(Size);
	}

	void Reallocate(const ArraySizeType Size)
	{
		ElementType* NewData = SMemory::Malloc<ElementType>(Size);
		for (ArraySizeType i = 0; i < ArrayNum; ++i)
		{
			::new (NewData + i) ElementType(SMemory::MoveTemp(Data[i]));
			Data[i].~ElementType();
		}

		SMemory::Free(Data);
		Data = NewData;

		ArrayMax = Size;
	}
	
	void DestructItems()
	{
		if constexpr (!std::is_trivially_destructible_v<ElementType>)
		{
			for (ArraySizeType i = 0; i < ArrayNum; ++i)
			{
				Data[i].~ElementType();
			}
		}
	}

	[[nodiscard]] ElementType& GetElement(const ArraySizeType Index)
	{
		STRICT_ENSURE(IsValidIndex(Index));
		return GetData()[Index];
	}
	[[nodiscard]] const ElementType& GetElement(const ArraySizeType Index) const
	{
		STRICT_ENSURE(IsValidIndex(Index));
		return GetData()[Index];
	}
	
public:
	
	/**
	 * DO NOT USE DIRECTLY!
	 * STL-like iterators to enable range-based for loop support.
	 */
	ElementType*		begin()			arrow( GetData() )
	ElementType*		end()			arrow( GetData() + ArrayNum )
	const ElementType*	begin() const	arrow( GetData() )
	const ElementType*	end() const		arrow( GetData() + ArrayNum )

	void Swap(TArray& Other) noexcept
	{
		SMemory::Swap(Data,		Other.Data);
		SMemory::Swap(ArrayNum,	Other.ArrayNum);
		SMemory::Swap(ArrayMax,	Other.ArrayMax);
	}
	
	/*----------------------------------------------------------------------------
		Operators
	----------------------------------------------------------------------------*/

	ElementType& operator[](const ArraySizeType Index) arrow( GetElement(Index) )
	
	const ElementType& operator[](const ArraySizeType Index) const arrow( GetElement(Index) )

	bool operator==(const TArray& Other) const
	{
		if (this == &Other)
		{
			return true;
		}
		if (ArrayNum != Other.ArrayNum)
		{
			return false;
		}

		for (ArraySizeType i = 0; i < ArrayNum; ++i)
		{
			if (Data[i] != Other.Data[i])
			{
				return false;
			}
		}

		return true;
	}
	bool operator!=(const TArray& Other) const arrow( !(this == &Other) )
};
