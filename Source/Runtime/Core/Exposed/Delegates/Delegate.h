// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <functional>

#include "Misc/Assertions.h"
#include "Containers/Map.h"

template <typename Signature>
class TDelegate;

template <typename ReturnType, typename... ArgTypes>
class TDelegate<ReturnType(ArgTypes...)>
{
	using CallbackType = std::function<ReturnType(ArgTypes...)>;
	
public:
	void Bind(CallbackType InCallback)
	{
		Callback = SMemory::MoveTemp(InCallback);
	}

	ReturnType Execute(ArgTypes... Args) const
	{
		return ENSURE_M(IsBound(), "Executed an unbound delegate!") ? Callback(Args...) : ReturnType();
	}

	[[nodiscard]] bool IsBound() const arrow (!!Callback)
	
private:
	CallbackType Callback;
};
