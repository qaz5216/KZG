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
#include <Components/BoxComponent.h>
#include "H_FoodActor.h"
#include <Components/SpotLightComponent.h>

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

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComp->SetupAttachment(GetCapsuleComponent());
	boxComp->SetRelativeLocation(FVector(110.000000, 0.000000, 0.000000));
	boxComp->SetBoxExtent(FVector(200));
	boxComp->SetCollisionProfileName(TEXT("Weapon"));
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; 
	CameraBoom->bUsePawnControlRotation = true; 
	CameraBoom->SetRelativeLocation(FVector(0.000000, 0.000000, 50.000000));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	GrabbedCam = CreateDefaultSubobject<UCameraComponent>(TEXT("GrabbedCam"));
	GrabbedCam->SetupAttachment(GetCapsuleComponent());
	GrabbedCam->SetRelativeLocationAndRotation(FVector(-65.832497, 0.000000, 116.922268), FRotator(-20.000000, 0.000000, 0.000000));

	CamFollowComp = CreateDefaultSubobject<USceneComponent>(TEXT("Cam Follow Comp"));
	CamFollowComp->SetupAttachment(GetCapsuleComponent());
	CamFollowComp->SetRelativeLocation(FVector(-130.000000, 200.000000, -10.000000));

	SeeScene = CreateDefaultSubobject<USceneComponent>(TEXT("SeeScene"));
	SeeScene->SetupAttachment(GetMesh());
	SeeScene->SetRelativeLocation(FVector(0,0,50));

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

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AKZGCharacter::OnComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AKZGCharacter::OnComponentBeginOverlapFood);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AKZGCharacter::OnComponentEndOverlap);

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
	curHungerP = maxHungerP;
	CameraLocation = FollowCamera->GetComponentLocation();
	CameraRot = FollowCamera->GetComponentRotation();
	camArmLen = CameraBoom->TargetArmLength;


}

void AKZGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, returnSpeed, 5 * DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("grab: %s"), bIsgrabbed ? *FString("true") : *FString("false")));
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("attack: %s"), bIsAttacking ? *FString("true") : *FString("false")));
	if(currentStamina > playerStamina) currentStamina = playerStamina;
	if(playerStamina > maxsize) playerStamina = maxsize;
	if(maxsize < 10) maxsize = 10;

	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("%d"), playerStamina));
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("%d"), curHungerP));
	if (!bIsCrouching && bIsRunning && currentStamina > 5)
	{
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
		curSP += DeltaTime;
		if (curSP > 1)
		{
			currentStamina -= recoveryPoint;
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
	//Server_ChangeView();
	
	curHungtime += DeltaTime;
	if (curHungtime > 1) 
	{
		curHungerP--;
		curHungtime = 0;
	}
	
	if (bIsAttacking)
	{
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, 100, 0.2f);
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, 200, 0.2f);
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
	//DrawDebugSphere(GetWorld(), GetActorLocation(), stepSoundrad, 30, FColor::Green, false, 1.0f);
}

void AKZGCharacter::GrabbedbyZombie(class AEnemy* Enemy)
{
	bIsgrabbed=true;
	bCangrabbed=false;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZGrabbedBase);
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
	Multicast_GrabbedWidget();
	
}

void AKZGCharacter::Multicast_GrabbedWidget_Implementation()
{
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

void AKZGCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Zombie = Cast<AEnemy>(OtherActor);
	Zombie->Damaged(damagePower);
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Warning, TEXT("Collision OFFzz"));

	
}

void AKZGCharacter::OnComponentBeginOverlapFood(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AH_FoodActor* food = Cast<AH_FoodActor>(OtherActor))
	{

	}
}	

void AKZGCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapping = false;

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

		EnhancedInputComponent->BindAction(InterAction, ETriggerEvent::Started, this, &AKZGCharacter::Server_InteractionInput);
		EnhancedInputComponent->BindAction(InterAction, ETriggerEvent::Completed, this, &AKZGCharacter::Server_InteractionInputEnd);

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
	if(bIsAttacking) return;
	if(bIsFinalAttackEnded) return;
	if (!bIsgrabbed) {
		//if (attackNum <= 100) anim->PlayAttackAnimation1();
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZHitBase);
		UGameplayStatics::PlaySound2D(this, batHitSound, 0.4f);
		//UE_LOG(LogTemp, Warning, TEXT("Collision ONzz"));
		boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		if (attackNum > 50) anim->PlayAttackAnimation2();
		else if (attackNum <= 50) anim->PlayAttackAnimation3();
		bIsAttacking = true;
	}
}

void AKZGCharacter::Server_InteractionInput_Implementation()
{
	Multicast_InteractionUnput();
}

void AKZGCharacter::Multicast_InteractionUnput_Implementation()
{
	if(bIsInteractionInput) return;
	if (bIsgrabbed)
	{
		bIsInteractionInput = true;
		TryEscape();
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZGrabbedBase);
		anim->playOffAnimation();
	}
	else
	{
		
		FVector MeLoc = GetActorLocation();
		FVector startloc = MeLoc;
		startloc.Z += 50.0f;//눈위치
		FVector endloc = MeLoc+GetActorForwardVector()*500;
		endloc.Z += 50.0f;//플레이어 위치보정
		FHitResult hitResult;
		FCollisionQueryParams collisionParams;
		collisionParams.AddIgnoredActor(this);
		//UE_LOG(LogTemp, Warning, TEXT("4"));
		//DrawDebugLine(GetWorld(), startloc, endloc, FColor::Red, false, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(hitResult, startloc, endloc, ECC_Visibility, collisionParams))
		{
			//UE_LOG(LogTemp, Warning, TEXT("5"));
			if (hitResult.GetActor())
			{
				//UE_LOG(LogTemp, Warning, TEXT("6"));
				if (AEnemy* hitEnemy = Cast<AEnemy>(hitResult.GetActor()))
				{
					if(hitEnemy->FSM->mState==EEnemyState::Die) return;
					bIsInteractionInput = true;
					auto* hitActor = hitResult.GetActor();
					//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hit Actor Name:%s"), *hitActor->GetName()));
					if (hitEnemy->isGroggy)
					{
						anim->finalAttackAnimation3();
						hitEnemy->FSM->ChangeToDieState();
						GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZGrabbedBase);
					}
				}
				//맞은액터 확인용
			}
		}
	}
}

void AKZGCharacter::Server_InteractionInputEnd_Implementation()
{
	Multicast_InteractionUnputEnd();
}

void AKZGCharacter::Multicast_InteractionUnputEnd_Implementation()
{
	bIsInteractionInput = false;
}

void AKZGCharacter::Server_ChangeView_Implementation()
{
	Multicast_ChangeView();

}

void AKZGCharacter::Multicast_ChangeView_Implementation()
{
	if (bIsgrabbed) 
	{
		GrabbedCam->SetActive(true);
		FollowCamera->SetActive(false);
	}
	else
	{
		FollowCamera->SetActive(true);
		GrabbedCam->SetActive(false);
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
	DOREPLIFETIME(AKZGCharacter, bIsInteractionInput);
	DOREPLIFETIME(AKZGCharacter, maxsize);
	//DOREPLIFETIME(AKZGCharacter, EWidget);

}
