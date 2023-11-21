// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AxeWeapon.h"

AH_AxeWeapon::AH_AxeWeapon()
{
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxeComp"));
	meshComp->SetupAttachment(RootComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>Tempbat(TEXT("/Script/Engine.StaticMesh'/Game/Props_MeleeWeapons/Meshes/fireAxe.fireAxe'"));

	if (Tempbat.Succeeded())
	{
		meshComp->SetStaticMesh(Tempbat.Object);
		meshComp->SetRelativeLocationAndRotation(FVector(-30.000000, 0.000000, 0.000000), FRotator(-90.000000, 0.000000, 0.000000));
		meshComp->SetSimulatePhysics(false);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}