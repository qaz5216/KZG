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
#include "EnemyAnimInstance.h"
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
	Me->GetCharacterMovement()->bOrientRotationToMovement=true;
	Me->GetCharacterMovement()->RotationRate.Yaw=180;
	//Target=Cast<AKZGCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(),AKZGCharacter::StaticClass()));
	//mState=EEnemyState::Tracking;
	SearchLoc=Me->GetActorLocation();
	anim = Cast<UEnemyAnimInstance>(Me->GetMesh()->GetAnimInstance());
	GetRandomPosInNavMesh(SearchLoc, SearchDist, SearchDest);
	start=true;
	mState=StartState;
	anim->AnimState=mState;
	if (StartState==EEnemyState::DeepSleep)
	{
		bDeepSleep=true;
	}
	if (StartState == EEnemyState::Eating)
	{
		bEating = true;
	}
}


// Called every frame
void UEnemyFsm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	if (ai!=nullptr)
	{
		//UE_LOG(LogTemp,Warning,TEXT("AI NULL zz"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AI YEs zz"));
	}	

	if (start)
	{
		//시야는 계속작동하니까
		if (mState==EEnemyState::Damage||mState==EEnemyState::Groggy||mState==EEnemyState::Die||mState==EEnemyState::Sleep||mState==EEnemyState::kill || mState == EEnemyState::DeepSleep || mState == EEnemyState::Eating)//이때는 빼고
		{
		}
		else
		{
			Viewing();
		}
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
		TrackingState(DeltaTime);
		break;
	case EEnemyState::Recognition:
		RecognitionState(DeltaTime);
		break;
	case EEnemyState::Attack:
		AttackState(DeltaTime);
		break;
	case EEnemyState::Damage:
		DamageState(DeltaTime);
		break;
	case EEnemyState::Die:
		DieState(DeltaTime);
		break;
	case EEnemyState::DeepSleep:
		DeepSleepState();
		break;
	case EEnemyState::Eating:
		EatingState();
		break;
	case EEnemyState::Sleep:
		SleepState();
		break;
	case EEnemyState::Groggy:
		GroggyState(DeltaTime);
		break;
	case EEnemyState::kill:
		KillingState(DeltaTime);
	break; 
	default:
		break;
	}
	}
	
}

void UEnemyFsm::KillingState(float DeltaTime)
{
	if (killing)
	{
		return;
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
			//UE_LOG(LogTemp, Warning, TEXT("DestFix"));
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
		//UE_LOG(LogTemp, Warning, TEXT("Fail Loc"));
	}

	/*밤일때
		SleepState();
	*/
}

void UEnemyFsm::TrackingState(float DeltaTime)
{
	if (Target==nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("nullzz"));
		return;
		ChangeToIdleState();
	}
	bool TargetSee = SeeTarget(Target);
	if (TargetSee)
	{
		dest = Target->GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("See"));
		Trackingtime_cur=0;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("NoSee"));
	}
	if (FVector::Dist(Me->GetActorLocation(), dest) < 100.0f) {
		if (TargetSee) {
			//공격
			double DotP = FVector::DotProduct(Me->GetActorForwardVector(), Target->GetActorForwardVector());
			if (DotP > 0.5)
			{
				Target->AssasinationDeath();
				ai->StopMovement();
				anim->PlayKillAnim();
				FTimerHandle myTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						killingplay();
					}), 4.6, false); // 반복 실행을 하고 싶으면 false 대신 true 대입
				mState = EEnemyState::kill;
				anim->AnimState = mState;
				return;
			}
			else{
			if (Target->bIsgrabbed)
			{
				return;
			}
			Trackingtime_cur=0;
			ChangeToAttackState();
			return;
			}
		}
		else
		{
			Trackingtime_cur+=DeltaTime;
			if (Trackingtime_cur > Trackingtime)
			{
				ChangeToIdleState();
			}
			//대기후 아이들 놓침
			return;
		}

	}
	EPathFollowingRequestResult::Type isAlreadyGoal = EPathFollowingRequestResult::Failed;
	FPathFindingResult r;
	FindPathByAI(dest, r);
	if (r.Result == ENavigationQueryResult::Success) {
		if (ai != nullptr)
		{
			isAlreadyGoal = ai->MoveToLocation(dest);
			FailLocTime=FailLocMaxTime;
		}
	}
	else
	{	
		FailLocTime += DeltaTime;
		if (FailLocTime>FailLocMaxTime)
		{
			ChangeToIdleState();
			FailLocTime=0;
		}	
	}
}

