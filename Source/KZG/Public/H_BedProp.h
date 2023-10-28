// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "H_FacilityProps.h"
#include "H_BedProp.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API AH_BedProp : public AH_FacilityProps
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AH_BedProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComp;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
