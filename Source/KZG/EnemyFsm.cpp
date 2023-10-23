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
#include "EnemyStat.h"
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
	
	//Target=Cast<AKZGCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(),AKZGCharacter::StaticClass()));
	//mState=EEnemyState::Tracking;
	SearchLoc=Me->GetActorLocation();
	GetRandomPosInNavMesh(SearchLoc, SearchDist, SearchDest);
	start=true;
}


// Called every frame
void UEnemyFsm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	if (ai!=nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("AI NULL zz"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI YEs zz"));
	}	

	if (start)
	{
	switch (mState)
	{/*-수면 상태
		-단순 이동 / 순찰
		- 인지(사운드 / 시각)
		- 추적
	*/
	case EEnemyState::Idle:
		IdleState(DeltaTime);
		break;
	case EEnemyState::Tracking:
		TrackingState();
		break;
	case EEnemyState::Recognition:
		RecognitionState(DeltaTime);
		break;
	case EEnemyState::Attack:
		AttackState(DeltaTime);
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
	case EEnemyState::Groggy:
		GroggyState(DeltaTime);
		break;
	default:
		break;
	}
	}
	
}

void UEnemyFsm::IdleState(float DeltaTime)
{
	//낮일때
	FVector MyLoc= Me->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Dest:%f,%f,%f MyLoc:%f,%f,%f dist=%f"),SearchDest.X,SearchDest.Y,SearchDest.Z,MyLoc.X,MyLoc.Y,MyLoc.Z, FVector::Dist(MyLoc, SearchDest));
	
	if (FVector::Dist(MyLoc, SearchDest) < 100.0f) {
		// 목표지점도착 3초정도 있다가 다음 탐색지역으로
		if (Idletime_cur>idletime)
		{
			UE_LOG(LogTemp, Warning, TEXT("DestFix"));
			Idletime_cur=0;
			GetRandomPosInNavMesh(SearchLoc, SearchDist, SearchDest);
		}
		else
		{
			Idletime_cur+=DeltaTime;
		}
		return;
	}
	EPathFollowingRequestResult::Type isAlreadyGoal = EPathFollowingRequestResult::Failed;
	FPathFindingResult r;
	FindPathByAI(SearchDest, r);
	if (r.Result == ENavigationQueryResult::Success) {
		if (ai!=nullptr&&SearchDest!=FVector(0,0,0))
		{
			isAlreadyGoal = ai->MoveToLocation(SearchDest);
			//UE_LOG(LogTemp, Warning, TEXT("MoveMovezz"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail Loc"));
	}

	/*밤일때
		SleepState();
	*/
}

void UEnemyFsm::TrackingState()
{
	if (Target==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullzz"));
		return;
	}
	FVector dest = Target->GetActorLocation();
	if (FVector::Dist(Me->GetActorLocation(), dest) < 100.0f) {
		//공격
		ChangeToAttackState();
		return;
	}
	EPathFollowingRequestResult::Type isAlreadyGoal = EPathFollowingRequestResult::Failed;
	FPathFindingResult r;
	FindPathByAI(dest, r);
	if (r.Result == ENavigationQueryResult::Success) {
		if (ai != nullptr)
		{
			isAlreadyGoal = ai->MoveToLocation(dest);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Fail Loc"));
	}
}

void UEnemyFsm::RecognitionState(float DeltaTime)
{
	//5초안에 별일 없으면 다시 idle;
	if (recognitiontime_cur>recognitiontime)
	{
		ChangeToIdleState();
		return;
	}
	else
	{
		recognitiontime_cur+=DeltaTime;
	}
	//일단 타겟방향으로 몸돌리기
}


void UEnemyFsm::AttackState(float DeltaTime)
{
	if (Me->Stamina_Cur>0)
	{
		if (attacktime_cur>attacktime)
		{
			Target->DamagedStamina(1);
			// player->damagedstamina(int32 value)
			UE_LOG(LogTemp, Warning, TEXT("Attack remain Sta=%d"),Target->playerStamina);
			attacktime_cur=0;
		}
		else
		{
			attacktime_cur+=DeltaTime;
		}
	}
	else
	{
		//스턴좀 먹고
		Me->StatUI->EImageHidden();
		ChangeToGroggyState();
	}
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

void UEnemyFsm::GroggyState(float DeltaTime)
{
	if (groggytime_cur>groggytime)
	{
		Me->isGroggy=false;
		ChangeToTrackingState(Target);
	}
	else
	{
		groggytime_cur+=DeltaTime;
	}
}


void UEnemyFsm::ChangeToTrackingState(class AKZGCharacter* NewTarget)
{
	Target=NewTarget;	
	Me->AttachUI();
	mState=EEnemyState::Tracking;
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
	if (ai != nullptr)
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
}

void UEnemyFsm::ChangeToIdleState()
{
	Target=nullptr;
	SearchLoc=Me->GetActorLocation();
	GetRandomPosInNavMesh(SearchLoc, SearchDist, SearchDest);
	mState=EEnemyState::Idle;
}

void UEnemyFsm::ChangeToAttackState()
{
	attacktime_cur=0;
	Me->Stamina_Cur=Me->Stamina_Max;
	if (ai != nullptr)
	{
		ai->StopMovement();
	}
	UE_LOG(LogTemp, Warning, TEXT("GoAttackzz"));
	Target->GrabbedbyZombie(Me);
	mState=EEnemyState::Attack;
	Me->StatUI->EImageShow();
}

void UEnemyFsm::ChangeToGroggyState()
{
	Me->isGroggy=true;
	groggytime_cur=0;
	mState=EEnemyState::Groggy;
}

void UEnemyFsm::ChangeToRecognitionState(class AKZGCharacter* NewTarget)
{
	Target=NewTarget;
	if (ai != nullptr)
	{
		ai->StopMovement();
	}
	recognitiontime_cur=0;
	RecognitionLoc=Target->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Turnzz"));
	Me->SetActorRotation((Target->GetActorLocation()-Me->GetActorLocation()).Rotation().Quaternion());
	mState=EEnemyState::Recognition;
}

void UEnemyFsm::Recognition(class AKZGCharacter* NewTarget)
{
	if (mState==EEnemyState::Idle)
	{
		ChangeToRecognitionState(NewTarget);
	}
	else if(mState==EEnemyState::Recognition)
	{
		ChangeToTrackingState(NewTarget);
	}
}