void UEnemyFsm::RecognitionState(float DeltaTime)
{
	//5초안에 별일 없으면 다시 idle;
	if (recognitiontime_cur>recognitiontime)
	{
		if (PremState==EEnemyState::Idle)
		{
			ChangeToIdleState();
		}
		else if (PremState==EEnemyState::Sleep)
		{
			mState=EEnemyState::Sleep;
			anim->AnimState = mState;
		}
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
		if (Target==nullptr)
		{
			ChangeToIdleState();
			return;
		}
		if (attacktime_cur>attacktime)
		{
			Target->DamagedStamina(attackdamage);
			// player->damagedstamina(int32 value)
			//UE_LOG(LogTemp, Warning, TEXT("Attack remain Sta=%d"),Target->currentStamina);
			attacktime_cur=0;
			if (Target->currentStamina <= 0)
			{
				anim->PlayKillAnim();
				FTimerHandle myTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						killingplay();
					}),4.6 ,false); // 반복 실행을 하고 싶으면 false 대신 true 대입
				mState=EEnemyState::kill;
				anim->AnimState = mState;
			}
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

void UEnemyFsm::DamageState(float DeltaTime)
{
	damagetime_cur+=DeltaTime;
	if (damagetime_cur>damagetime)
	{
		if (Target!=nullptr)
		{
			ChangeToTrackingState(Target);
		}
		else
		{
			ChangeToIdleState();
		}
	}
}

void UEnemyFsm::DieState(float DeltaTime)
{
	dietime+=DeltaTime;
	if (dietime>6.5)
	{
		Me->Destroy();
	}
}

void UEnemyFsm::SleepState()
{
	//자는중
}

void UEnemyFsm::DeepSleepState()
{
	//딥슬립중
}

void UEnemyFsm::EatingState()
{
	//식사중
}

void UEnemyFsm::GroggyState(float DeltaTime)
{
	//UE_LOG(LogTemp,Warning,TEXT("%f,%f"),groggytime_cur,groggytime);
	if (groggytime_cur>groggytime)
	{
		Me->isGroggy=false;
		Me->StaminaHeal(Me->Stamina_Max);
		Me->StatUI->EImageHidden();
		anim->StopGroogyAnim();
		ChangeToTrackingState(Target);
	}
	else
	{
		groggytime_cur+=DeltaTime;
		if (!Me->isGroggy&&groggytime_cur>groggydelaytime)
		{
			Me->isGroggy=true;
		}
	}
}


void UEnemyFsm::killingplay()
{
	//killing=false;
	//ChangeToIdleState();
}

