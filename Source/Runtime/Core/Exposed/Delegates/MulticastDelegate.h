// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include <functional>

#include "DelegateUtils.h"
#include "Containers/Map.h"

struct SDelegateHandle
{
	SDelegateHandle(const uint64 InIdentifier) : Identifier(InIdentifier) { }

	FORCEINLINE uint64 GetIdentifier() const arrow (Identifier)
	FORCEINLINE bool IsValid() const arrow (Identifier != 0)

	bool operator==(const SDelegateHandle& Other) const arrow (Other.Identifier == Identifier)
	
private:
	uint64 Identifier = 0;
};

template <typename Signature>
class TMulticastDelegate;

template <typename ReturnType, typename... ArgTypes>
class TMulticastDelegate<ReturnType(ArgTypes...)>
{
	using CallbackType = std::function<ReturnType(ArgTypes...)>;
	using SCallbackEntry = TCallbackEntry<CallbackType>;

public:
	/** Binds a lambda or static function. */
	SDelegateHandle Subscribe(CallbackType Callback)
	{
		SDelegateHandle Handle = SDelegateHandle(CurrentHandleIndex++);
		
		Callbacks.Add(Handle, SCallbackEntry{ SMemory::MoveTemp(Callback), nullptr });
		return Handle;
	}
	
	/** Binds a callback method. */
	template <typename ClassType>
	SDelegateHandle Subscribe(ClassType* Object, ReturnType (ClassType::*Method)(ArgTypes...))
	{
		SDelegateHandle Handle = SDelegateHandle(CurrentHandleIndex++);
       
		CallbackType Callback = [Object, Method](ArgTypes... Args) {
			(Object->*Method)(Args...);
		};
       
		Callbacks.Add(Handle, SCallbackEntry{ SMemory::MoveTemp(Callback), Object });
		return Handle;
	}
	
	template <typename ClassType>
	SDelegateHandle Subscribe(const ClassType* Object, ReturnType (ClassType::*Method)(ArgTypes...) const)
	{
		SDelegateHandle Handle = SDelegateHandle(CurrentHandleIndex++);
       
		CallbackType Callback = [Object, Method](ArgTypes... Args) {
			(Object->*Method)(Args...);
		};
       
		Callbacks.Add(Handle, SCallbackEntry{ SMemory::MoveTemp(Callback), Object });
		return Handle;
	}
	
	void Unsubscribe(SDelegateHandle Handle)
	{
		Callbacks.Remove(Handle);
	}

	void UnsubscribeAll()
	{
		Callbacks.Clear();
	}

	/** Unbinds all callbacks associated with the passed object. */
	void UnsubscribeAll(const void* Object)
	{
		if (!Object) return;
		
		Callbacks.RemoveAllByPredicate([Object](const TPair<SDelegateHandle, SCallbackEntry>& Pair) arrow( Pair.Value.Object == Object ));
	}
	
	void Broadcast(ArgTypes... Args) const
	{
		for (TPair<SDelegateHandle, SCallbackEntry> Callback : Callbacks)
		{
			Callback.Value.Callback(Args...);
		}
	}

private:
	TMap<SDelegateHandle, SCallbackEntry> Callbacks;
	uint64 CurrentHandleIndex = 0;
};
