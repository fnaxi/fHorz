// CopyRight FlareHorz Team. All Rights Reserved.


#include "Engine.h"

#include "Logging/Logger.h"

DEFINE_LOG_CATEGORY(lEngine)

ENGINE_API CEngine* GEngine = nullptr;

int32 CEngine::Initialize()
{
	FH_LOG(Info, lEngine, "Engine initialized")
	return 0;
}

void CEngine::Tick()
{
	RequestExit("some reason");
}

void CEngine::Shutdown()
{
}

void CEngine::RequestExit(const TCHAR* Reason)
{
	FH_LOG(Info, lEngine, "Engine exit requested (reason: %s%s)", Reason, bRequestingExit ? "; note: exit was already requested" : "")
	bRequestingExit = true;
}

