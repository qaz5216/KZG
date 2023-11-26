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
#include <Components/AudioComponent.h>
#include <Sound/SoundCue.h>
#include "BP_H_Gun.h"
#include "H_BatWeapon.h"
#include "H_AxeWeapon.h"
#include "H_AttackCamActor.h"


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

	gunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMesh"));
	gunMesh->SetupAttachment(GetMesh());
	// Get the socket name
	
	 
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Tempgun (TEXT("/Script/Engine.StaticMesh'/Game/HSH/AnimShooterPack/Weapons/Pistol/SM_handgun_02_main.SM_handgun_02_main'"));

	if (Tempgun.Succeeded())
	{
		gunMesh->SetStaticMesh(Tempgun.Object);
		gunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		gunMesh->SetVisibility(false);
		gunMesh->SetRelativeLocationAndRotation(FVector(-20.137923, 8.897582, 2.340531), FRotator(1.669609, 68.438939, 8.796659));

	}

	batMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("batMesh"));
	batMesh->SetupAttachment(GetMesh());
	// Get the socket name
	//FName WeaponSocketName = FName(TEXT("WeaoponSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TempBat (TEXT("/Script/Engine.StaticMesh'/Game/Props_MeleeWeapons/Meshes/baseballBat.baseballBat'"));

	if (TempBat.Succeeded())
	{
		batMesh->SetStaticMesh(TempBat.Object);
		batMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		batMesh->SetVisibility(false);
		

	}

	axeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxeMesh"));
	axeMesh->SetupAttachment(GetMesh());
	
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>TempAxe(TEXT("/Script/Engine.StaticMesh'/Game/Props_MeleeWeapons/Meshes/fireAxe.fireAxe'"));

	if (TempAxe.Succeeded())
	{
		axeMesh->SetStaticMesh(TempAxe.Object);
		axeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		axeMesh->SetVisibility(false);
		
	}

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComp->SetupAttachment(batMesh);
	boxComp->SetRelativeLocationAndRotation(FVector(0.704398, -0.587258, 35.754137), FRotator(90.000000, -0.000000, 0.000000));
	boxComp->SetBoxExtent(FVector(65, 10, 10));
	boxComp->SetRelativeScale3D(FVector(1));
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
	/*audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComp->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<USoundCue> DefaultBGMFinder(TEXT("/Script/Engine.SoundCue'/Game/Sound/BGM/Dark_Music_Pack/WAV/By_DrBorowski/BGM_Nomal_Cue.BGM_Nomal_Cue'"));
	if (DefaultBGMFinder.Succeeded())
	{
		DefaultBGM = DefaultBGMFinder.Object;
		audioComp->SetSound(DefaultBGM);
	}
	audioComp->Play();*/

	//static ConstructorHelpers::FClassFinder<UH_PlayerInfo> TempInfo(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/CYS/UI_CYS_PlayerInfo2.UI_CYS_PlayerInfo2_C'"));

	//if (TempInfo.Succeeded())
	//{
	//	// Assign the found class to a variable
	//	BP_InfoWidget = TempInfo.Class;
	//}
	bReplicates = true;
}

void AKZGCharacter::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(GetWorld(), bgmSound, 2.0f);

	//FTimerHandle ActorHandle;
	//GetWorldTimerManager().SetTimer(ActorHandle, this, &AKZGCharacter::SetPlayerLocationToFirst, restartLoc, false);

	FName WeaponSocketName = FName(TEXT("WeaoponSocket"));
	
	// Attach batMesh to the WeaoponSocket
	gunMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	gunMesh->SetRelativeLocationAndRotation(FVector(-20.137923, 8.897582, 2.340531), FRotator(1.669609, 68.438939, 8.796659));
	
	// Attach batMesh to the WeaoponSocket
	batMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	batMesh->SetRelativeLocationAndRotation(FVector(-13.419591, -2.414909, 9.095527), FRotator(12.700006, -15.579394, -41.744371));
	
	// Attach batMesh to the WeaoponSocket
	axeMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	axeMesh->SetRelativeLocationAndRotation(FVector(-11.144902, 1.984119, 4.615342), FRotator(12.700006, -15.579394, -41.744371));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AKZGCharacter::OnComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AKZGCharacter::OnComponentBeginOverlapFood);

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	returnSpeed = walkSpeed;

	anim = Cast<UH_KZGPlayerAnim>(GetMesh()->GetAnimInstance());

	EWidget = CreateWidget<UH_EWidget>(GetWorld(), BP_EWidget);
	InfoWidget = CreateWidget<UH_PlayerInfo>(GetWorld(), BP_InfoWidget);

	FTimerHandle widgetHandle;
	GetWorldTimerManager().SetTimer(widgetHandle, this, &AKZGCharacter::WidgetDelay, widgetDelay, false);
	
	currentStamina = playerStamina;
	curHungerP = maxHungerP;
	CameraLocation = FollowCamera->GetComponentLocation();
	CameraRot = FollowCamera->GetComponentRotation();
	camArmLen = CameraBoom->TargetArmLength;
	
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	camActor = GetWorld()->SpawnActor<AH_AttackCamActor>(BP_Cam, GetActorLocation(), GetActorRotation(), Param);
	//camActor = Cast<AH_AttackCamActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AH_AttackCamActor::StaticClass()));
}

void AKZGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, returnSpeed, 5 * DeltaTime);
	if (camActor)
	{
		camActor->SetActorLocationAndRotation(GrabbedCam->GetComponentLocation(), GrabbedCam->GetComponentRotation());
	}
	/*if (attackWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("weaponHP: %d"), attackWeapon->WeaponHP));
		if (attackWeapon->WeaponHP <= 0)
		{
			attackWeapon->Destroy();
			bHasWeapon = false;
		}
	}*/
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("Reload: %s"), bIsReloading ? *FString("true") : *FString("false")));
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("crouch: %s"), bIsCrouching ? *FString("true") : *FString("false")));
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("comboIndex: %d"), comboIndex));

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

	if (comboIndex == 1)
	{
		curComTime+= DeltaTime;

		if (curComTime > curComboSec)
		{
			bComboTime = false;
			curComTime = 0;
			comboIndex = 0;
		}
	}
	else if (comboIndex == 2)
	{
		curComTime += DeltaTime;

		if (curComTime > curComboSec)
		{
			bComboTime = false;
			curComTime = 0;
			comboIndex = 0;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Ammo: %d , Max: %d"), curAmmo, maxAmmo));

	/*if (curAmmo <= 0)
	{
		if(bIsReloading) return;
		bIsReloading = true;
		anim->playReloadAnim();
		maxAmmo -= 15 - curAmmo;
		FTimerHandle reloadHandle1;
		GetWorldTimerManager().SetTimer(reloadHandle1, this, &AKZGCharacter::FinishedReloading, 1.1f, false);
	}*/

	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Weapon HP : %d"), realWeaponHP));
	/*if (realWeaponHP <= 0)
	{
		if (axeMesh->IsVisible()) axeMesh->SetVisibility(false);
		else if (batMesh->IsVisible()) batMesh->SetVisibility(false);
		bHasWeapon = false;
	}*/
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
			DamagedStamina(recoveryPoint * 2);
			//currentStamina -= recoveryPoint;
			curSP = 0;
		}
	}
	else if (bIsCrouching && !bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = 150;
		curSP += DeltaTime;
		if (curSP > recoverTime)
		{
			//DamagedStamina(recoveryPoint);
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
			DamagedStamina(recoveryPoint);
			//currentStamina += recoveryPoint;
			curSP = 0;
		}
	}

	Server_GrabbedWidget();
	//Server_ChangeView();
	if (bIsgrabbed)
	{
		
		boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		if (APlayerController* pc = Cast<APlayerController>(Controller))
		{
			if (camActor)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("%s"), *camActor->GetName()));
				pc->SetViewTargetWithBlend(camActor, blendTime);
			}
		}
		//GrabbedCam->SetActive(true);
		//FollowCamera->SetActive(false);
		if (bHasGun)
		{
			bHasGun = false;
			bGotGun = true;
		}
	}
	else if(!bIsgrabbed)
	{
		if (bGotGun)
		{
			bHasGun = true;
			bGotGun = false;
		}
		//FollowCamera->SetActive(true);
		//GrabbedCam->SetActive(false);
	}
	
	curHungtime += DeltaTime;
	if (curHungtime > 1) 
	{
		curHungerP--;
		curHungtime = 0;
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("%f"), CameraBoom->TargetArmLength));

	if (bIsAttacking || bIsZooming)
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
			if (WhatEnemy[index] < assasinationDistance)
			{
				bCanAssasination=true;
				AssaionateEnemy = hitEnemys[index];
			}
			else
			{
				bCanAssasination = false;
				AssaionateEnemy = nullptr;
			}

		}
		else
		{
			bCanAssasination=false;
			AssaionateEnemy=nullptr;
		}
	}//DrawDebugSphere(GetWorld(), MeLoc, 300, 30, FColor::Green, false, 1.0f);

}