void UEnemyFsm::ChangeToTrackingState(class AKZGCharacter* NewTarget)
{
	if (mState == EEnemyState::Die)
	{
		return;
	}
	Me->GetCharacterMovement()->MaxWalkSpeed = runspeed;
	if (mState==EEnemyState::Tracking)
	{
		if (NewTarget != Target)
		{
			FVector TargetDist = Target->GetActorLocation() - Me->GetActorLocation();
			FVector NewTargetDist = NewTarget->GetActorLocation() - Me->GetActorLocation();
			if (NewTargetDist.Size() < TargetDist.Size())
			{
				Target=NewTarget;
				dest=Target->GetActorLocation();
				Trackingtime_cur=0;
				FailLocTime=0;
			}
		}
		else
		{	//소리내면
			dest = Target->GetActorLocation();
			Trackingtime_cur = 0;
			FailLocTime=0;
		}
	}
	else if (mState==EEnemyState::Idle)
	{
		Target = NewTarget;
		dest = Target->GetActorLocation();
		//Me->AttachUI();
		Trackingtime_cur=0;
		FailLocTime = 0;
		mState = EEnemyState::Tracking;
		anim->AnimState = mState;
	}
	else if(mState==EEnemyState::Recognition)
	{
		Target = NewTarget;
		dest = Target->GetActorLocation();
		//Me->AttachUI();
		Trackingtime_cur=0;
		FailLocTime = 0;
		anim->StopRecoAnim();
		mState = EEnemyState::Tracking;
		anim->AnimState = mState;
	}
	else if (mState==EEnemyState::Damage)
	{
		Target = NewTarget;
		dest = Target->GetActorLocation();
		//Me->AttachUI();
		Trackingtime_cur = 0;
		FailLocTime = 0;
		mState = EEnemyState::Tracking;
		anim->AnimState = mState;
	}
	else if (mState == EEnemyState::Eating)
	{
		Target = NewTarget;
		dest = Target->GetActorLocation();
		//Me->AttachUI();
		Trackingtime_cur = 0;
		FailLocTime = 0;
		mState = EEnemyState::Tracking;
		anim->AnimState = mState;
	}
	else if (mState == EEnemyState::DeepSleep)
	{
		Target = NewTarget;
		dest = Target->GetActorLocation();
		//Me->AttachUI();
		Trackingtime_cur = 0;
		FailLocTime = 0;
		mState = EEnemyState::Tracking;
		anim->AnimState = mState;
	}

	else if (mState==EEnemyState::Groggy)
	{
		Target = NewTarget;
		if (Target!=nullptr)
		{
			dest = Target->GetActorLocation();

		}
		//Me->AttachUI();
		Trackingtime_cur = 0;
		FailLocTime = 0;
		mState = EEnemyState::Tracking;
		anim->AnimState = mState;
	}
}

void UEnemyFsm::DeepSleepToTrackingState(class AKZGCharacter* NewTarget)
{
	if (bDeepSleep)
	{
		bDeepSleep=false;
		anim->PlayWakeUpAnim();
		FTimerHandle myTimerHandle;
		Target = NewTarget;
		GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				anim->StopWakeUpAnim();
				ChangeToTrackingState(Target);
			}), 2.4, false); // 반복 실행을 하고 싶으면 false 대신 true 대입
	}
}

void UEnemyFsm::EatingToTrackingState(class AKZGCharacter* NewTarget)
{
	if (bEating)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("EatingToTrack")));
		bEating=false;
		anim->PlayGetUpAnim();
		FTimerHandle myTimerHandle;
		Target = NewTarget;
		GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				
				anim->StopGetUpAnim();
				ChangeToTrackingState(Target);
			}), 2.5, false); // 반복 실행을 하고 싶으면 false 대신 true 대입
	}
}

void UEnemyFsm::ChangeToKillState()
{
	anim->PlayKillAnim();
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			killingplay();
		}), 4.6, false); // 반복 실행을 하고 싶으면 false 대신 true 대입
	mState = EEnemyState::kill;
	anim->AnimState = mState;
}

bool UEnemyFsm::GetRandomPosInNavMesh(FVector center, float radius, FVector& Rdest)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(center, radius, loc);
	Rdest = loc.Location;
	return true;
}

void UEnemyFsm::FindPathByAI(FVector destination, FPathFindingResult& result)
{
	if (ai != nullptr)
	{

		// navigation 시스템 세팅
		auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (ns!=nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("nsNull")));
		}
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
	if (mState == EEnemyState::Die)
	{
		return;
	}
	Target=nullptr;
	SearchLoc=Me->GetActorLocation();
	GetRandomPosInNavMesh(SearchLoc, SearchDist, SearchDest);
	Me->GetCharacterMovement()->MaxWalkSpeed = speed;
	mState=EEnemyState::Idle;
	anim->AnimState = mState;
}

void UEnemyFsm::ChangeToAttackState()
{
	if (mState == EEnemyState::Die)
	{
		return;
	}
	attacktime_cur=0;
	Me->Stamina_Cur=Me->Stamina_Max;
	if (ai != nullptr)
	{
		ai->StopMovement();
	}
	//UE_LOG(LogTemp, Warning, TEXT("GoAttackzz"));
	Target->GrabbedbyZombie(Me);
	mState=EEnemyState::Attack;
	anim->AnimState = mState;
	Me->StatUI->EImageShow();
}

