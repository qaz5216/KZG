// Fill out your copyright notice in the Description page of Project Settings.


#include "H_KZG_Player.h"

// Sets default values
AH_KZG_Player::AH_KZG_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AH_KZG_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AH_KZG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AH_KZG_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

