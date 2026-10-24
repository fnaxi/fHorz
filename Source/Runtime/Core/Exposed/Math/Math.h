// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

struct SMath
{	
	/** Compares two values and returns a reference to the larger one. */
	template <typename Type>
	static const Type& Max(const Type& A, const Type& B)
	{
		return (A < B) ? B : A;
	}

	/** Compares two values and returns a reference to the smallest one. */
	template <typename Type>
	static const Type& Min(const Type& A, const Type& B)
	{
		return (A < B) ? A : B;
	}
};
