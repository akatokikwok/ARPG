#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"
#include "MMOARPGType.h"

class FMMOAPRGDSRobot
{
public:
	FMMOAPRGDSRobot();
	void Init(const FString& Host, const int32 Port);
	virtual void Tick(float DeltaTime);

	FSimpleNetManage* DSClient;// 模拟DS的客户端.
};
