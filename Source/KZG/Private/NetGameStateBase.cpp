// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameStateBase.h"
#include "GameFramework/PlayerState.h"
#include <Net/UnrealNetwork.h>


ANetGameStateBase::ANetGameStateBase()
{
	bReplicates = true;
}

TArray<APlayerState*> ANetGameStateBase::GetPlayerArrayByScore()
{
	TArray<APlayerState*> sortedPlayers = PlayerArray;

	sortedPlayers.Sort([](const APlayerState& p1, const APlayerState& p2) { return p1.GetScore() > p2.GetScore(); });
	return sortedPlayers;
}

void ANetGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetGameStateBase, bGameStart);
}