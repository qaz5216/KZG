// Fill out your copyright notice in the Description page of Project Settings.


#include "H_BatWeapon.h"


AH_BatWeapon::AH_BatWeapon()
{
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("batComp"));
	meshComp->SetupAttachment(RootComp);
	

	ConstructorHelpers::FObjectFinder<UStaticMesh>Tempbat(TEXT("/Script/Engine.StaticMesh'/Game/Props_MeleeWeapons/Meshes/baseballBat.baseballBat'"));

	if (Tempbat.Succeeded())
	{
		meshComp->SetStaticMesh(Tempbat.Object);
		meshComp->SetRelativeLocationAndRotation(FVector(-30.000000, 0.000000, 0.000000), FRotator(-90.000000, 0.000000, 0.000000));
		meshComp->SetSimulatePhysics(false);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}
