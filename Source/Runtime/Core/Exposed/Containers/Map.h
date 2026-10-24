// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Array.h"

template <typename KeyType, typename ValueType>
class TPair
{
public:
	TPair() = default;

	TPair(const KeyType& InKey, const ValueType& InValue)
		: Key(InKey), Value(InValue) {}

	TPair(KeyType&& InKey, ValueType&& InValue)
		: Key(SMemory::MoveTemp(InKey)), Value(SMemory::MoveTemp(InValue)) {}

	/** Copy constructor. */
	TPair(const TPair& Other)
		: Key(Other.Key), Value(Other.Value) {}

	TPair& operator=(const TPair& Other)
	{
		if (this != &Other)
		{
			Key = Other.Key;
			Value = Other.Value;
		}
		return *this;
	}
	
	/** Move constructor. */
	TPair(TPair&& Other) noexcept
		: Key(SMemory::MoveTemp(Other.Key)), Value(SMemory::MoveTemp(Other.Value)) {}

	TPair& operator=(TPair&& Other) noexcept
	{
		if (this != &Other)
		{
			Key = SMemory::MoveTemp(Other.Key);
			Value = SMemory::MoveTemp(Other.Value);
		}
		return *this;
	}
	
	KeyType Key;
	ValueType Value;
};

template <typename KeyType, typename ValueType>
class TMap
{
	using PairType = TPair<KeyType, ValueType>;
	
public:
	//@TODO: Maps are only partly implemented

	TMap()
	{
		Pairs = TArray<PairType>();
	}

	TMap(std::initializer_list<PairType> InitList)
	{
		Pairs = TArray<PairType>(InitList);
	}
	
	/**
	 * Set the value associated with a key.
	 *
	 * @param Key The key to associate the value with.
	 * @param Value The value to associate with the key.
	 */
	void Add(const KeyType& Key, const ValueType& Value)
	{
		for (ArraySizeType i = 0; i < Pairs.Num(); ++i)
		{
			if (Pairs[i].Key == Key)
			{
				Pairs[i].Value = Value;
				return;
			}
		}
		Pairs.Add(PairType(Key, Value));
	}
	
	/**
	 * Remove all value associations for a key.
	 *
	 * @param Key The key to remove associated values for.
	 * @return The number of values that were associated with the key.
	 */
	void Remove(const KeyType& Key)
	{
		for (ArraySizeType i = 0; i < Pairs.Num(); )
		{
			if (Pairs[i].Key == Key)
			{
				RemoveAt(i);
			}
			else
			{
				++i;
			}
		}
	}
	
	/**
	 * Removes the element at the specified index.
	 * 
	 * @param Index The index of the element to remove.
	 */
	FORCEINLINE void RemoveAt(const int32 Index)
	{
		Pairs.RemoveAt(Index);
	}
	
	/**
	 * Check if map contains the specified key.
	 *
	 * @param Key The key to check for.
	 * @return true if the map contains the key.
	 */
	[[nodiscard]] FORCEINLINE bool Contains(const KeyType& Key) const
	{
		return Find(Key) != nullptr;
	}
	
	/**
	 * Find the value associated with a specified key.
	 *
	 * @param Key The key to search for.
	 */
	[[nodiscard]] FORCEINLINE const ValueType* Find(const KeyType& Key) const
	{
		for (ArraySizeType i = 0; i < Pairs.Num(); ++i)
		{
			if (Pairs[i].Key == Key)
			{
				return &Pairs[i].Value;
			}
		}
		return nullptr;
	}
	
	/** Removes all elements from the map. */
	FORCEINLINE void Clear()
	{
		Pairs.Clear();
	}
	
	FORCEINLINE void Shrink()
	{
		Pairs.Shrink();
	}
	
	/** @return he number of elements in the map. */
	[[nodiscard]] FORCEINLINE int32 Num() const
	{
		return Pairs.Num();
	}
	
private:
	/** A set of the key-value pairs in the map. */
	TArray<PairType> Pairs;

public:
	/**
	 * DO NOT USE DIRECTLY!
	 * STL-like iterators to enable range-based for loop support.
	 */
	PairType*			begin()			arrow(Pairs.begin())
	PairType*			end()			arrow(Pairs.end())
	const PairType*		begin() const	arrow(Pairs.begin())
	const PairType*		end() const		arrow(Pairs.end())

	/*----------------------------------------------------------------------------
		Operators
	----------------------------------------------------------------------------*/

	ValueType& operator[](const KeyType& Key)
	{
		for (ArraySizeType i = 0; i < Pairs.Num(); ++i)
		{
			if (Pairs[i].Key == Key)
			{
				return Pairs[i].Value;
			}
		}
		Pairs.Add(PairType(Key, ValueType()));
		
		return Pairs.Last().Value;
	}
};
