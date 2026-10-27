// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#include "Platform/Platform.h"

class CLaunchContext
{
public:
	/**
	 * Loads core modules.
	 * 
	 * @return The error level; 0 if successful, > 0 if there were errors.
	 */
	int32	Configure();

	/**
	 * Create and initialize a global engine instance.
	 * 
	 * @return The error level; 0 if successful, > 0 if there were errors.
	 */
	int32	Initialize();

	/** Every frame: pull input, tick the engine, kick of rendering, update RHI. */
	void	RunEngineLoop();

	/** Detaches modules and run cleanup. */
	void	Shutdown();
};
