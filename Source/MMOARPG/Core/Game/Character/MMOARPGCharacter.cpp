// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGCharacter.h"
// #include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AMMOARPGCharacter

AMMOARPGCharacter::AMMOARPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AMMOARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 	// 手动设定LOC, LOC影响舞台人物拉腿之后的站立高度.
	// 	InitKneadingLocation(GetMesh()->GetComponentLocation());
}

// void AMMOARPGCharacter::UpdateKneadingBoby()
// {
// 	if (SlotID != INDEX_NONE) {// 异常保护.
// 
// 		if (AHallPlayerState* InPS = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>()) {
// 
// 			/**
// 			 * 先查PS里是否已有存档,没存档就启用临时存档,用临时存档来更新女人物身材样式.
// 			 */
// 			if (FMMOARPGCharacterAppearance* InCA_exist = InPS->GetCharacterCA(SlotID)) {
// 				UpdateKneadingBoby(*InCA_exist);// 生成指定CA存档的人物身材.
// 			}
// 			else if (InPS->GetCurrentTmpCreateCharacterCA() && InPS->GetCurrentTmpCreateCharacterCA()->SlotPosition == SlotID) {// 槽号的设定在 UUI_CharacterCreatePanel::SpawnCharacter里完成.
// 				FMMOARPGCharacterAppearance* CA_Temp = InPS->GetCurrentTmpCreateCharacterCA();
// 				UpdateKneadingBoby(*CA_Temp);
// 				// 				UpdateKneadingBoby(*InPS->GetCurrentTmpCreateCharacterCA());
// 			}
// 		}
// 	}
// }

// void AMMOARPGCharacter::UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA)
// {
// 	SetLegSize(InCA.LegSize);
// 	SetWaistSize(InCA.WaistSize);
// 	SetArmSize(InCA.ArmSize);
// 
// 	// 需要Mesh也要和我们的骨骼保持一致变化.
// 	SetMeshPostion(GetMesh());
// }

void AMMOARPGCharacter::Print(float InTime, const FString& InString)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, InTime, FColor::Purple, FString::Printf(TEXT("%s"), *InString));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input


void AMMOARPGCharacter::OnResetVR()
{
	// If MMOARPG is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in MMOARPG.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
// 	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMMOARPGCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector InLocation)
{
	Jump();
}

void AMMOARPGCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector InLocation)
{
	StopJumping();
}

void AMMOARPGCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMMOARPGCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMMOARPGCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMMOARPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("SwitchFight", IE_Pressed, this, &AMMOARPGCharacter::SwitchFight);// 切换战斗姿势.
	PlayerInputComponent->BindAction("Fly", IE_Pressed, this, &AMMOARPGCharacter::Fly);// 飞行
	PlayerInputComponent->BindAction("Fast", IE_Pressed, this, &AMMOARPGCharacter::Fast);// 急速执行某运动动作.
	PlayerInputComponent->BindAction("Fast", IE_Released, this, &AMMOARPGCharacter::FastReleased);// 解除急速执行某运动动作.
	PlayerInputComponent->BindAction("DodgeLeft", IE_Pressed, this, &AMMOARPGCharacter::DodgeLeft);
	PlayerInputComponent->BindAction("DodgeRight", IE_Pressed, this, &AMMOARPGCharacter::DodgeRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMMOARPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMMOARPGCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMMOARPGCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMMOARPGCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMMOARPGCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMMOARPGCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMMOARPGCharacter::OnResetVR);
}

void AMMOARPGCharacter::FightChanged()
{
	if (FCharacterAnimTable* InAnimTable_row = AMMOARPGCharacterBase::GetAnimTable()) {
		if (InAnimTable_row->SwitchFightMontage != nullptr) {
			PlayAnimMontage(InAnimTable_row->SwitchFightMontage, // 蒙太奇资产: SwitchFight
				1.f,// 播放倍率
				ActionState == ECharacterActionState::FIGHT_STATE ? TEXT("0") : TEXT("1")// 按切换姿势来切换哪一个section.
			);
		}
	}
}

