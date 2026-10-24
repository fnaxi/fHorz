// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

class IModuleInterface
{
public:
	/** Note: Even though this is an interface class we need a virtual destructor here because modules are deleted via a pointer to this interface. */
	virtual ~IModuleInterface() = default;

	virtual void MountModule() { /* ... */ }
	
	virtual void DetachModule() { /* ... */ }
};

/**
 * A default minimal implementation of a module that does nothing at mount and detach.
 */
class CDefaultModuleImpl : public IModuleInterface
{ /* ... */ };