void UEnemyFsm::ChangeToGroggyState()
{
	if (mState == EEnemyState::Die)
	{
		return;
	}
	groggytime_cur=0;
	anim->PlayGroogyAnim();
	mState=EEnemyState::Groggy;
	anim->AnimState = mState;
	Me->StatUI->EImageShow();
}

void UEnemyFsm::ChangeToRecognitionState(class AKZGCharacter* NewTarget)
{
	if (mState == EEnemyState::Die)
	{
		return;
	}
	Target=NewTarget;
	if (ai != nullptr)
	{
		ai->StopMovement();
	}
	recognitiontime_cur=0;
	RecognitionLoc=Target->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Turnzz"));
	Me->SetActorRotation((Target->GetActorLocation()-Me->GetActorLocation()).Rotation().Quaternion());
	mState=EEnemyState::Recognition;
	anim->AnimState = mState;
}

void UEnemyFsm::ChangeToDamageState()
{
	if (mState == EEnemyState::Die)
	{
		return;
	}
	//FString sectionName = FString::Printf(TEXT("Damage%d"), index);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("hizzzz")));
	//anim->PlayDamageAnim(FName(*sectionName));
	if (mState==EEnemyState::Damage)
	{
		damagetime_cur=0;
		anim->StopDamageAnim();
		anim->PlayDamageAnim();
	}
	else if (mState == EEnemyState::Groggy)
	{
		//그로기일때 맞으면 어떻게하죠?
		anim->StopDamageAnim();
		anim->PlayDamageAnim();
	}
	else
	{
		if (ai != nullptr)
		{
			ai->StopMovement();
		}
		damagetime_cur=0;
		PremState = mState;
		mState = EEnemyState::Damage;
		anim->AnimState = mState;
		anim->PlayDamageAnim();
	}
}

