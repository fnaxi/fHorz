// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <functional>

#include "DelegateUtils.h"
#include "Containers/Map.h"

template <typename Signature>
class TDelegate;

template <typename ReturnType, typename... ArgTypes>
class TDelegate<ReturnType(ArgTypes...)>
{
	using CallbackType = std::function<ReturnType(ArgTypes...)>;
	using SCallbackEntry = TCallbackEntry<CallbackType>;
	
public:
	void Bind(CallbackType InCallback)
	{
		Entry.Callback = SMemory::MoveTemp(InCallback);
		Entry.Object = nullptr;
	}
	
	template <typename ClassType>
	void Bind(ClassType* Object, ReturnType (ClassType::*Method)(ArgTypes...))
	{
		Entry.Callback = [Object, Method](ArgTypes... Args) -> ReturnType arrow( (Object->*Method)(Args...) );
		Entry.Object = Object;
	}

	template <typename ClassType>
	void Bind(ClassType* Object, ReturnType (ClassType::*Method)(ArgTypes...) const)
	{
		Entry.Callback = [Object, Method](ArgTypes... Args) -> ReturnType arrow( (Object->*Method)(Args...) );
		Entry.Object = Object;
	}
	
	void Unbind()
	{
		Entry.Callback = nullptr;
		Entry.Object = nullptr;
	}

	[[nodiscard]] bool IsBound() const arrow (!!Entry.Callback)

	ReturnType Execute(ArgTypes... Args) const
	{
		return ENSURE_M(IsBound(), "Executed an unbound delegate!") ? Entry.Callback(Args...) : ReturnType();
	}
	ReturnType ExecuteIfBound(ArgTypes... Args) const arrow (IsBound() ? Entry.Callback(Args...) : ReturnType())

private:
	SCallbackEntry Entry;
};
