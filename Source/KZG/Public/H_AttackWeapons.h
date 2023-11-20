// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_AttackWeapons.generated.h"

UCLASS()
class KZG_API AH_AttackWeapons : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AH_AttackWeapons();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;
	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category="HitPlaySound")
	class USoundBase* batHitSound;

	bool bIsOverlapping = false;

	class AKZGCharacter* me;

	
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "weaponHP")
	int32 WeaponHP = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Damage")
	int32 weaponDamage = 20;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Damage")
	int32 zombieDamage = 35;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
};
