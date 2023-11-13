// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFsm.h"
#include <Components/CapsuleComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "EnemyStat.h"
#include "KZGCharacter.h"
#include "EnemyAnimInstance.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FSM = CreateDefaultSubobject<UEnemyFsm>(TEXT("FSM"));
	CapsuleComp=GetCapsuleComponent();
	SetRootComponent(CapsuleComp);
	SkeletalMeshComp=GetMesh();
	MyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MyStatWidget"));
	MyWidget->SetupAttachment(SkeletalMeshComp);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HP_Cur=HP_Max;
	Stamina_Cur=Stamina_Max;
	StatUI=Cast<UEnemyStat>(MyWidget->GetUserWidgetObject());
	StatUI->SetMaxHP(HP_Max);
	StatUI->SetHPBar(HP_Cur,HP_Max);
	StatUI->SetStaminaBar(Stamina_Cur,Stamina_Max);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bisUIShow)
	{
		if (FSM->Target!=nullptr)
		{
			MyWidget->SetWorldRotation((FSM->Target->GetActorLocation()-MyWidget->GetComponentLocation()).Rotation());
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Damaged(int32 damage)
{
	if (HP_Cur-damage>0)
	{
		HP_Cur=HP_Cur-damage;
		FSM->ChangeToDamageState();
	}
	else
	{
		HP_Cur=0;
		FSM->ChangeToDieState();
	}
	StatUI->SetHPBar(HP_Cur,HP_Max);
}

void AEnemy::StaminaDamaged(int32 value)
{
	if (Stamina_Cur - value > 0)
	{
		Stamina_Cur = Stamina_Cur - value;
		int32 index = FMath::RandRange(0, 1);
		//FString sectionName = FString::Printf(TEXT("Stamina%d"), index);
		//FSM->anim->PlayStaminaAnim(FName(*sectionName));
	}
	else
	{
		Stamina_Cur = 0;
	}
	StatUI->SetStaminaBar(Stamina_Cur,Stamina_Max);
}

void AEnemy::StaminaHeal(int32 value)
{
	if (Stamina_Cur + value > Stamina_Max)
	{
		Stamina_Cur = Stamina_Max;
	}
	else
	{
		Stamina_Cur = Stamina_Cur + value;
	}
	StatUI->SetStaminaBar(Stamina_Cur, Stamina_Max);
}

void AEnemy::AttachUI()
{
	if (StatUI!=nullptr)
	{
		bisUIShow = true;
		StatUI->ShowUI();
	}
	
}

void AEnemy::DetachUI()
{
	if (StatUI != nullptr)
	{
		bisUIShow = false;
		StatUI->HiddenUI();
	}
}
