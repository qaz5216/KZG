// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropProp.generated.h"

UCLASS()
class KZG_API ADropProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	TSubclassOf<class AActor> DropActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	TSubclassOf<class AActor> SmokeActor;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	FVector SpawnLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	float dropspeed=500;

	void SpawnDrop();
};
