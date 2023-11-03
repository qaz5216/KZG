// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AirPlane.generated.h"

UCLASS()
class KZG_API AAirPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAirPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	FVector StartLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	FVector EndLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	FVector DropLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	float Speed=2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	bool bisdrop=false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	TSubclassOf<class ADropProp> prop;

	void drop();
};