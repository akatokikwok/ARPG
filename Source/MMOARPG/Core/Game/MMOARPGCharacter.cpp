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
	// 手动设定LOC, LOC影响舞台人物拉腿之后的站立高度.
	InitKneadingLocation(GetMesh()->GetComponentLocation());
}

void AMMOARPGCharacter::UpdateKneadingBoby()
{
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
}

void AMMOARPGCharacter::UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);

	// 需要Mesh也要和我们的骨骼保持一致变化.
	SetMeshPostion(GetMesh());
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMMOARPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
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

void AMMOARPGCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMMOARPGCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
