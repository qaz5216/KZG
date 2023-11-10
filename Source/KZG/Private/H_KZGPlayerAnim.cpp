// Fill out your copyright notice in the Description page of Project Settings.


#include "H_KZGPlayerAnim.h"
#include "KZGCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include <Components/BoxComponent.h>


void UH_KZGPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto ownerPawn = TryGetPawnOwner();

	player = Cast<AKZGCharacter>(ownerPawn);

	if (player)
	{
		FVector forward = player->GetActorForwardVector();
		FVector velocity = player->GetVelocity();
		speed = FVector::DotProduct(forward, velocity);

		FVector right = player->GetActorRightVector();
		direction = FVector::DotProduct(right, velocity);

		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UH_KZGPlayerAnim::AnimNotify_AttackEnd1()
{
	if (player) 
	{
		player->bIsAttacking = false;
		player->currentStamina--;
		player->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void UH_KZGPlayerAnim::AnimNotify_AttackEnd2()
{
	player->bIsFinalAttackEnded = true;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(player->ZFinalBase);

}

void UH_KZGPlayerAnim::AnimNotify_AttackEnd3()
{
	player->bIsFinalAttackEnded = false;
}

void UH_KZGPlayerAnim::AnimNotify_SoundStep()
{
	if(player) player->PlayStepSoundPlaying();
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("stepSound")));
}

void UH_KZGPlayerAnim::AnimNotify_AssasinationEnd()
{
	if (player)
	{
		//player->bCanAssasination = false;
		player->bStartAssaination = false;
	}
}

void UH_KZGPlayerAnim::PlayAttackAnimation2()
{
	Montage_Play(attackMontage2);
}

void UH_KZGPlayerAnim::PlayAttackAnimation3()
{
	Montage_Play(attackMontage3);

}

void UH_KZGPlayerAnim::PlayDeathAnimation()
{
	Montage_Play(deathMontage);

}

void UH_KZGPlayerAnim::finalAttackAnimation3()
{
	Montage_Play(finalAttackMontage);
}

void UH_KZGPlayerAnim::playAssasinationAnimation()
{
	Montage_Play(assasinationMontage);
}