/** 当字段被DS刷新后,客户端Player做出的反应. */
void AMMOARPGCharacter::OnRep_ActionStateChanged()
{
	// 此处设定为不为不服务器的，诸如只在客户端或在模拟玩家上发生的行为.
	// 它会第二时间,慢于本机客户端.
	// AMMOARPGCharacterBase.GetLifetimeReplicatedProps 方法里条件设定为 只转发至模拟玩家
	if (GetLocalRole() != ROLE_Authority) {

		/* 当前动作状态 或收尾动作都处于战斗姿势.*/
		if (ActionState == ECharacterActionState::FIGHT_STATE || LastActionState == ECharacterActionState::FIGHT_STATE) {
			this->FightChanged();
		}

		/* .*/
		LastActionState = ActionState;// 刷新最后一次动作点状态.
	}
}

void AMMOARPGCharacter::SwitchFight()
{
	ResetActionState(ECharacterActionState::FIGHT_STATE);// 强制刷新到战斗姿态. 若和新姿态相同则还原为normal.

	// 本地本机本客户端先执行一次动画逻辑.
	// 本机客户端动作会第一时间优先执行.
	FightChanged();

	// 客户端发送命令通知一下DS刷新ActionState; 除本机外的其他客户端或模拟对象收到后会执行OnRep_ActionStateChanged.
	AMMOARPGCharacterBase::SwitchActionStateOnServer(ActionState);

	// 刷新最后姿态
	LastActionState = ActionState;
}

void AMMOARPGCharacter::MoveForward(float Value)
{
	//if ((Controller != nullptr) && (Value != 0.0f)) {
	if (Controller != nullptr) {

		// 按姿态重新划分逻辑.
		if (ActionState == ECharacterActionState::FLIGHT_STATE) {
			GetFlyComponent()->FlyForwardAxis(Value);// 计算飞行组件的轴向.
		}
		else if (ActionState == ECharacterActionState::SWIMMING_STATE) {
			GetSwimmingComponent()->SwimForwardAxis(Value);
		}
		else {
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void AMMOARPGCharacter::Fly_Implementation()
{	
	// 发指令给服务器; 在服务器上做MulticastFly()里的一些具体逻辑.
	MulticastFly();

}

void AMMOARPGCharacter::MulticastFly_Implementation()
{
	/* 在服务器上做这些逻辑,做完后再广播, 通知到客户端. 使用NetMulticast宏. */

	ResetActionState(ECharacterActionState::FLIGHT_STATE);// 强制刷为飞行姿态,若已飞行则切回normal
	GetFlyComponent()->ResetFly();// 手动使用一套用于飞行姿态下的组件设置.
}

void AMMOARPGCharacter::Fast_Implementation()
{
	MulticastFast();
}

void AMMOARPGCharacter::MulticastFast_Implementation()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE) {
		GetFlyComponent()->ResetFastFly();// 还原一套加速飞行的组件配置.
	}
	else if (ActionState == ECharacterActionState::SWIMMING_STATE) {
		GetSwimmingComponent()->ResetFastSwiming();// 还原一套用于急速游泳姿态的组件设置.
	}
}

void AMMOARPGCharacter::FastReleased_Implementation()
{
	if (ActionState == ECharacterActionState::SWIMMING_STATE) {
		GetSwimmingComponent()->ResetFastSwiming();
	}
}

void AMMOARPGCharacter::DodgeLeft_Implementation()
{
	MulticastDodgeLeft();
}

void AMMOARPGCharacter::MulticastDodgeLeft_Implementation()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE) {
		GetFlyComponent()->ResetDodgeFly(EDodgeFly::DODGE_LEFT);
	}
}

void AMMOARPGCharacter::DodgeRight_Implementation()
{
	MulticastDodgeRight();
}

void AMMOARPGCharacter::MulticastDodgeRight_Implementation()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE) {
		GetFlyComponent()->ResetDodgeFly(EDodgeFly::DODGE_RIGHT);
	}
}
