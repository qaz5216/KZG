// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AttackWeapons.h"
#include <Components/BoxComponent.h>
#include "../Enemy.h"
#include <Components/SphereComponent.h>

// Sets default values
AH_AttackWeapons::AH_AttackWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComp->SetupAttachment(RootComp);
	boxComp->SetBoxExtent(FVector(50, 10, 10));
	boxComp->SetCollisionProfileName(TEXT("Weapon"));

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetupAttachment(RootComp);
	sphereComp->SetSphereRadius(30);
	sphereComp->SetCollisionProfileName(TEXT("WeaponDrop"));

	bReplicates = true;

}

// Called when the game starts or when spawned
void AH_AttackWeapons::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AH_AttackWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

