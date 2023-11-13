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
#include "H_AttackWeapons.h"

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

	batMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("batMesh"));
	batMesh->SetupAttachment(GetMesh());
	// Get the socket name
	FName WeaponSocketName = FName(TEXT("WeaoponSocket"));
	// Attach batMesh to the WeaoponSocket
	batMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	 
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempBat (TEXT("/Script/Engine.StaticMesh'/Game/Props_MeleeWeapons/Meshes/baseballBat.baseballBat'"));

	if (TempBat.Succeeded())
	{
		batMesh->SetStaticMesh(TempBat.Object);
		batMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		batMesh->SetVisibility(false);
		batMesh->SetRelativeLocationAndRotation(FVector(-13.419591, -2.414909, 9.095527), FRotator(12.700006, -15.579394, -51.744371));

	}

	axeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxeMesh"));
	axeMesh->SetupAttachment(GetMesh());
	// Attach batMesh to the WeaoponSocket
	axeMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempAxe(TEXT("/Script/Engine.StaticMesh'/Game/Props_MeleeWeapons/Meshes/fireAxe.fireAxe'"));

	if (TempAxe.Succeeded())
	{
		axeMesh->SetStaticMesh(TempAxe.Object);
		axeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		axeMesh->SetVisibility(false);
		axeMesh->SetRelativeLocationAndRotation(FVector(-11.144902, 1.984119, 4.615342), FRotator(12.700006, -15.579394, -51.744371));
	}

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComp->SetupAttachment(GetCapsuleComponent());
	boxComp->SetRelativeLocation(FVector(110.000000, 0.000000, 0.000000));
	boxComp->SetBoxExtent(FVector(100));
	boxComp->SetRelativeScale3D(FVector(-0.500000, 0.500000, 1.000000));
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

	static ConstructorHelpers::FClassFinder<UH_PlayerInfo> TempInfo(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/CYS/UI_CYS_PlayerInfo2.UI_CYS_PlayerInfo2_C'"));

	if (TempInfo.Succeeded())
	{
		// Assign the found class to a variable
		BP_InfoWidget = TempInfo.Class;
	}
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

	if (currentStamina <= 0)
	{
		if (!bIsDead)
		{
			if (bIsgrabbed)
			{
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				anim->PlayDeathGrabMontage();
				if (GrabbedCam->IsActive())
				{
					FollowCamera->SetActive(true);
					GrabbedCam->SetActive(false);
				}
				CameraBoom->TargetArmLength = 1000.0f;
				FollowCamera->SetRelativeLocation(FVector(0, 0, -100));
				bIsgrabbed = false;
				bIsDead = true;
			}
			else
			{
				Server_PlayerDeath();
				bIsDead = true;
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Weapon HP : %d"), curWeaponHP));
	if (curWeaponHP <= 0)
	{
		if(axeMesh->IsVisible()) axeMesh->SetVisibility(false);
		else if(batMesh->IsVisible()) batMesh->SetVisibility(false);
	}
	if(currentStamina > playerStamina) currentStamina = playerStamina;
	if(playerStamina > maxsize) playerStamina = maxsize;
	if(maxsize <= 50) maxsize = 60;
	if(curHungerP <= 0) curHungerP = 0;
	if(curHungerP > maxHungerP) curHungerP = maxHungerP;

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
	if (bIsgrabbed)
	{
		GrabbedCam->SetActive(true);
		FollowCamera->SetActive(false);
	}
	else if(!bIsgrabbed)
	{
		FollowCamera->SetActive(true);
		GrabbedCam->SetActive(false);
	}
	
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

	//암살가능범위
	TArray<FOverlapResult> hitInfos;
	FVector MeLoc = GetActorLocation();
	if (GetWorld()->OverlapMultiByProfile(hitInfos, MeLoc, FQuat::Identity, FName("StepSound"), FCollisionShape::MakeSphere(300)))
	{
		bool findEnemy = false;
		TArray<AEnemy*> hitEnemys;
		TArray<double> WhatEnemy;
		//UE_LOG(LogTemp,Warning,TEXT("1"));
		for (const FOverlapResult& hitInfo : hitInfos)
		{
			//UE_LOG(LogTemp, Warning, TEXT("2"));
			if (AEnemy* hitEnemy = Cast<AEnemy>(hitInfo.GetActor()))//이거 되긴하냐..?
			{	
				//에너미의 뒤인가?
				FVector CheckDir=GetActorLocation()-hitEnemy->GetActorLocation();
				CheckDir.Normalize();
				double DotP = FVector::DotProduct(CheckDir, hitEnemy->GetActorForwardVector());
				if (DotP<0&&hitEnemy->FSM->mState!=EEnemyState::Die)
				{
					hitEnemys.Add(hitEnemy);
					FVector CheckDir2 = hitEnemy->GetActorLocation()- GetActorLocation();
					double DotP2 = FVector::DotProduct(CheckDir2,FollowCamera->GetForwardVector());
					WhatEnemy.Add(DotP2);
					findEnemy = true;
				}
			}
		}
		if (findEnemy)
		{	// 카메라가 보는 방향에가장 근접한 에너미
			int32 index=0;
			int32 dist=-1;
			for (int32 i=0;i<WhatEnemy.Num();i++)
			{
				if (dist<WhatEnemy[i])
				{
					index=i;
				}
			}
			bCanAssasination=true;
			AssaionateEnemy = hitEnemys[index];

		}
		else
		{
			bCanAssasination=false;
			AssaionateEnemy=nullptr;
		}
	}//DrawDebugSphere(GetWorld(), MeLoc, 300, 30, FColor::Green, false, 1.0f);

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
	bIsgrabbed = true;
	bCangrabbed = false;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZGrabbedBase);
	GrabbedEnemy=Enemy;
	FVector meLoc = GetActorLocation();
	if (Enemy)
	{
		Enemy->SetActorLocation(meLoc + GetActorForwardVector());
		Enemy->SetActorRotation((GetActorLocation() - Enemy->GetActorLocation()).Rotation());
	}
	//UE_LOG(LogTemp, Warning, TEXT("Grabbedzz"));
}

void AKZGCharacter::EscapebyZombie()
{
	bIsgrabbed=false;   
	GrabbedEnemy=nullptr;
	if (bIsCrouching) bIsCrouching = false;
	//UE_LOG(LogTemp, Warning, TEXT("Escapezz"));
	anim->playPushAnimation();
	FTimerHandle myhandle;
	GetWorldTimerManager().SetTimer(myhandle, this,&AKZGCharacter::GrabbedDelay, grabbedDelayTime, false);
}

void AKZGCharacter::TryEscape()
{
	//UE_LOG(LogTemp, Warning, TEXT("Tryzz"));
	DamagedStamina(damageNumber);
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

void AKZGCharacter::Multicast_GrabbedWidget_Implementation()
{
	
}

void AKZGCharacter::Server_PlayerDeath_Implementation()
{
	Multicast_PlayerDeath();
}

void AKZGCharacter::Multicast_PlayerDeath_Implementation()
{
	bIsDead = true;
	anim->PlayDeathAnimation();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKZGCharacter::AttackCollisionOff()
{
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKZGCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Zombie = Cast<AEnemy>(OtherActor);
	Zombie->FSM->Target=this;
	Zombie->Damaged(damagePower);
	
	if (OtherActor == Zombie)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), batHitSound, GetActorLocation(), FRotator(), 0.4f);
		if (batMesh->IsVisible())
		{
			curWeaponHP -= 5;
		}
		else if (axeMesh->IsVisible())
		{
			curWeaponHP -= 5;
		}
	}
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Warning, TEXT("Collision OFFzz"));

	
	
}

