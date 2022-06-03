#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"
#include "MMOARPGType.h"
#include "MMOARPGGateRobot.h"
#include "MMOARPGDSRobot.h"

class FMMOARPGRobot
{
public:
	FMMOARPGRobot();
	void InitGate(const FString& Host, const int32 Port);
	void InitDS(const FString& Host, const int32 Port);
	void Tick(float DeltaTime);

	void RunRobot();
	FMMOARPGGateRobot& GetGateRobot() { return GateRobot; }
private:
	FMMOARPGGateRobot GateRobot;
	FMMOAPRGDSRobot DSRobot;

	FMMOARPGUserData UserData;// 模拟用户表.
};
