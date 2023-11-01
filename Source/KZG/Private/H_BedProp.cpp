// Fill out your copyright notice in the Description page of Project Settings.


#include "H_BedProp.h"
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include "KZGCharacter.h"


AH_BedProp::AH_BedProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp =CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
	SphereComp->SetupAttachment(boxComp);
	SphereComp->SetSphereRadius(100);

}

void AH_BedProp::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AH_BedProp::OnComponentBeginOverlap);

}

// Called every frame
void AH_BedProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AH_BedProp::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKZGCharacter* player = Cast<AKZGCharacter>(OtherActor);

	if (player)
	{
		
	}
}
