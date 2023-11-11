// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFsm.generated.h"
UENUM(BlueprintType)

enum class EEnemyState : uint8
{/*-수면 상태
		-단순 이동 / 순찰
		- 인지(사운드 / 시각)
		- 추적
	*/
	Idle UMETA(DisplayName = "Idle State"),
	Recognition,
	Tracking,
	Attack,
	Damage,
	Die,
	Sleep,
	Groggy,
	kill,
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

	void KillingState(float DeltaTime);
	void SleepState(float DeltaTime);
	void IdleState(float DeltaTime);
	void TrackingState(float DeltaTime);
	void RecognitionState(float DeltaTime);
	void AttackState(float DeltaTime);
	void DamageState(float DeltaTime);
	void DieState(float DeltaTime);
	void SleepState();
	void GroggyState(float DeltaTime);



	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FSM")
	EEnemyState mState=EEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FSM")
	EEnemyState PremState;

	UPROPERTY()
	class UEnemyAnimInstance* anim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	class AEnemy *Me;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	class AKZGCharacter *Target;
	
	UPROPERTY(EditAnywhere, Category = "FSM")
	EEnemyState StartState=EEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool killing=false;

	void killingplay();

	void ChangeToTrackingState(class AKZGCharacter* NewTarget);

	// 패트롤 하기 위해서 이동가능 위치를 랜덤하게 찾아주기
	bool GetRandomPosInNavMesh(FVector center, float radius, FVector& dest);
	// FindPathByAI 
	void FindPathByAI(FVector destination, struct FPathFindingResult& result);

	UPROPERTY()
	class AAIController* ai;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float speed = 150;
	

	UPROPERTY(EditAnywhere, Category = "FSM")
	float runspeed = 300;
	
	float dietime=0;

	void ChangeToIdleState();

	void ChangeToAttackState();

	void ChangeToGroggyState();

	void ChangeToRecognitionState(class AKZGCharacter* NewTarget);

	void ChangeToDamageState();

	void ChangeToDieState();

	void ChangeToSleepState();

	void Viewing();

	bool SeeTarget(class AKZGCharacter* NewTarget);


	void Recognition(class AKZGCharacter* NewTarget);
	
	UPROPERTY(EditAnywhere, Category = "FSM")
	FVector dest;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float viewDistance=1000.0f;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float recognitiontime_cur=0;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float recognitiontime=5;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float groggytime_cur=0;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float groggytime=5;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float groggydelaytime=1;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float Idletime_cur=0;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float idletime=3;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float damagetime_cur=0;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float damagetime=3;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float Trackingtime_cur=0;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float Trackingtime=3;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float attacktime_cur=0;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float attacktime=1;

	float startdelay=0;

	//Idle상태일때
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	FVector SearchLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	FVector SearchDest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float SearchDist=500;

	UPROPERTY(EditAnywhere, Category = "FSM")
	FVector RecognitionLoc;

	bool start=false;
};
