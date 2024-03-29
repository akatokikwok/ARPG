﻿#include "MMOARPGRobot.h"

FMMOARPGRobot::FMMOARPGRobot()
{

}

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

void FMMOARPGRobot::InitUser(int32 InUserID, int32 InSlotPos)
{
	GateRobot.UserID = InUserID;
	GateRobot.SlotID = InSlotPos;
}

void FMMOARPGRobot::RunRobot()
{
// 	GateRobot.Run_LoginToDSServerRequests();
}
