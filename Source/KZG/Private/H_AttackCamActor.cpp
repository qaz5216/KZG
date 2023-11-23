// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AttackCamActor.h"
#include <Camera/CameraComponent.h>

// Sets default values
AH_AttackCamActor::AH_AttackCamActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	Cam->SetupAttachment(RootComp);
	
}

// Called when the game starts or when spawned
void AH_AttackCamActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AH_AttackCamActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

