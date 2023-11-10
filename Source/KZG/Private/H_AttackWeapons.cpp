// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AttackWeapons.h"
#include <Components/BoxComponent.h>
#include "../Enemy.h"

// Sets default values
AH_AttackWeapons::AH_AttackWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = boxComp;
	boxComp->SetBoxExtent(FVector(50, 50, 50));
	boxComp->SetCollisionProfileName(TEXT("WeaponDrop"));

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

