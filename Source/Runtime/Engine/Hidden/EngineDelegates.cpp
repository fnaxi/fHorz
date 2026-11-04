// CopyRight FlareHorz Team. All Rights Reserved.


#include "EngineDelegates.h"

TMulticastDelegate<void()>				SEngineDelegates::OnWindowCreated;
TMulticastDelegate<void(SDL_Event*)>	SEngineDelegates::OnProcessEvent;

