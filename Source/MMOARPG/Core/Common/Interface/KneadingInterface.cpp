// Fill out your copyright notice in the Description page of Project Settings.


#include "KneadingInterface.h"

IKneadingInterface::IKneadingInterface()
	: LegSize(0.f)
	, WaistSize(0.f)
	, ArmSize(0.f)
{

}

// Add default functionality here for any IKneadingInterface functions that are not pure virtual.
void IKneadingInterface::SetLegSize(float InLegSize)
{
	LegSize = InLegSize;
}

void IKneadingInterface::SetWaistSize(float InWaistSize)
{
	WaistSize = InWaistSize;
}

void IKneadingInterface::SetArmSize(float InArmSize)
{
	ArmSize = InArmSize;
}

float IKneadingInterface::GetLegSize()
{
	return LegSize;
}
float IKneadingInterface::GetWaistSize()
{
	return WaistSize;
}
float IKneadingInterface::GetArmSize()
{
	return ArmSize;
}

void IKneadingInterface::SetMeshPostion(USceneComponent* InMesh)
{
	if (InMesh) {
		FVector ComponentLocation = Location;
		ComponentLocation.Z += LegSize;// 拿到人物的位置垂直方向上 增长, 就是拉腿.

		InMesh->SetRelativeLocation(ComponentLocation);// 重设舞台人物的 玩家高度.
	}
}

void IKneadingInterface::InitKneadingLocation(const FVector& InLocation)
{
	Location = InLocation;
}
