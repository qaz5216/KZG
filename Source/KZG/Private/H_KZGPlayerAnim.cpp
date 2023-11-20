// Fill out your copyright notice in the Description page of Project Settings.


#include "H_KZGPlayerAnim.h"
#include "KZGCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>


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
		//player->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	if(player) 
	{
		player->PlayStepSoundPlaying();
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("stepSound")));
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), stepSound, player->GetActorLocation(), FRotator(), 0.4f);
	}
}

void UH_KZGPlayerAnim::AnimNotify_AssasinationEnd()
{
	if (player)
	{
		player->bCanAssasination = false;
		player->bStartAssaination = false;
	}
}

void UH_KZGPlayerAnim::AnimNotify_Combo1()
{	
	player->comboIndex = 1;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Combo1")));

}

void UH_KZGPlayerAnim::AnimNotify_Combo2()
{
	player->comboIndex = 2;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Combo2")));

}

void UH_KZGPlayerAnim::AnimNotify_Combo3()
{
	player->comboIndex = 3;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Combo3")));

}

void UH_KZGPlayerAnim::AnimNotify_Combo1End()
{
	player->bComboTime = true;
	player->bIsAttacking = false;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("ComboEnd1")));

}

void UH_KZGPlayerAnim::AnimNotify_Combo2End()
{
	player->bComboTime = true;
	player->bIsAttacking = false;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("ComboEnd2")));

}

void UH_KZGPlayerAnim::AnimNotify_Combo3End()
{
	player->bComboTime = true;
	player->bIsAttacking = false;
	player->comboIndex = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("ComboEnd3")));


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

void UH_KZGPlayerAnim::PlayComboAnimation1()
{
	Montage_Play(comboMontage1);
}

void UH_KZGPlayerAnim::PlayComboAnimation2()
{
	Montage_Play(comboMontage2);

}

void UH_KZGPlayerAnim::PlayComboAnimation3()
{
	Montage_Play(comboMontage3);

}

void UH_KZGPlayerAnim::finalAttackAnimation3()
{
	Montage_Play(finalAttackMontage);
}

void UH_KZGPlayerAnim::playAssasinationAnimation()
{
	Montage_Play(assasinationMontage);
}

void UH_KZGPlayerAnim::PlayDeathGrabMontage()
{
	Montage_Play(deathGrabMontage);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("DeathGrab")));

}

void UH_KZGPlayerAnim::playAssasinationDeathAnimation()
{
	Montage_Play(assasinationDeathMontage);
}

void UH_KZGPlayerAnim::playPushAnimation()
{
	Montage_Play(pushMontage);
}
