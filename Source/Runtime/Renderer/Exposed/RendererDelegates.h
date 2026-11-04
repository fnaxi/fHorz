// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Delegates/MulticastDelegate.h"

struct SRendererDelegates
{
	static TMulticastDelegate<void(struct SDL_Window*, struct SDL_Renderer*)> OnInitialized;

	static TMulticastDelegate<void()> OnBeginFrame;
	static TMulticastDelegate<void()> OnEndFrame;
};
