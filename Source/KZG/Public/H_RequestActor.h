// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "H_RequestActor.generated.h"

UCLASS()
class KZG_API AH_RequestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AH_RequestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float currentTime = 9995;
	float currentTime1 = 0;
	bool bIsRequestSend = false;

	void SendRequest(const FString url);
	void OnReceivedData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void PostRequest(const FString url);
	void OnPostData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully);
};