//void AKZGCharacter::SwitchBGMtoDetecting()
//{
//	// Check if the audio component and the new background music sound cue are valid
//	if (audioComp && !bIsBGMDetecting)
//	{
//		// Stop the currently playing background music
//		bIsBGMDetecting = true;
//		if(bIsBGMDiscover) bIsBGMDiscover = false;
//		if(bIsBGMGrab) bIsBGMGrab = false;
//		if(bIsBGMDefault) bIsBGMDefault = false;
//		audioComp->Stop();
//
//		// Set the new background music sound cue (replace "NewBGM" with your actual sound cue asset)
//		USoundCue* LoadCue = LoadObject<USoundCue> (nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sound/BGM/Dark_Music_Pack/WAV/BGM_ChaseStart_Cue.BGM_ChaseStart_Cue'"));
//		if (LoadCue)
//		{
//			audioComp->SetSound(LoadCue);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Detecting BGM Error"));
//		}
//
//		// Start playing the new background music
//		audioComp->Play();
//	}
//}
//
//
//void AKZGCharacter::SwitchBGMtoDefault()
//{
//	// Check if the audio component and the new background music sound cue are valid
//	if (audioComp && !bIsBGMDefault)
//	{
//		// Stop the currently playing background music
//
//		bIsBGMDefault = true;
//		if (bIsBGMDiscover) bIsBGMDiscover = false;
//		if (bIsBGMGrab) bIsBGMGrab = false;
//		if (bIsBGMDetecting) bIsBGMDetecting = false;
//
//		audioComp->Stop();
//
//		// Set the new background music sound cue (replace "NewBGM" with your actual sound cue asset)
//		if (DefaultBGM)
//		{
//			audioComp->SetSound(DefaultBGM);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Default BGM Error"));
//		}
//
//		// Start playing the new background music
//		audioComp->Play();
//	}
//}
//
//void AKZGCharacter::SwitchBGMtoDiscover()
//{
//	// Check if the audio component and the new background music sound cue are valid
//	if (audioComp && !bIsBGMDiscover)
//	{
//		// Stop the currently playing background music
//		bIsBGMDiscover = true;
//		if (bIsBGMDefault) bIsBGMDefault = false;
//		if (bIsBGMGrab) bIsBGMGrab = false;
//		if (bIsBGMDetecting) bIsBGMDetecting = false;
//
//		audioComp->Stop();
//
//		// Set the new background music sound cue (replace "NewBGM" with your actual sound cue asset)
//		USoundCue* LoadCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sound/BGM/Trailer/BGM_FindZombie_Cue.BGM_FindZombie_Cue'"));
//		if (LoadCue)
//		{
//			audioComp->SetSound(LoadCue);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Detecting BGM Error"));
//		}
//
//		// Start playing the new background music
//		audioComp->Play();
//	}
//}
//
//void AKZGCharacter::SwitchBGMtoGrab()
//{
//	// Check if the audio component and the new background music sound cue are valid
//	if (audioComp && !bIsBGMGrab)
//	{
//		// Stop the currently playing background music
//		bIsBGMGrab = true;
//		if (bIsBGMDefault) bIsBGMDefault = false;
//		if (bIsBGMDiscover) bIsBGMDiscover = false;
//		if (bIsBGMDetecting) bIsBGMDetecting = false;
//
//		audioComp->Stop();
//
//		// Set the new background music sound cue (replace "NewBGM" with your actual sound cue asset)
//		USoundCue* LoadCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sound/BGM/Trailer/BGM_Discover_Cue.BGM_Discover_Cue'"));
//		if (LoadCue)
//		{
//			audioComp->SetSound(LoadCue);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Detecting BGM Error"));
//		}
//
//		// Start playing the new background music
//		audioComp->Play();
//	}
//}



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

				//SwitchBGMtoDetecting();

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
	if (bIsCrouching) bIsCrouching = false;
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
	if (APlayerController* pc = Cast<APlayerController>(Controller))
	{
		pc->SetViewTargetWithBlend(this, blendTime);

	}
	bIsgrabbed=false;   
	GrabbedEnemy=nullptr;
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
		if (GrabbedEnemy!=nullptr)
		{
			GrabbedEnemy->FSM->ChangeToKillState();
		}
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

