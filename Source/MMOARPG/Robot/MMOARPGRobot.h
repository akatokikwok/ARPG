#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"
#include "MMOARPGType.h"
#include "MMOARPGGateRobot.h"
#include "MMOARPGDSRobot.h"

class FMMOARPGRobot
{
public:
	void InitGate(const FString& Host, const int32 Port);
	void InitDS(const FString& Host, const int32 Port);
	virtual void Tick(float DeltaTime);
private:
	FMMOARPGGateRobot GateRobot;
	FMMOAPRGDSRobot DSRobot;

	FMMOARPGUserData UserData;// 模拟用户表.
};
