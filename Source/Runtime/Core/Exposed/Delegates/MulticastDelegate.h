// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <functional>

#include "Containers/Map.h"

template <typename Signature>
class TMulticastDelegate;

template <typename ReturnType, typename... ArgTypes>
class TMulticastDelegate<ReturnType(ArgTypes...)>
{
	using CallbackType = std::function<ReturnType(ArgTypes...)>;
	using HandleType = uint64;

public:
	HandleType Subscribe(CallbackType Callback)
	{
		HandleType Handle = CurrentHandleIndex++;
		
		Callbacks.Add(Handle, SMemory::MoveTemp(Callback));
		return Handle;
	}
	void Unsubscribe(HandleType Handle)
	{
		Callbacks.Remove(Handle);
	}

	void Clear()
	{
		Callbacks.Clear();
	}
	
	void Broadcast(ArgTypes... Args) const
	{
		for (TPair<HandleType, CallbackType> Callback : Callbacks)
		{
			Callback.Value(Args...);
		}
	}

private:
	TMap<HandleType, CallbackType> Callbacks;
	HandleType CurrentHandleIndex = 0;
};
