// Copyright Epic Games, Inc. All Rights Reserved.
#include "MMOARPGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../../Component/FlyComponent.h"
#include "../../Component/SwimmingComponent.h"
#include "../../Component/ClimbingComponent.h"
#include "../../Component/FightComponent.h"
#include "../Abilities/MMOARPGGameplayAbility.h"
#include "ThreadManage.h"
#include "../MMOARPGGameMode.h"
#include "../MMOARPGPlayerState.h"
#include "../MMOARPGPlayerController.h"
#include "MMOARPGTagList.h"

extern void NameToEGamePlayTags0s(const FName& InName, TArray<FName>& OutName);
extern FName EGamePlayTags0sToName(const TArray<FName>& InName);
// extern void AnalysisArrayNameToGamePlayTags(const TArray<FName>& InNames, TArray<FName>& OutNames);
extern void AnalysisGamePlayTagsToArrayName(const TArray<FName>& InNames, TArray<FName>& OutNames);

#define LOCTEXT_NAMESPACE "AMMOARPGCharacter"

/** 按键绑定. */
void AMMOARPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("MouseClick", IE_Pressed, this, &AMMOARPGCharacter::MouseLeftClick);
	PlayerInputComponent->BindAction("MouseRightClick", IE_Pressed, this, &AMMOARPGCharacter::MouseRightClick);
	PlayerInputComponent->BindAction("MouseClick", IE_Released, this, &AMMOARPGCharacter::MouseLeftClickReleased);
	PlayerInputComponent->BindAction("MouseRightClick", IE_Released, this, &AMMOARPGCharacter::MouseRightClickReleased);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMMOARPGCharacter::Sprint);// 冲刺


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMMOARPGCharacter::CharacterJump);// 跳
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMMOARPGCharacter::CharacterStopJumping);// 跳, 释放.

	PlayerInputComponent->BindAction("SwitchFight", IE_Pressed, this, &AMMOARPGCharacter::SwitchFight);// 切换战斗姿势.
	PlayerInputComponent->BindAction("ActionSwitching", IE_Pressed, this, &AMMOARPGCharacter::ActionSwitching);// 各种姿态系统的内部切换(如站立切飞行,游泳切潜泳)

	PlayerInputComponent->BindAction("Fast", IE_Pressed, this, &AMMOARPGCharacter::Fast);// 急速执行某运动动作.
	PlayerInputComponent->BindAction("Fast", IE_Released, this, &AMMOARPGCharacter::FastReleased);// 解除急速执行某运动动作.

	PlayerInputComponent->BindAction("SlowDown", IE_Pressed, this, &AMMOARPGCharacter::SlowDown);
	PlayerInputComponent->BindAction("SlowDown", IE_Released, this, &AMMOARPGCharacter::SlowDownReleased);

	PlayerInputComponent->BindAction("DodgeLeft", IE_Pressed, this, &AMMOARPGCharacter::DodgeLeft);
	PlayerInputComponent->BindAction("DodgeRight", IE_Pressed, this, &AMMOARPGCharacter::DodgeRight);

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

	/** 仅在客户端主机上执行执行的逻辑. */
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
		// 这部分逻辑放弃,改用PlayerState里的Tick检测人物运行阶段.

// 		GThread::Get()->GetCoroutines().BindLambda(0.04f, [&]() ->void {
// 			GetCharacterDataRequests();// 在客户端向CS发送属性集request
// 			});
	}
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
	if (ActionState == ECharacterActionState::CLIMB_STATE) {/* 进入攀岩姿态. */
		// 在攀岩姿态下是独一套键盘输入.

		GetClimbingComponent()->ClimbingMoveRightAxis(Value);
	}
	else if ((Controller != nullptr) && (Value != 0.0f)) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/** 抽刀收刀的切换逻辑; 变量bFight在DS被修改之后发生的逻辑; 可供OnRep_FightChanged调用. */
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

