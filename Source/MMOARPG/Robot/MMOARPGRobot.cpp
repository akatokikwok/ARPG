﻿#include "MMOARPGRobot.h"

void FMMOARPGRobot::InitGate(const FString& Host, const int32 Port)
{
	GateRobot.Init(Host, Port);
}

void FMMOARPGRobot::InitDS(const FString& Host, const int32 Port)
{
	DSRobot.Init(Host, Port);
}

void FMMOARPGRobot::Tick(float DeltaTime)
{
	GateRobot.Tick(DeltaTime);
	DSRobot.Tick(DeltaTime);
}