void AKZGCharacter::AssasinationDeath()
{
	bIsDead = true;
	anim->playAssasinationDeathAnimation();
	//anim->PlayDeathGrabMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKZGCharacter::AttackCollisionOff()
{
	/*if (attackWeapon)
	{
		attackWeapon->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}*/
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKZGCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Zombie = Cast<AEnemy>(OtherActor);
	Zombie->FSM->Target = this;
	Zombie->Damaged(damagePower);

	

	if (OtherActor == Zombie)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), batHitSound, GetActorLocation(), FRotator(), 0.4f);
		if (batMesh->IsVisible())
		{
			realWeaponHP -= weaponDamage;
		}
		else if (axeMesh->IsVisible())
		{
			realWeaponHP -= weaponDamage;
		}
	}

	if (realWeaponHP <= 0)
	{
		if (axeMesh->IsVisible()) axeMesh->SetVisibility(false);
		else if (batMesh->IsVisible()) batMesh->SetVisibility(false);
		UGameplayStatics::PlaySound2D(GetWorld(), batDestSound, 1.0f);
		bHasWeapon = false;
	}

	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKZGCharacter::OnComponentBeginOverlapFood(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	attackWeapon = Cast<AH_AttackWeapons>(OtherActor);
	gunWeapon = Cast<ABP_H_Gun>(OtherActor);

	/*if (attackWeapon && !bHasWeapon && !bHasGun)
	{
		if (attackWeapon->meshComp != nullptr)
		{
			attackWeapon->meshComp->SetSimulatePhysics(false);
			attackWeapon->meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		attackWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WeaoponSocket")));
		attackWeapon->SetActorRelativeLocation(FVector(-15.411383, -18.107558, 29.253529));
		attackWeapon->SetActorRelativeRotation(FRotator(48.973539, -105.339813, -101.692076));
		bHasWeapon = true;
	}*/
	/*else if (gunWeapon && !bHasGun && !bHasWeapon)
	{
		gunMesh->SetVisibility(true);
		gunWeapon->Destroy();
		bHasGun = true;
	}*/
	//else if (axeWeapon && !bHasWeapon && !bHasGun)
	//{
	//	/*axeWeapon->meshComp->SetSimulatePhysics(false);
	//	axeWeapon->meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	//	axeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WeaoponSocket")));
	//	axeWeapon->SetActorRelativeLocation(FVector(-15.411383, -18.107558, 29.253529));
	//	axeWeapon->SetActorRelativeRotation(FRotator(48.973539, -105.339813, -101.692076));
	//	bHasWeapon = true;
	//}

	if (attackWeapon && !bHasWeapon && !bHasGun)
	{
		bHasWeapon = true;
		attackWeapon->Destroy();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Overlap")));

		if (OtherActor->GetName().Contains(FString(TEXT("Bat"))))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%s"), *Weapon->GetName()));
			batMesh->SetVisibility(true);
			UGameplayStatics::PlaySound2D(GetWorld(), batEquipSound, batEquipSoundVolume);
			realWeaponHP = attackWeapon->WeaponHP;
			if (axeMesh->IsVisible())
			{
				axeMesh->SetVisibility(false);
			}
		}
		else if (OtherActor->GetName().Contains(FString(TEXT("Axe"))))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%s"), *Weapon->GetName()));
			axeMesh->SetVisibility(true);
			UGameplayStatics::PlaySound2D(GetWorld(), batEquipSound, 1.0f);
			realWeaponHP = attackWeapon->WeaponHP;
			if (batMesh->IsVisible())
			{
				batMesh->SetVisibility(false);
			}
		}
	}
	else if (gunWeapon && !bHasGun && !bHasWeapon)
	{
		gunMesh->SetVisibility(true);
		gunWeapon->Destroy();
		UGameplayStatics::PlaySound2D(GetWorld(), gunEquipSound, 1.0f);
		bHasGun = true;
	}
}	



