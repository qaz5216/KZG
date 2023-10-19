// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFsm.h"
#include "Enemy.h"

// Sets default values for this component's properties
UEnemyFsm::UEnemyFsm()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFsm::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Me = Cast<AEnemy>(GetOwner());
}


// Called every frame
void UEnemyFsm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	switch (mState)
	{/*-수면 상태
		-단순 이동 / 순찰
		- 인지(사운드 / 시각)
		- 추적
	*/
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Tracking:
		TrackingState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	case EEnemyState::Sleep:
		SleepState();
		break;
	case EEnemyState::Search:
		SearchState();
		break;
	default:
		break;
	}
}

void UEnemyFsm::IdleState()
{

}

void UEnemyFsm::TrackingState()
{

}

void UEnemyFsm::MoveState()
{
	
}

void UEnemyFsm::AttackState()
{

}

void UEnemyFsm::DamageState()
{

}

void UEnemyFsm::DieState()
{

}

void UEnemyFsm::SleepState()
{

}

void UEnemyFsm::SearchState()
{

}