void UEnemyFsm::ChangeToDieState()
{	
	if(mState==EEnemyState::Die)
	return;
	mState=EEnemyState::Die;
	anim->AnimState = mState;
	anim->PlayDieAnim();
	Me->HP_Cur=0;
	dietime=0;
	Me->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void UEnemyFsm::ChangeToAssasinDieState()
{
	if (mState == EEnemyState::Die)
		return;
	mState = EEnemyState::Die;
	anim->AnimState = mState;
	anim->PlayAssaineDieAnim();
	bAssassinDie = true;
	anim->bADie=true;
	Me->HP_Cur = 0;
	dietime = 0;
	Me->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UEnemyFsm::ChangeToSleepState()
{
	if (mState == EEnemyState::Die)
	{
		return;
	}
	mState=EEnemyState::Sleep;
	anim->AnimState = mState;
}

void UEnemyFsm::Viewing()
{
	TArray<FOverlapResult> hitInfos;
	FVector MeLoc= Me->GetActorLocation();
	if (GetWorld()->OverlapMultiByProfile(hitInfos,	MeLoc , FQuat::Identity, FName("ViewPlayer"), FCollisionShape::MakeSphere(viewDistance)))
	{
		bool findCharacter=false;
		TArray<AKZGCharacter*> hitPlayers;
		//UE_LOG(LogTemp,Warning,TEXT("1"));
		for (const FOverlapResult& hitInfo : hitInfos)
		{	
			//UE_LOG(LogTemp, Warning, TEXT("2"));
			if (AKZGCharacter* hitPlayer = Cast<AKZGCharacter>(hitInfo.GetActor()))//이거 되긴하냐..?
			{	
				//UE_LOG(LogTemp, Warning, TEXT("3"));
				//시야에 들어가있냐?
				if (SeeTarget(hitPlayer)) //여기부터확인해 이게 안불려;;
				{
					///UE_LOG(LogTemp, Warning, TEXT("8"));
					hitPlayers.Add(hitPlayer);
					findCharacter = true;
				}
				//hitPlayers.Add(hitPlayer);
				//findCharacter=true;
			}
		}
		// 시야에 보인놈들중 가장 가까운놈 추적
		if (findCharacter)
		{
			float mindist=-1;
			AKZGCharacter* ShortPlayer=nullptr;
			for (AKZGCharacter* DistCharacter:hitPlayers)
			{	// 가까운놈 찾기
				if (mindist==-1)
				{
					ShortPlayer=DistCharacter;
					float checkdist=(DistCharacter->GetActorLocation()-MeLoc).Size();
					mindist=checkdist;
				}
				else
				{
					float checkdist = (DistCharacter->GetActorLocation() - MeLoc).Size();
					if (checkdist<mindist)
					{
						ShortPlayer = DistCharacter;
						mindist=checkdist;
					}
				}
			}
			if (ShortPlayer!=nullptr)
			{
				ChangeToTrackingState(ShortPlayer);
			}
		}
		else
		{
			//
		}
	}//DrawDebugSphere(GetWorld(), MeLoc, viewDistance, 30, FColor::Green, false, 1.0f);

}

bool UEnemyFsm::SeeTarget(class AKZGCharacter* TargetChar)
{
	FVector MeLoc = Me->GetActorLocation();
	FVector startloc = MeLoc;
	startloc.Z += 50.0f;//눈위치
	FVector endloc = TargetChar->GetMesh()->GetBoneLocation(FName("Head"));
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(Me);
	//UE_LOG(LogTemp, Warning, TEXT("4"));
	//DrawDebugLine(GetWorld(), startloc, endloc, FColor::Red, false, 1.0f);
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startloc, endloc, ECC_Visibility, collisionParams))
	{
		//UE_LOG(LogTemp, Warning, TEXT("5"));
		if (hitResult.GetActor())
		{
			//UE_LOG(LogTemp, Warning, TEXT("6"));
			if (AKZGCharacter* hitPlayer = Cast<AKZGCharacter>(hitResult.GetActor()))
			{
				auto* hitActor=hitResult.GetActor();
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hit Actor Name:%s"), *hitActor->GetName()));
				//UE_LOG(LogTemp, Warning, TEXT("7"));
				if (hitPlayer == TargetChar)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Findzz"));
					//시야각 안에있냐?
					FVector DirectionVec=hitPlayer->GetActorLocation()-Me->GetActorLocation();
					DirectionVec.Z=0;
					DirectionVec.Normalize();
					double DotP = FVector::DotProduct(Me->GetActorForwardVector(),DirectionVec);
					if (DotP>0.5) // 잘생각해봐
					{
						//UE_LOG(LogTemp, Warning, TEXT("SeeAngle"));
						return true;
					}
					else
					{
						//UE_LOG(LogTemp, Warning, TEXT("SeeAngle X"));
					}
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("NOFindzz"));
				}

			}
			//맞은액터 확인용
		}
	}
	return false;
}

void UEnemyFsm::Recognition(class AKZGCharacter* NewTarget)
{
	FPathFindingResult r;
	FindPathByAI(NewTarget->GetActorLocation(), r);
	if (r.Result != ENavigationQueryResult::Success) //갈수없는 위치의 소리중 
	{
		if ((NewTarget->GetActorLocation()-Me->GetActorLocation()).Size()>200)//2m이내면 감지
		{
			
		}
		else //아니면 감지안함
		{			
			return;
		}
	}
	//갈수있는 경우는 다감지
	if (mState == EEnemyState::Idle || mState == EEnemyState::Sleep)
	{
		PremState = mState;
		anim->PlayRecoAnim();
		ChangeToRecognitionState(NewTarget);
	}
	else if (mState == EEnemyState::Recognition)
	{
		if (NewTarget != Target)
		{
			anim->PlayRecoAnim();
			ChangeToRecognitionState(NewTarget);
		}
		else
		{
			anim->StopRecoAnim();
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Stopp!")));
			ChangeToTrackingState(NewTarget);
		}
	}
	else if (mState == EEnemyState::Tracking)
	{
		ChangeToTrackingState(NewTarget);
	}

	/*if (mState==EEnemyState::DeepSleep)
	{
		DeepSleepToTrackingState(NewTarget);
	}
	if (mState == EEnemyState::Eating)
	{
		EatingToTrackingState(NewTarget);
	}*/
}
