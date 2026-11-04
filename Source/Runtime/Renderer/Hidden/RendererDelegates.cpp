// CopyRight FlareHorz Team. All Rights Reserved.


#include "RendererDelegates.h"

TMulticastDelegate<void(struct SDL_Window*, struct SDL_Renderer*)> SRendererDelegates::OnInitialized;

TMulticastDelegate<void()> SRendererDelegates::OnBeginFrame;
TMulticastDelegate<void()> SRendererDelegates::OnEndFrame;

