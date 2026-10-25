// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Logging/LogCategory.h"

DECLARE_LOG_CATEGORY(lEngine, All)

class CEngine
{
public:
	CEngine() = default;
	virtual ~CEngine() = default;
	
	int32	Initialize();

	void	Tick();

	void	Shutdown();
	
	/**
	 * Requests the engine exit as soon as it can safely do so
	 * The reason string is not optional and should be a useful description of why the engine exit is requested.
	 */
	void	RequestExit(const TCHAR* Reason);

	FORCEINLINE bool IsExitRequested() const arrow (bRequestingExit)
	
private:
	/** Indicates that main loop should be exited at the end of the current iteration */
	bool bRequestingExit = false;
};

extern ENGINE_API CEngine* GEngine;
