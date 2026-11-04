// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

template <typename CallbackType>
struct TCallbackEntry
{
	CallbackType Callback;

	/** @note this is nullptr for lambdas and static functions. */
	const void* Object = nullptr;
};
