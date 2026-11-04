// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Delegates/MulticastDelegate.h"

struct SEngineDelegates
{
	static TMulticastDelegate<void()>					OnWindowCreated;
	static TMulticastDelegate<void(union SDL_Event*)>	OnProcessEvent;
};
