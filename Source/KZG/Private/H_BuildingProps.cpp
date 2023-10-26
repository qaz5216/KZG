// Fill out your copyright notice in the Description page of Project Settings.

#include "H_BuildingProps.h"
#include <Components/BoxComponent.h>

// Sets default values
AH_BuildingProps::AH_BuildingProps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = boxComp;
	boxComp->SetBoxExtent(FVector(75, 75, 75));
	boxComp->SetCollisionProfileName(TEXT("BuildingProp"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AH_BuildingProps::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AH_BuildingProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

