// Fill out your copyright notice in the Description page of Project Settings.


#include "DropProp.h"

// Sets default values
ADropProp::ADropProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADropProp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADropProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		FVector P0 = GetActorLocation();
		FVector direction = SpawnLoc - P0;
		direction.Normalize();
		//UE_LOG(LogTemp, Warning, TEXT("Droping%f,&%f"), P0.Z, direction.Z);
		FVector vt = direction * dropspeed * DeltaTime;
		FVector P = P0 + vt;
		//UE_LOG(LogTemp, Warning, TEXT("gogo zz%f,%f"),P0.Z,P.Z);
		SetActorLocation(P);
		if (FVector::Dist(P, SpawnLoc) < 100.0f)
		{
			SpawnDrop();
			Destroy();
		}
}

void ADropProp::SpawnDrop()
{
	//소환해라
	UE_LOG(LogTemp,Warning,TEXT("Spwanzz"));
	GetWorld()->SpawnActor<AActor>(DropActor, GetActorLocation(), FRotator(0, 0, 0));

	AActor* SmokeA=GetWorld()->SpawnActor<AActor>(SmokeActor, GetActorLocation(), FRotator(0, 0, 0));
	FTimerHandle myTimerHandle;
}

