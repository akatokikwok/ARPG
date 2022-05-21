#include "Core/SimpleBrowse.h"

namespace SimpleActorAction
{
	FSimpleBrowse::FSimpleBrowse(APlayerController* InController, AActor* InTarget)
		: bRotateCharacter(false)
		, Controller(InController)
		, Target(InTarget)
	{

	}

	FSimpleBrowse::FSimpleBrowse()
		: bRotateCharacter(false)
		, Controller(NULL)
		, Target(NULL)
	{

	}

	void FSimpleBrowse::Tick(float DeltaTime)
	{
		if (Controller && Target.IsValid()) {// 弱指针判空保护
			/**
			 * 旋转操作的算法.
			 */
			if (bRotateCharacter == true) {// 是否启用旋转.
				FRotator CurrentRot = Target->GetActorRotation();

				/* 最新2D坐标减去旧坐标算取带方向的旋转速度.*/
				FVector2D NewMousePos;
				Controller->GetMousePosition(NewMousePos.X, NewMousePos.Y);
				float RotSpeed = (NewMousePos.X - MousePos_last.X) * -1.f;

				/* 重设朝向.*/
				CurrentRot.Yaw += RotSpeed * 1.f;
				Target->SetActorRotation(CurrentRot);

				/* 刷新旧坐标至最新.*/
				MousePos_last = NewMousePos;
			}
			else {/*未启用旋转, 则时时刻刻刷新最近一次鼠标坐标.*/
				Controller->GetMousePosition(MousePos_last.X, MousePos_last.Y);
			}

		}
	}

	TStatId FSimpleBrowse::GetStatId() const
	{
		return TStatId();
	}


	void FSimpleBrowse::Register(APlayerController* InController, AActor* InTarget)
	{
		Controller = InController;
		Target = InTarget;
	}

	void FSimpleBrowse::OpenBrowsing()
	{
		bRotateCharacter = true;
	}

	void FSimpleBrowse::EndBrowsing()
	{
		bRotateCharacter = false;
	}

	void FSimpleBrowse::ResetTarget(AActor* InNewTarget)
	{
		Target = InNewTarget;
	}

}