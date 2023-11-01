// Fill out your copyright notice in the Description page of Project Settings.


#include "H_FoodActor.h"
#include <Components/SphereComponent.h>
#include "KZGCharacter.h"

// Sets default values
AH_FoodActor::AH_FoodActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
	RootComponent = SphereComp;
	SphereComp->SetSphereRadius(100);
	SphereComp->SetCollisionProfileName(TEXT("Food"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AH_FoodActor::BeginPlay()
{
	Super::BeginPlay();
	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AH_FoodActor::OnComponentBeginOverlap);

	FTimerHandle myhandle;
	GetWorldTimerManager().SetTimer(myhandle, this, &AH_FoodActor::DestroySelf, 5.0f, false);

}

// Called every frame
void AH_FoodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AH_FoodActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (AKZGCharacter* player = Cast<AKZGCharacter>(OtherActor))
//	{
//		player->curHungerP += 30;
//		Destroy();
//	}
//}

void AH_FoodActor::DestroySelf()
{
	Destroy();
}

