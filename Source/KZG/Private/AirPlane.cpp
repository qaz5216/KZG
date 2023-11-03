// Fill out your copyright notice in the Description page of Project Settings.


#include "AirPlane.h"
#include "DropProp.h"

// Sets default values
AAirPlane::AAirPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAirPlane::BeginPlay()
{
	Super::BeginPlay();

	StartLoc=GetActorLocation();
}

// Called every frame
void AAirPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector P0 = GetActorLocation();
	if (bisdrop)
	{	
		FVector direction = EndLoc - P0;
		direction.Normalize();
		FVector vt = direction * Speed * DeltaTime;
		FVector P = P0 + vt;
		if (FVector::Dist(P, EndLoc) < 300.0f)
		{
			Destroy();
		}
		SetActorLocation(P);
	}
	else
	{	//¶³±¼ÁöÁ¡À¸·Î °¡¶ó
		FVector direction=DropLoc-P0;
		direction.Normalize();
		FVector vt=direction*Speed*DeltaTime;
		FVector P=P0+vt;
		if (FVector::Dist(P, DropLoc) < 300.0f)
		{
			drop();
		}
		SetActorLocation(P);
	}
}

void AAirPlane::drop()
{
	bisdrop=true;
	FVector Lendloc = DropLoc+FVector::DownVector*100000;
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), DropLoc, Lendloc, FColor::Red, false, 1.0f);
	if (GetWorld()->LineTraceSingleByChannel(hitResult, DropLoc, Lendloc, ECC_Visibility, collisionParams))
	{
		//UE_LOG(LogTemp, Warning, TEXT("5"));
		if (hitResult.GetActor())
		{
			ADropProp* Myprop = GetWorld()->SpawnActor<ADropProp>(prop,DropLoc,FRotator(0,0,0));
			Myprop->SpawnLoc=hitResult.ImpactPoint;
			UE_LOG(LogTemp, Warning, TEXT("gogo Dropzz%f,&%f"), DropLoc.Z, hitResult.ImpactPoint);
		}
	}
}

