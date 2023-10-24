 // Copyright Epic Games, Inc. All Rights Reserved.

#include "KZGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "H_KZGPlayerAnim.h"
#include "../Enemy.h"
#include "../EnemyFsm.h"
#include "H_EWidget.h"
#include "H_PlayerInfo.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include <Camera/PlayerCameraManager.h>

//////////////////////////////////////////////////////////////////////////
// AKZGCharacter

AKZGCharacter::AKZGCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	bReplicates = true;
}

void AKZGCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	returnSpeed = walkSpeed;

	anim = Cast<UH_KZGPlayerAnim>(GetMesh()->GetAnimInstance());

	EWidget = CreateWidget<UH_EWidget>(GetWorld(), BP_EWidget);
	InfoWidget = CreateWidget<UH_PlayerInfo>(GetWorld(), BP_InfoWidget);
	if (InfoWidget != nullptr)
	{
		InfoWidget->AddToViewport();
	}
	currentStamina = playerStamina;
	CameraLocation = FollowCamera->GetComponentLocation();
	CameraRot = FollowCamera->GetComponentRotation();
}

void AKZGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, returnSpeed, 5 * DeltaTime);

	if (!bIsCrouching && bIsRunning && currentStamina > 5)
	{
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
		curSP += DeltaTime;
		if (curSP > 1)
		{
			currentStamina-= recoveryPoint;
			curSP = 0;
		}
	}
	else if (bIsCrouching && !bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = 150;
		curSP += DeltaTime;
		if (curSP > recoverTime)
		{
			currentStamina += recoveryPoint * 10;
			curSP = 0;
		}
	}
	else if(!bIsCrouching && !bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		curSP += DeltaTime;
		if (curSP > recoverTime)
		{
			currentStamina += recoveryPoint;
			curSP = 0;
		}
	}

	Server_GrabbedWidget();
	
	
	if (bIsgrabbed)
	{
		StartCameraBlending();
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Black, FString::Printf(TEXT("%s"), bIsAttacking ? *FString("true") : *FString("false")));
}

void AKZGCharacter::PlayStepSoundPlaying()
{
	TArray<FOverlapResult> hitInfos;
	if (GetWorld()->OverlapMultiByProfile(hitInfos, GetActorLocation(), FQuat::Identity, FName("StepSound"), FCollisionShape::MakeSphere(stepSoundrad)))
	{
		UE_LOG(LogTemp, Warning, TEXT("hitInfo=%d"), hitInfos.Num());
		for (const FOverlapResult& hitInfo : hitInfos)
		{
			if (AEnemy* hitEnemy = Cast<AEnemy>(hitInfo.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("RecoEnemy"));
				hitEnemy->FSM->Recognition(this);
			}
		}
	}
	DrawDebugSphere(GetWorld(), GetActorLocation(), stepSoundrad, 30, FColor::Green, false, 1.0f);
}

void AKZGCharacter::GrabbedbyZombie(class AEnemy* Enemy)
{
	bIsgrabbed=true;
	bCangrabbed=false;
	GrabbedEnemy=Enemy;
	//UE_LOG(LogTemp, Warning, TEXT("Grabbedzz"));
}

void AKZGCharacter::EscapebyZombie()
{
	bIsgrabbed=false;
	GrabbedEnemy=nullptr;
	if (bIsCrouching) bIsCrouching = false;
	//UE_LOG(LogTemp, Warning, TEXT("Escapezz"));
	FTimerHandle myhandle;
	GetWorldTimerManager().SetTimer(myhandle, this,&AKZGCharacter::GrabbedDelay, grabbedDelayTime, false);
}

void AKZGCharacter::TryEscape()
{
	//UE_LOG(LogTemp, Warning, TEXT("Tryzz"));
	DamagedStamina(1);
	GrabbedEnemy->StaminaDamaged(10);
	if (GrabbedEnemy->Stamina_Cur<=0)
	{
		EscapebyZombie();
	}
}

void AKZGCharacter::GrabbedDelay()
{	//그랩가능상태로 바꾸는함수
	bCangrabbed=true;
	UE_LOG(LogTemp,Warning,TEXT("DelayHochulzz"));
}

void AKZGCharacter::DamagedStamina(int32 value)
{
	if (currentStamina -value>0)
	{
		currentStamina -= value;
	}
	else
	{
		currentStamina = 0;
		//사망처리

	}
}