/** 攀爬跳姿势的切换逻辑. */
void AMMOARPGCharacter::ClimbingMontageChanged(EClimbingMontageState InJumpState)
{
	if (FCharacterAnimTable* InAnimTable_row = AMMOARPGCharacterBase::GetAnimTable()) {
		if (InAnimTable_row->ClimbingMontage != nullptr) {
			PlayAnimMontage(InAnimTable_row->ClimbingMontage,
				1.f,// 播放倍率
				*FString::FromInt((int32)InJumpState)// 播蒙太奇的哪一个sectionname?
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
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking) {
		ResetActionState(ECharacterActionState::FIGHT_STATE);// 强制刷新到战斗姿态. 若和新姿态相同则还原为normal.

		// 本地本机本客户端先执行一次动画逻辑.
		// 本机客户端动作会第一时间优先执行.
		FightChanged();

		// 客户端发送命令通知一下DS刷新ActionState; 除本机外的其他客户端或模拟对象收到后会执行OnRep_ActionStateChanged.
		AMMOARPGCharacterBase::SwitchActionStateOnServer(ActionState);

		// 刷新最后姿态
		LastActionState = ActionState;
	}
}

void AMMOARPGCharacter::MoveForward(float Value)
{
	// 	ActionState = ECharacterActionState::CLIMB_STATE;// 测试用代码.

		//if ((Controller != nullptr) && (Value != 0.0f)) {
	if (Controller != nullptr) {

		// 按姿态重新划分逻辑.
		if (ActionState == ECharacterActionState::FLIGHT_STATE) {
			GetFlyComponent()->FlyForwardAxis(Value);// 计算飞行组件的轴向.
		}
		else if (ActionState == ECharacterActionState::SWIMMING_STATE) {
			GetSwimmingComponent()->SwimForwardAxis(Value);
		}
		else if (ActionState == ECharacterActionState::CLIMB_STATE) {
			GetClimbingComponent()->ClimbingForwardAxis(Value);
		}
		else if (Value != 0.0f) {/* 存在按键输入时.*/

			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void AMMOARPGCharacter::ActionSwitching_Implementation()
{
	// 发指令给服务器; 在服务器上做MulticastFly()里的一些具体逻辑.
	MulticastActionSwitching();

}

void AMMOARPGCharacter::MulticastActionSwitching_Implementation()
{
	/* 在服务器上做这些逻辑,做完后再广播, 通知到客户端. 使用NetMulticast宏. */

	if (UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent())) {
		if ((CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Walking || CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Flying)
			&& ActionState != ECharacterActionState::FIGHT_STATE) // 后面优化,战斗姿态中禁用飞行.
		{
			ResetActionState(ECharacterActionState::FLIGHT_STATE);// 强制刷为飞行姿态,若已飞行则切回normal
			GetFlyComponent()->ResetFly();// 手动使用一套用于飞行姿态下的组件设置.
		}
		else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming) {
			GetSwimmingComponent()->GoUnderWater();// 潜入水下.
		}
		else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Custom) {
			if (!GetClimbingComponent()->IsDropClimbingState()) {/*复查是不是攀岩-坠落.防止与UE原生的falling重复, 做一层保护*/
				GetClimbingComponent()->ReleaseClimbing();// 还原一套walking设置
				GetClimbingComponent()->DropClimbingState();// 攀岩状态切换为坠落.
				ClimbingMontageChanged(EClimbingMontageState::CLIMBING_DROP_RM);// 播坠地蒙太奇section

				// 手动坠落的时候 反方向蹬腿给力.
				// 暂定给1000的数值, 它会影响给力的大小,进而影响到
				// UClimbingComponent.PhysClimbong里的速度大小. 
				FVector Dir = -GetActorForwardVector();
				GetClimbingComponent()->LaunchCharacter(Dir * 1000.f);
			}
		}
		else if (ActionState == ECharacterActionState::FIGHT_STATE) {

		}
	}
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
	else if (ActionState == ECharacterActionState::NORMAL_STATE) {
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
}

void AMMOARPGCharacter::FastReleased_Implementation()
{
	/* 广播 停止加速*/
	MulticastFastReleased();
}

void AMMOARPGCharacter::MulticastFastReleased_Implementation()
{
	if (ActionState == ECharacterActionState::SWIMMING_STATE) {
		GetSwimmingComponent()->ResetFastSwiming();
	}
	else if (ActionState == ECharacterActionState::NORMAL_STATE) {
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
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

void AMMOARPGCharacter::SlowDown_Implementation()
{
	MulticastSlowDown();
}

void AMMOARPGCharacter::MulticastSlowDown_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 190.f;
}

void AMMOARPGCharacter::SlowDownReleased_Implementation()
{
	MulticastSlowDownReleased();
}

void AMMOARPGCharacter::MulticastSlowDownReleased_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

/** 用于攀爬系统的 跳爬释放 */
void AMMOARPGCharacter::CharacterStopJumping()
{
	StopJumping();// 基类的.
}

/** 用于攀爬系统的 跳爬 */
void AMMOARPGCharacter::CharacterJump()
{
	Jump();// 基类的.

	if (ActionState == ECharacterActionState::CLIMB_STATE) {
		GetClimbingComponent()->Jump();// 执行攀岩组件里的 "大跳逻辑"
	}
}

// RPC服务器 "执行攀岩跳的效果".
// void AMMOARPGCharacter::CharacterJumpToServer_Implementation()
// {
// 	MulticastCharacterJump();
// }

// 广播 "攀岩跳"
// void AMMOARPGCharacter::MulticastCharacterJump_Implementation()
// {
// 
// }

// RPC在服务器, 由客户端向CS发送属性集请求.
void AMMOARPGCharacter::GetCharacterDataRequests_Implementation()
{
	if (GetWorld()) {
		if (AMMOARPGGameMode* MMOARPGGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>()) {
			if (AMMOARPGPlayerState* InPS = GetPlayerState<AMMOARPGPlayerState>()) {
				MMOARPGGameMode->GetCharacterDataRequests(UserID, ID, InPS->GetCA().SlotPosition);// 向CS服务器发送 gas人物属性集请求 
			}
		}
	}
}

// RPC在服务器, 左mouse按下后续
void AMMOARPGCharacter::MouseLeftClick_Implementation()
{
	if (ActionState == ECharacterActionState::FIGHT_STATE) {// 仅在战斗姿态里.
		// 广播COMBO触发器Press()至其他客户端.
		GetFightComponent()->Press();
	}
}

// RPC在服务器, 右mouse按下后续
void AMMOARPGCharacter::MouseRightClick_Implementation()
{
	if (ActionState == ECharacterActionState::FIGHT_STATE || ActionState == ECharacterActionState::NORMAL_STATE) {
		GetFightComponent()->DodgeSkill();
	}
}

// RPC在服务器, 左mouse松开后续
void AMMOARPGCharacter::MouseLeftClickReleased_Implementation()
{
	// 广播COMBO触发器Release()至其他客户端.
	GetFightComponent()->Released();
}

// RPC在服务器, 右mouse松开后续
void AMMOARPGCharacter::MouseRightClickReleased/*_Implementation*/()
{

}

// 按键后冲刺.
void AMMOARPGCharacter::Sprint_Implementation()
{
	if (ActionState == ECharacterActionState::FIGHT_STATE) {
		GetFightComponent()->SprintSkill();
	}
}

/** 覆盖CombatInterface接口, 如若信号值设定2,则复位触发器黑盒. */
void AMMOARPGCharacter::AnimSignal(int32 InSignal)
{
	Super::AnimSignal(InSignal);
	/* 如果在蓝图蒙太奇里配的这个AnimNotify-AnimSignal型的Notify的信号值被配成2.f, 则让连招触发器黑盒重置. */
	if (InSignal == 2) {
		GetFightComponent()->Reset();
	}
}

// 覆写GetTarget接口. 读取敌对目标人物或者怪物
AActor* AMMOARPGCharacter::GetTarget()
{
	// 使用AMMOARPGPlayerController里的读取敌对目标
	if (AMMOARPGPlayerController* InPlayerController = Cast<AMMOARPGPlayerController>(GetController())) {
		return InPlayerController->GetTarget();
	}
	return nullptr;
}

void AMMOARPGCharacter::UpdateLevel(float InLevel)
{
	Super::UpdateLevel(InLevel);
	if (GetWorld()) {
		/** 升级 DS服务器把数据传递到中心服务器 */
		if (AMMOARPGGameMode* MMOARPGGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>()) {
			FMMOARPGCharacterAttribute CharacterAttribute;// 待填充的人物属性信息
			// 1. 处理人物属性字段, 提出AS里的人物所有属性字段,存成MMOAD型
			GetAttribute()->ToMMOARPGCharacterAttribute(CharacterAttribute);

			// 2. 处理角色技能
			TArray<FName> SkillTagsName;
			TArray<FName> ComboAttackTagsName;
			TArray<FName> LimbsTagsName;
			// 使用各自的提取方法, 提取3种标签名群组
			GetSkillTagsName(SkillTagsName);
			GetComboAttackTagsName(ComboAttackTagsName);
			GetLimbsTagsName(LimbsTagsName);
			// 借由服务器方法, 把标签群组 转为位序列
			AnalysisGamePlayTagsToArrayName(SkillTagsName, CharacterAttribute.Skill);// 将Tag组转换为服务器存储的序列
			AnalysisGamePlayTagsToArrayName(ComboAttackTagsName, CharacterAttribute.ComboAttack);// 将Tag组转换为服务器存储的序列
			AnalysisGamePlayTagsToArrayName(LimbsTagsName, CharacterAttribute.Limbs);// 将Tag组转换为服务器存储的序列

			// 3.序列化技能装配
			SerializationSkillAssembly(CharacterAttribute.SkillAssemblyString);

			// 4. 命令GM升人物等级
			MMOARPGGameMode->UpdateLevelRequests(UserID, ID, CharacterAttribute);
		}
	}
}

/** 处理人的血量; 覆写 */
void AMMOARPGCharacter::HandleHealth(AMMOARPGCharacterBase* InstigatorPawn, AActor* DamageCauser, const struct FGameplayTagContainer& InTags, float InNewValue)
{
	Super::HandleHealth(InstigatorPawn, DamageCauser, InTags, InNewValue);

	if (GetWorld()) {
		if (AMMOARPGGameMode* MMOARPGGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>()) {
			// 使用GM去发给服务器命令, 更新获取人物 "血量"属性字段
			MMOARPGGameMode->UpdateAttributeRequests(UserID, ID,
				MMOARPGCharacterAttributeType::ATTRIBUTETYPE_HEALTH,
				GetCharacterHealth());
		}
	}
}

/** 处理人的蓝量; 覆写 */
void AMMOARPGCharacter::HandleMana(const struct FGameplayTagContainer& InTags, float InNewValue)
{
	if (GetWorld()) {
		if (AMMOARPGGameMode* MMOARPGGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>()) {
			// 使用GM去发给服务器命令, 更新获取人物 "蓝量"属性字段
			MMOARPGGameMode->UpdateAttributeRequests(UserID, ID,
				MMOARPGCharacterAttributeType::ATTRIBUTETYPE_MANA,
				GetCharacterMana());
		}
	}
}

// 覆写处理经验值接口
void AMMOARPGCharacter::HandleExp(const struct FGameplayTagContainer& InTags, float InNewValue)
{
	Super::HandleExp(InTags, InNewValue);
	if (GetWorld()) {
		if (AMMOARPGGameMode* InGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>()) {
			InGameMode->UpdateAttributeRequests(UserID, ID,
				MMOARPGCharacterAttributeType::ATTRIBUTETYPE_EMPIRICALVALUE, GetCharacterExp());
		}
	}
}

void AMMOARPGCharacter::CreateResurrectionWindowsClient_Implementation()
{
	if (GetWorld()) {
		if (AMMOARPGPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AMMOARPGPlayerController>()) {
			InPlayerController->CreateWindows(
				LOCTEXT("Resurrection_Button", "Resurrection"),
				LOCTEXT("Resurrection_Content", "When the character dies, it will be revived to the latest generation point after clicking rebirth."),
				(uint8)EPopupMsgType::POPUP_MSG_RESURRECTION);
		}
	}
}

/** 服务端执行技能形式的技能攻击(需指定一个槽号) */
void AMMOARPGCharacter::SKillAttackOnServer_Implementation(int32 InSlot)
{
	if (GetFightComponent()) {
		GetFightComponent()->SKillAttack(InSlot);
	}
}

// 在客户端 更新技能表(SkillPage)-UI外观
void AMMOARPGCharacter::UpdateSkillTableOnClient_Implementation(const TArray<FName>& InSkillTags)
{
	if (AMMOARPGPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AMMOARPGPlayerController>()) {
		InPlayerController->UpdateSkillTableDelegate.ExecuteIfBound(InSkillTags);
	}
}

// 在客户端 更新技能槽节点(横框)-UI外观
void AMMOARPGCharacter::UpdateSkillSlotsOnClient_Implementation(const TArray<FName>& InSkillTags)
{
	if (AMMOARPGPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AMMOARPGPlayerController>()) {
		InPlayerController->UpdateSkillSlotDelegate.ExecuteIfBound(InSkillTags);
	}
}

// 在客户端 向DS请求更新技能节点
void AMMOARPGCharacter::UpdateSkillSlotsOnServer_Implementation()
{
	// to do.
}

#pragma region 技能槽业务可用到的一些接口
/** 从横框到技能页: 移动 */
void AMMOARPGCharacter::SKillSlotMoveToSkillTable_Implementation(int32 InSlot)
{
	if (GetFightComponent()) {
		/* 移除SkillTMap的技能节点并查询是否成功 */
		if (GetFightComponent()->RemoveSkillSlot(InSlot)) {
			// 更新UI-更新技能节点
			GetFightComponent()->UpdateSkillSlots();

			// 向CS发送更新装配技能命令
			this->UpdateSkillAssembly();
		}
	}
}

/** 从横框到技能页: 交换 */
void AMMOARPGCharacter::SKillSlotSwapSkillTable_Implementation(int32 InRemoveSlot, const FName& InSkillName)
{
	if (GetFightComponent()) {
		/* 先移除横框里的技能节点, 并添加新的技能节点*/
		if (GetFightComponent()->RemoveSkillSlot(InRemoveSlot, InSkillName)) {
			// 更新UI-更新技能节点
			GetFightComponent()->UpdateSkillSlots();

			// 向CS发送更新装配技能命令
			this->UpdateSkillAssembly();
		}
	}
}

/** 从技能页到横框: 移动 */
void AMMOARPGCharacter::SKillTableSlotMoveToSkillSlot_Implementation(const FName& InSkillName, int32 InSlot)
{
	if (GetFightComponent()) {
		/* 往横框里添加技能 */
		if (GetFightComponent()->AddSkillSlot(InSlot, InSkillName)) {
			// 更新UI-更新技能节点
			GetFightComponent()->UpdateSkillSlots();

			// 向CS发送更新装配技能命令
			this->UpdateSkillAssembly();
		}
	}
}

/** 从技能页到横框: 交换 */
void AMMOARPGCharacter::SKillTableSlotSwapSkillSlot_Implementation(int32 InRemoveSlot, const FName& InSkillName)
{
	if (GetFightComponent()) {
		/* 先移除横框里的技能节点, 并添加新的技能节点*/
		if (GetFightComponent()->RemoveSkillSlot(InRemoveSlot, InSkillName)) {
			// 更新UI-更新技能节点
			GetFightComponent()->UpdateSkillSlots();

			// 向CS发送更新装配技能命令
			this->UpdateSkillAssembly();
		}
	}
}

/** 横框内, 任意2个技能Slot之间的移动 */
void AMMOARPGCharacter::SKillSlotMoveToNewSlot_Implementation(int32 InASlot, int32 InBSlot)
{
	if (GetFightComponent()) {
		if (GetFightComponent()->MoveSkillSlot(InASlot, InBSlot)) {/** 移动技能并查询是否成功 */
			// 更新UI-更新技能节点
			GetFightComponent()->UpdateSkillSlots();

			// 向CS发送更新装配技能命令
			this->UpdateSkillAssembly();
		}
	}
}

/** 横框内, 任意2个技能内2个技能槽交换 */
void AMMOARPGCharacter::SillSlotSwap_Implementation(int32 InASlot, int32 InBSlot)
{
	if (GetFightComponent()) {
		if (GetFightComponent()->SwapSkillSlot(InASlot, InBSlot)) {/** 交换技能并查询是否成功 */
			// 更新UI-更新技能节点
			GetFightComponent()->UpdateSkillSlots();

			// 向CS发送更新装配技能命令
			this->UpdateSkillAssembly();
		}
	}
}
#pragma endregion 技能槽业务可用到的一些接口

void AMMOARPGCharacter::InitSkill()
{
	if (GetFightComponent()) {
		GetFightComponent()->InitSkill();
	}
}

void AMMOARPGCharacter::DeserializationSkillAssembly(const FString& InString)
{

}

void AMMOARPGCharacter::SerializationSkillAssembly(FString& OutString)
{

}

void AMMOARPGCharacter::UpdateSkillAssembly()
{
	if (AMMOARPGGameMode* MMOARPGGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>()) {
		// 序列化装配技能
		FString SkillAssemblyString;
		SerializationSkillAssembly(SkillAssemblyString);

		// 角色所有形式技能
		TArray<FName> SkillTagsName;
		TArray<FName> ComboAttackTagsName;
		TArray<FName> LimbsTagsName;

		// 让GM去Request CS服务器的数据
		MMOARPGGameMode->UpdateSkillAssembly(UserID, this->ID, SkillAssemblyString);
	}
}

#undef LOCTEXT_NAMESPACE