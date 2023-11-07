// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class KZG_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UEnemyFsm* FSM;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 HP_Max=100;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 HP_Cur=100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Stamina_Max=100;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 Stamina_Cur=100;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool isGroggy=false;


	UFUNCTION(BlueprintCallable)
	void Damaged(int32 damage);

	UFUNCTION(BlueprintCallable)
	void StaminaDamaged(int32 value);

	UFUNCTION(BlueprintCallable)
	void StaminaHeal(int32 value);


	UFUNCTION(BlueprintCallable)
	void AttachUI();
	UFUNCTION(BlueprintCallable)
	void DetachUI();

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|UI")
	class UEnemyStat* StatUI;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|UI")
	class UWidgetComponent* MyWidget;

	bool bisUIShow = false;
};