void AKZGCharacter::SetPlayerLocationToFirst()
{
	SetActorLocation(FVector(-6113.896223, 17158.341344, 4187.370783));
}

void AKZGCharacter::WidgetDelay()
{
	if (InfoWidget != nullptr)
	{
		InfoWidget->AddToViewport();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("2.0f")));

	}
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

		EnhancedInputComponent->BindAction(throwAction, ETriggerEvent::Triggered, this, &AKZGCharacter::ThrowAction);

		EnhancedInputComponent->BindAction(pressedOne, ETriggerEvent::Triggered, this, &AKZGCharacter::PressedOneAction);

		EnhancedInputComponent->BindAction(pressedTwo, ETriggerEvent::Triggered, this, &AKZGCharacter::PressedTwoAction);

		EnhancedInputComponent->BindAction(zoomAction, ETriggerEvent::Started, this, &AKZGCharacter::StartedZoom);
		EnhancedInputComponent->BindAction(zoomAction, ETriggerEvent::Completed, this, &AKZGCharacter::FinishedZoom);

		EnhancedInputComponent->BindAction(reloadAction, ETriggerEvent::Triggered, this, &AKZGCharacter::ReloadAmmo);

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
		if (bStartAssaination) return;
		if (bIsFinalAttackEnded) return;

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
	//if(axeMesh->IsVisible() == false && batMesh->IsVisible() == false) return;
	if(bIsAttacking) return;
	if(bIsFinalAttackEnded) return;
	if (bIsDead) return;
	if (!bIsgrabbed && currentStamina > 5 && bHasWeapon && !bHasGun) {
		bIsAttacking = true;
		//if (attackNum <= 100) anim->PlayAttackAnimation1();
		//UE_LOG(LogTemp, Warning, TEXT("Collision ONzz"));
		boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		/*if (attackWeapon)
		{
			attackWeapon->boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}*/
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), batSwingSound, GetActorLocation(), FRotator(), 2.0f);
		FTimerHandle CollisionTimerHandle;
		GetWorldTimerManager().SetTimer(CollisionTimerHandle, this, &AKZGCharacter::AttackCollisionOff, 1.0f, false);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZHitBase);
		
		anim->PlayComboAnimation1();
		if (comboIndex == 1 && bComboTime)
		{
			anim->PlayComboAnimation2();
		}
		if (comboIndex == 2 && bComboTime)
		{
			anim->PlayComboAnimation3();
		}		
	}
	else if (!bIsgrabbed && currentStamina > 5 && !bHasWeapon && bHasGun && bIsZooming)
	{
		if(bIsReloading) return;
		
		if(curAmmo <= 0) 
		{
			UGameplayStatics::PlaySound2D(GetWorld(), gunEmptySound, 1.0f);
			return;
		}
		else UGameplayStatics::PlaySound2D(GetWorld(), gunShotSound, 1.0f);

		if(maxAmmo <= -14) curAmmo = 0;
		
		//1. 시작점이 필요하다.
		FVector startPos = FollowCamera->GetComponentLocation();
		//2. 종료점이 필요하다.
		FVector endPos = startPos + FollowCamera->GetForwardVector() * 5000;
		//3. 선을 만들어야 한다, 나는 안맞게 만들어야 한다.
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);
		//bool bHit = GetWorld()->LineTraceSingleByProfile(hitInfo, startPos, endPos, TEXT("Weapon"), param);
		FTransform trans1;
		trans1.SetLocation(gunMesh->GetSocketLocation(FName(FString(TEXT("ParticleSocket")))));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BP_gunEffect, trans1);

		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(gunShakeBase);
		anim->playGunShootAnim();
		curAmmo--;
		//4. 선이 부딪혔으니까
		if (bHit)
		{	
			AEnemy* enemy = Cast<AEnemy>(hitInfo.GetActor());
			FTransform trans;
			trans.SetLocation(hitInfo.ImpactPoint);
			trans.SetRotation(hitInfo.ImpactNormal.ToOrientationQuat());
			if (enemy)
			{
				enemy->Damaged(gunDamage);
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("%d"), enemy->HP_Cur));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BP_shotBloodEffect, trans);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BP_shotEffect, trans);
			}
			else
			{
				
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BP_shotEffect, trans);
			}
			
		}
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
		if (currentStamina > 10)
		{
			bIsInteractionInput = true;
			TryEscape();
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ZGrabbedBase);
		}
		
	}
	else if(!bIsgrabbed && !bCanAssasination)
	{
		if (axeMesh->IsVisible() == false && batMesh->IsVisible() == false) return;
		if(!bHasWeapon) return;
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
			AssaionateEnemy->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 70);
			AssaionateEnemy->SetActorRotation(GetActorForwardVector().Rotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), assasinationSound ,GetActorLocation(), FRotator() , 0.4f);

			AssaionateEnemy->FSM->ChangeToAssasinDieState();
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

