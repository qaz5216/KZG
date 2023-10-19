// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFsm.h"
#include "Enemy.h"
#include "KZGCharacter.h"
#include <NavigationSystem/Public/NavigationSystem.h>
#include <AIModule/Classes/AIController.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "NavigationData.h"
#include <Components/CapsuleComponent.h>
#include "AITypes.h"
#include "NavigationSystemTypes.h"
#include <AIModule/Classes/Navigation/PathFollowingComponent.h>
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
	ai = Cast<AAIController>(Me->GetController());

	Me->GetCharacterMovement()->MaxWalkSpeed = speed;
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
	if (Target!=nullptr)
	{
		////PRINT2SCREEN(TEXT("No CurDestination Pos"));
		return;
	}
	FVector dest = Target->GetActorLocation();
	if (FVector::Dist(Me->GetActorLocation(), dest) < 100.0f) {
		//공격
		mState = EEnemyState::Attack;
		return;
	}
	EPathFollowingRequestResult::Type isAlreadyGoal = EPathFollowingRequestResult::Failed;
	FPathFindingResult r;
	FindPathByAI(dest, r);
	if (r.Result == ENavigationQueryResult::Success) {
		isAlreadyGoal = ai->MoveToLocation(dest);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Fail Loc"));
	}
}

void UEnemyFsm::MoveState()
{
	
}

void UEnemyFsm::AttackState()
{
	UE_LOG(LogTemp,Warning,TEXT("Attack"));
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

void UEnemyFsm::ChangeToTrackingState(class AKZGCharacter* NewTarget)
{
	if (mState!=EEnemyState::Tracking)
	{
		Target=NewTarget;	
		mState=EEnemyState::Tracking;
	}
}

bool UEnemyFsm::GetRandomPosInNavMesh(FVector center, float radius, FVector& dest)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(center, radius, loc);
	dest = loc.Location;
	return true;
}

void UEnemyFsm::FindPathByAI(FVector destination, FPathFindingResult& result)
{
	// navigation 시스템 세팅
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetGoalLocation(destination);
	req.SetAcceptanceRadius(3);

	ai->BuildPathfindingQuery(req, query);
	result = ns->FindPathSync(query);
}