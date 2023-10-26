// Fill out your copyright notice in the Description page of Project Settings.

#include "H_FacilityProps.h"
#include <Components/BoxComponent.h>

// Sets default values
AH_FacilityProps::AH_FacilityProps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = boxComp;
	boxComp->SetBoxExtent(FVector(75, 75, 75));
	boxComp->SetCollisionProfileName(TEXT("FacilityProp"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AH_FacilityProps::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AH_FacilityProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