void AKZGCharacter::OnComponentBeginOverlapFood(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (AH_AttackWeapons* Weapon = Cast<AH_AttackWeapons>(OtherActor))
	{
		Weapon->Destroy();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Overlap")));

		if (OtherActor->GetName().Contains(FString(TEXT("Bat"))))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%s"), *Weapon->GetName()));\
			batMesh->SetVisibility(true);
			curWeaponHP = maxWeaponHP;
			if (axeMesh->IsVisible())
			{
				axeMesh->SetVisibility(false);
			}
		}
		else if (OtherActor->GetName().Contains(FString(TEXT("Axe"))))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%s"), *Weapon->GetName()));
			axeMesh->SetVisibility(true);
			curWeaponHP = maxWeaponHP;
			if (batMesh->IsVisible())
			{
				batMesh->SetVisibility(false);
			}
		}
	}
}	

void AKZGCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapping = false;

}

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
		if(bIsDead) return;

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

	if (Controller != nullptr && !bIsgrabbed)
	{
		if (bIsDead) return;
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
	if(axeMesh->IsVisible() == false && batMesh->IsVisible() == false) return;
	if(bIsAttacking) return;
	if(bIsFinalAttackEnded) return;
	if (bIsDead) return;
	if (!bIsgrabbed && currentStamina > 5) {
		//if (attackNum <= 100) anim->PlayAttackAnimation1();
		//UE_LOG(LogTemp, Warning, TEXT("Collision ONzz"));
		boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), batSwingSound, GetActorLocation(), FRotator(), 1.0f);
		FTimerHandle CollisionTimerHandle;
		GetWorldTimerManager().SetTimer(CollisionTimerHandle, this, &AKZGCharacter::AttackCollisionOff, 0.1f, false);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZHitBase);
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
	if(bIsAttacking) return;
	if (bIsDead) return;
	if (bIsgrabbed && !bCanAssasination)
	{
		if (currentStamina > 1)
		{
			bIsInteractionInput = true;
			TryEscape();
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZGrabbedBase);
		}
		
	}
	else if(!bIsgrabbed && !bCanAssasination)
	{
		if (axeMesh->IsVisible() == false && batMesh->IsVisible() == false) return;

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
						killNum++;
						hitEnemy->FSM->ChangeToDieState();
						GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZGrabbedBase);
					}
				}
				//맞은액터 확인용
			}
		}
	}
	else if (!bIsgrabbed && bCanAssasination && !bStartAssaination)
	{
		if (AssaionateEnemy!=nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.01, FColor::Black, FString::Printf(TEXT("Assasination")));
			bStartAssaination = true;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZFinalBase);
			anim->playAssasinationAnimation();
			AssaionateEnemy->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 30);
			AssaionateEnemy->SetActorRotation(GetActorForwardVector().Rotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), assasinationSound ,GetActorLocation(), FRotator() , 0.4f);

			AssaionateEnemy->FSM->ChangeToDieState();
		}
	}
}

void AKZGCharacter::Server_InteractionInputEnd_Implementation()
{
	//Multicast_InteractionUnputEnd();
	bIsInteractionInput = false;

}

void AKZGCharacter::Multicast_InteractionUnputEnd_Implementation()
{
	bIsInteractionInput = false;
}

void AKZGCharacter::Server_ChangeView_Implementation()
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

void AKZGCharacter::Multicast_ChangeView_Implementation()
{
	
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
	//DOREPLIFETIME(AKZGCharacter, bIsgrabbed);
	DOREPLIFETIME(AKZGCharacter, bIsInteractionInput);
	DOREPLIFETIME(AKZGCharacter, maxsize);
	DOREPLIFETIME(AKZGCharacter, EWidget);

}
