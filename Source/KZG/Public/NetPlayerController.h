// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API ANetPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	class ABattleGameModeBase* gm;
	
	UPROPERTY(Replicated)
	bool bishost=false;

	class APawn* respawnplayer;
	UFUNCTION(Server, Unreliable)
	void ServerRespawnPlayer();

	UFUNCTION(NetMulticast, Reliable)
	void MultiChangePlayerToSpectator();

	UFUNCTION(Server, Reliable)
	void ServerChangePlayerToSpectator();

	UFUNCTION(Server, Reliable)
	void ServerChangePlayerHidden();

	UFUNCTION(NetMulticast, Reliable)
	void MultiChangePlayerHidden(class APawn* HiddenPlayer);


	UFUNCTION(Server, Reliable)
	void ServerChangePlayerToTracer();

	UFUNCTION(NetMulticast, Reliable)
	void MultiChangePlayerToTracer();

	UFUNCTION(Server, Unreliable)
	void ServerChangePlayerToGenji();

	UFUNCTION(NetMulticast, Reliable)
	void MultiChangePlayerToGenji();

	UFUNCTION(NetMulticast,Reliable)
	void MultiRespawnPlayer();

	UFUNCTION(NetMulticast,Reliable)
	void ChangeUIGameStart(int32 index);
	UFUNCTION(NetMulticast,Reliable)
	void ChangeUIGameEnd();


	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Character)
	TSubclassOf<class AKZGCharacter> BPPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Character)
	TSubclassOf<class ASpectatorPawn> Spec;

};
