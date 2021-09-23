// Copyright Epic Games, Inc. All Rights Reserved.


#include "MMOARPGdbServer.h"

#include "RequiredProgramMainCPPInclude.h"

DEFINE_LOG_CATEGORY_STATIC(LogMMOARPGdbServer, Log, All);

IMPLEMENT_APPLICATION(MMOARPGdbServer, "MMOARPGdbServer");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	GEngineLoop.PreInit(ArgC, ArgV);
	UE_LOG(LogMMOARPGdbServer, Display, TEXT("Hello World"));
	FEngineLoop::AppExit();
	return 0;
}
