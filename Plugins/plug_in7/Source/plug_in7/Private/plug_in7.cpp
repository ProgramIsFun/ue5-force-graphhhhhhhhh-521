// Copyright Epic Games, Inc. All Rights Reserved.

#include "plug_in7.h"

#define LOCTEXT_NAMESPACE "Fplug_in7Module"

void Fplug_in7Module::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void Fplug_in7Module::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(Fplug_in7Module, plug_in7)