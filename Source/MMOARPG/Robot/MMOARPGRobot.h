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

	// 注册子类网管机器人的用户号和槽位号
	void InitUser(int32 InUserID, int32 InSlotPos);

	void RunRobot();
	FMMOARPGGateRobot& GetGateRobot() { return GateRobot; }
private:
	FMMOARPGGateRobot GateRobot;
	FMMOAPRGDSRobot DSRobot;

	FMMOARPGUserData UserData;// 模拟用户表.
};