void AKZGCharacter::ThrowAction()
{
	/*if (attackWeapon != nullptr)
	{
		if (attackWeapon->GetName().Contains(FString(TEXT("Bat"))))
		{
			FActorSpawnParameters Param;
			Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AH_AttackWeapons* weapon = GetWorld()->SpawnActor<AH_AttackWeapons>(BP_BatWeapon, GetActorLocation() + GetActorForwardVector() * 100, FRotator(), Param);
			weapon->WeaponHP = realWeaponHP;
			attackWeapon->Destroy();
			bHasWeapon = false;
		}
		else if (attackWeapon->GetName().Contains(FString(TEXT("Axe"))))
		{
			FActorSpawnParameters Param;
			Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AH_AttackWeapons* weapon = GetWorld()->SpawnActor<AH_AttackWeapons>(BP_AxeWeapon, GetActorLocation() + GetActorForwardVector() * 100, FRotator(), Param);

			weapon->WeaponHP = realWeaponHP;
			attackWeapon->Destroy();
			bHasWeapon = false;
		}

	}*/
	/*if (gunMesh->IsVisible())
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABP_H_Gun* weapon = GetWorld()->SpawnActor<ABP_H_Gun>(BP_Gun, GetActorLocation() + GetActorForwardVector() * 100, FRotator(), Param);

		gunMesh->SetVisibility(false);
		bHasGun = false;
	}*/

	
	if (axeMesh->IsVisible())
	{
		axeMesh->SetVisibility(false);
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AH_AttackWeapons* weapon = GetWorld()->SpawnActor<AH_AttackWeapons>(BP_AxeWeapon, GetActorLocation() + GetActorForwardVector() * 100, FRotator(), Param);
		if (weapon)
		{
			weapon->WeaponHP = realWeaponHP;
		}
		bHasWeapon = false;
	}
	else if (batMesh->IsVisible())
	{
		batMesh->SetVisibility(false);
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AH_AttackWeapons* weapon = GetWorld()->SpawnActor<AH_AttackWeapons>(BP_BatWeapon, GetActorLocation() + GetActorForwardVector() * 100, FRotator(), Param);

		if (weapon)
		{
			weapon->WeaponHP = realWeaponHP;

		}
		bHasWeapon = false;
	}
	else if (gunMesh->IsVisible())
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABP_H_Gun* weapon = GetWorld()->SpawnActor<ABP_H_Gun>(BP_Gun, GetActorLocation() + GetActorForwardVector() * 100, FRotator(), Param);

		gunMesh->SetVisibility(false);
		bHasGun = false;
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


void AKZGCharacter::PressedOneAction()
{
	
}

void AKZGCharacter::PressedTwoAction()
{
	
}

void AKZGCharacter::StartedZoom()
{
	if(bHasGun) bIsZooming = true;
}

void AKZGCharacter::FinishedZoom()
{
	if (bHasGun) bIsZooming = false;
}

void AKZGCharacter::ReloadAmmo()
{
	if(!bIsReloading) 
	{
		if (curAmmo >= 15) return;
		bIsReloading = true;
		anim->playReloadAnim();
		maxAmmo -= 15 - curAmmo;
		FTimerHandle reloadHandle;
		GetWorldTimerManager().SetTimer(reloadHandle, this, &AKZGCharacter::FinishedReloading, 1.1f, false);
	}
}

void AKZGCharacter::FinishedReloading()
{
	bIsReloading = false;
	curAmmo = 15;
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
