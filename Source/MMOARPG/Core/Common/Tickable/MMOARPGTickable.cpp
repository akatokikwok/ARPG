#include "MMOARPGTickable.h"
#include "ThreadManage.h"

FMMOARPGTickable* FMMOARPGTickable::MMOARPGTickableSinglePtr = nullptr;

FMMOARPGTickable* FMMOARPGTickable::Get()
{
	if (!MMOARPGTickableSinglePtr) {
		MMOARPGTickableSinglePtr = new FMMOARPGTickable();
	}

	return MMOARPGTickableSinglePtr;
}

void FMMOARPGTickable::Destroy()
{
	if (MMOARPGTickableSinglePtr) {
		delete MMOARPGTickableSinglePtr;
	}

	MMOARPGTickableSinglePtr = nullptr;
}

// 本类的tick使用协程形式的tick
void FMMOARPGTickable::Tick(float DeltaTime)
{
	GThread::Get()->Tick(DeltaTime);
}

TStatId FMMOARPGTickable::GetStatId() const
{
	return TStatId();
}