void AKZGCharacter::Server_GrabbedWidget_Implementation()
{
	//Multicast_GrabbedWidget();
	if (bIsgrabbed)
	{
		if (EWidget != nullptr)
		{
			EWidget->AddToViewport();
		}
	}
	else
	{
		if (EWidget != nullptr)
		{
			EWidget->RemoveFromParent();
		}
	}
}

void AKZGCharacter::StartCameraBlending()
{
	FollowCamera->SetRelativeLocation(CameraMoveLoc);
	//// 이미 블렌딩 중이라면 중복 시작을 방지
	//if (bIsBlending) return;

	if (!bIsgrabbed)
	{
		StartCameraBlendToFirst();
	}
	//// 월드 포인터를 가져와서 블렌딩 시작
	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	World->GetTimerManager().SetTimer(BlendTimerHandle, this, &AKZGCharacter::DoingCameraBlending, 0.05f, false);
	//	bIsBlending = true;
	//}

}

void AKZGCharacter::DoingCameraBlending()
{
	// 현재 시간과 블렌딩의 비율 계산
	float CurrentTime = GetWorld()->GetTimerManager().GetTimerElapsed(BlendTimerHandle);
	float BlendAlpha = FMath::Clamp(CurrentTime / blendTime, 0.0f, 1.0f);

	// 현재 카메라의 위치를 가져옴
	FVector StartLocation = CameraLocation;
	FRotator StartRot = CameraRot;
	// 블렌딩된 위치 계산
	FVector BlendedLocation = FMath::Lerp(StartLocation, CameraMoveLoc, BlendAlpha);
	FRotator BlendedRotation = FMath::Lerp(StartRot, CameraMoveRot, BlendAlpha);

	// 카메라의 위치를 설정
	FollowCamera->SetRelativeLocationAndRotation(BlendedLocation, BlendedRotation);

	if (BlendAlpha >= 0.9f)
	{
		// 블렌딩이 완료되면 타이머를 정지하고 블렌딩 중인 상태를 해제
		GetWorld()->GetTimerManager().ClearTimer(BlendTimerHandle);
		bIsBlending = false;

		GetWorld()->GetTimerManager().SetTimer(BlendTimerHandle, this, &AKZGCharacter::StartCameraBlendToFirst, 0.05f, false);

	}
}



void AKZGCharacter::StartCameraBlendToFirst()
{
	FollowCamera->SetRelativeLocation(CameraLocation);
	//// 이미 블렌딩 중이라면 중복 시작을 방지
	//if (bIsBlending) return;


	//// 월드 포인터를 가져와서 블렌딩 시작
	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	World->GetTimerManager().SetTimer(BlendTimerHandle, this, &AKZGCharacter::DoCameraBlendToFirst, 0.05f, false);
	//	bIsBlending = true;
	//}
}

void AKZGCharacter::DoCameraBlendToFirst()
{
	// 현재 시간과 블렌딩의 비율 계산
	float CurrentTime = GetWorld()->GetTimerManager().GetTimerElapsed(BlendTimerHandle);
	float BlendAlpha = FMath::Clamp(CurrentTime / blendTime, 0.0f, 1.0f);

	// 현재 카메라의 위치를 가져옴
	FVector StartLocation = CameraMoveLoc;
	FRotator StartRot = CameraMoveRot;
	// 블렌딩된 위치 계산
	FVector BlendedLocation = FMath::Lerp(StartLocation, CameraLocation, BlendAlpha);
	FRotator BlendedRotation = FMath::Lerp(StartRot, CameraRot, BlendAlpha);

	// 카메라의 위치를 설정
	FollowCamera->SetRelativeLocationAndRotation(BlendedLocation, BlendedRotation);

	if (BlendAlpha >= 0.9f)
	{
		// 블렌딩이 완료되면 타이머를 정지하고 블렌딩 중인 상태를 해제
		GetWorld()->GetTimerManager().ClearTimer(BlendTimerHandle);
		bIsBlending = false;
	}
}

//void AKZGCharacter::Multicast_GrabbedWidget_Implementation()
//{
//	if (bIsgrabbed)
//	{
//		if (EWidget != nullptr)
//		{
//			EWidget->AddToViewport();
//		}
//	}
//	else
//	{
//		if (EWidget != nullptr)
//		{
//			EWidget->RemoveFromParent();
//		}
//	}
//}

void AKZGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AKZGCharacter::JumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKZGCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKZGCharacter::Look);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AKZGCharacter::Server_InputRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AKZGCharacter::Server_InputRun);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AKZGCharacter::Server_CrouchInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AKZGCharacter::Server_CrouchInput);
		

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKZGCharacter::Server_AttackInput);

		EnhancedInputComponent->BindAction(InterAction, ETriggerEvent::Triggered, this, &AKZGCharacter::Server_InteractionInput);
	}
}

void AKZGCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if(bIsgrabbed) return;
		if(bIsAttacking) return;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKZGCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AKZGCharacter::Server_InputRun_Implementation()
{
	//auto movement = GetCharacterMovement();
	// 만약 현재 달리기 상태라면 (released)
	//GEngine->AddOnScreenDebugMessage(-1, 0.01, FColor::Black, FString::Printf(TEXT("Run")));
	if (bIsRunning) bIsRunning = false;
	else bIsRunning = true;
}

void AKZGCharacter::Server_CrouchInput_Implementation()
{
	Multicast_CrouchInput();
}

void AKZGCharacter::Multicast_CrouchInput_Implementation()
{
	if (bIsgrabbed) return;
	if (bIsAttacking) return;
	if (bIsCrouching) bIsCrouching = false;
	else bIsCrouching = true;
}

void AKZGCharacter::Server_AttackInput_Implementation()
{
	Multicast_AttackInput();
}

void AKZGCharacter::Multicast_AttackInput_Implementation()
{
	int32 attackNum = FMath::RandRange(1, 100);
	if (!bIsAttacking && !bIsgrabbed) {
		//if (attackNum <= 100) anim->PlayAttackAnimation1();
		if (attackNum > 50) anim->PlayAttackAnimation2();
		else if (attackNum <= 50) anim->PlayAttackAnimation3();
	}
	bIsAttacking = true;
}

void AKZGCharacter::Server_InteractionInput_Implementation()
{
	Multicast_InteractionUnput();
}

void AKZGCharacter::Multicast_InteractionUnput_Implementation()
{
	if (bIsgrabbed)
	{
		TryEscape();
	}
	else
	{

	}
}

//void AKZGCharacter::InputRun()
//{
//	//auto movement = GetCharacterMovement();
//	// 만약 현재 달리기 상태라면 (released)
//	//GEngine->AddOnScreenDebugMessage(-1, 0.01, FColor::Black, FString::Printf(TEXT("Run")));
//	if (bIsRunning) bIsRunning = false;
//	else bIsRunning = true;
//}

//void AKZGCharacter::CrouchInput()
//{
//	if (bIsgrabbed) return;
//	if (bIsAttacking) return;
//	if(bIsCrouching) bIsCrouching = false;
//	else bIsCrouching = true;
//}

//void AKZGCharacter::AttackInput()
//{	
//	
//	int32 attackNum = FMath::RandRange(1, 100);
//	if (!bIsAttacking && !bIsgrabbed) {
//		if (attackNum <= 33) anim->PlayAttackAnimation1();
//		else if (attackNum > 33 && attackNum <= 66) anim->PlayAttackAnimation2();
//		else if (attackNum > 66) anim->PlayAttackAnimation3();
//	}
//	bIsAttacking = true;
//
//	
//}
//void AKZGCharacter::InteractionInput() 
//{
//	if (bIsgrabbed)
//	{
//		TryEscape();
//	}
//	else
//	{
//		
//	}
//}

void AKZGCharacter::JumpInput()
{
	if (bIsgrabbed) return;
	if (bIsAttacking) return;
	Jump();
}


void AKZGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKZGCharacter, curSP); 
	DOREPLIFETIME(AKZGCharacter, recoverTime);
	DOREPLIFETIME(AKZGCharacter, currentStamina);
	DOREPLIFETIME(AKZGCharacter, playerStamina);
	DOREPLIFETIME(AKZGCharacter, bIsCrouching);
	DOREPLIFETIME(AKZGCharacter, bIsRunning);
	DOREPLIFETIME(AKZGCharacter, bOnDamaged);
	DOREPLIFETIME(AKZGCharacter, bIsAttacking);
	DOREPLIFETIME(AKZGCharacter, bIsgrabbed);
	//DOREPLIFETIME(AKZGCharacter, EWidget);

}
