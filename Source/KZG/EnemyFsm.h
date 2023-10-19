// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFsm.generated.h"
UENUM(BlueprintType)

enum class EEnemyState : uint8
{/*-���� ����
		-�ܼ� �̵� / ����
		- ����(���� / �ð�)
		- ����
	*/
	Idle UMETA(DisplayName = "Idle State"),
	Move,
	Tracking,
	Attack,
	Damage,
	Die,
	Sleep,
	Search,
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KZG_API UEnemyFsm : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFsm();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void IdleState();
	void TrackingState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
	void SleepState();
	void SearchState();
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FSM")
	EEnemyState mState=EEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	class AEnemy *Me;
};
