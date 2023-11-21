// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AttackWeapons.h"
#include <Components/BoxComponent.h>
#include "../Enemy.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "KZGCharacter.h"

// Sets default values
AH_AttackWeapons::AH_AttackWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComp->SetupAttachment(RootComp);
	boxComp->SetBoxExtent(FVector(70, 10, 10));
	boxComp->SetCollisionProfileName(TEXT("Weapon"));
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetupAttachment(boxComp);
	sphereComp->SetSphereRadius(30);
	sphereComp->SetCollisionProfileName(TEXT("WeaponDrop"));

	bReplicates = true;

}

// Called when the game starts or when spawned
void AH_AttackWeapons::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AH_AttackWeapons::OnComponentBeginOverlap);
	
	
}

// Called every frame
void AH_AttackWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AH_AttackWeapons::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Zombie = Cast<AEnemy>(OtherActor);
	//Zombie->FSM->Target = this;
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("Overlap")));


	if (OtherActor == Zombie)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), batHitSound);
		Zombie->Damaged(zombieDamage);
		WeaponHP -= weaponDamage;
		boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


