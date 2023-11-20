// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_H_Gun.h"
#include <Components/SphereComponent.h>

// Sets default values
ABP_H_Gun::ABP_H_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = sphereComp;
	sphereComp->SetSphereRadius(30);
	sphereComp->SetCollisionProfileName(TEXT("WeaponDrop"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunComp"));
	meshComp->SetupAttachment(sphereComp);

}

// Called when the game starts or when spawned
void ABP_H_Gun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_H_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

