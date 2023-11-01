// Fill out your copyright notice in the Description page of Project Settings.


#include "H_RequestActor.h"
#include "HttpModule.h"
#include "ImageUtils.h"
#include "H_JsonParseLibrary.h"

// Sets default values
AH_RequestActor::AH_RequestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AH_RequestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AH_RequestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	if (currentTime > 5)
	{
		FString fullURL;
		fullURL = FString::Printf(TEXT("http://192.168.1.44:80"));

		UE_LOG(LogTemp, Warning, TEXT("Send Request!"));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *fullURL);


		FString num = "1";

		FString fullPath = fullURL + "/post" + "/" + num;
		UE_LOG(LogTemp, Warning, TEXT("Post Request!"));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *fullPath);

		SendRequest(fullURL);
		PostRequest(fullPath);
		currentTime = 0;
	}



	/*currentTime += DeltaTime;
	if (bIsRequestSend)
	{

		if (currentTime > 1)
		{
			bIsRequestSend = false;
			currentTime = 0;
		}
	}
	else
	{
		FString fullURL;
		fullURL = FString::Printf(TEXT("http://127.0.0.1:80/"));

		UE_LOG(LogTemp, Warning, TEXT("Send Request!"));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *fullURL);
		SendRequest(fullURL);

	}*/

}

void AH_RequestActor::SendRequest(const FString url)
{
	bIsRequestSend = true;
	// ����� �����ϰ� request �ν��Ͻ��� �����Ѵ�.
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedPtr<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�ϱ� ���� ������ �����Ѵ�.
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// ��û�� �Ϸ�Ǿ��� �� ����� �Լ��� ���ε��Ѵ�.
	//req->OnProcessRequestComplete().BindUFunction(this, FName("OnReceivedData"));
	req->OnProcessRequestComplete().BindUObject(this, &AH_RequestActor::OnReceivedData);

	// ������ ��û�� ������.
	req->ProcessRequest();
}

void AH_RequestActor::OnReceivedData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Warning, TEXT("received"));
	if (bConnectedSuccessfully)
	{
		FString res = Response->GetContentAsString();
		FString parsedData = UH_JsonParseLibrary::JsonParse(res);
		//gm->SetLogText(parsedData);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("%s"), *parsedData));
		UE_LOG(LogTemp, Warning, TEXT("received Value: %s"), *parsedData);

	}
	else
	{
		//gm->SetLogText("Failed Response from server...");
	}
}

void AH_RequestActor::PostRequest(const FString url)
{
	TMap<FString, FString> handsNumber;
	handsNumber.Add("Number", "0");

	FString myJsonData = UH_JsonParseLibrary::MakeJson(handsNumber);
	//gm->SetLogText(myJsonData);

	//��û ����
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	req->SetContentAsString(myJsonData);
	req->OnProcessRequestComplete().BindUObject(this, &AH_RequestActor::OnPostData);
	req->ProcessRequest();
}

void AH_RequestActor::OnPostData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		FString receivedData = Response->GetContentAsString();

		// ���� �����͸� ȭ�鿡 ����Ѵ�.
		//gm->SetLogText(receivedData);
		UE_LOG(LogTemp, Warning, TEXT("ReceivedData : %s"), *receivedData);

	}
	else
	{
		// ��û ���� ���� Ȯ��
		EHttpRequestStatus::Type status = Request->GetStatus();
		switch (status)
		{
		case EHttpRequestStatus::NotStarted:
			break;
		case EHttpRequestStatus::Processing:
			break;
		case EHttpRequestStatus::Failed:
			break;
		case EHttpRequestStatus::Failed_ConnectionError:
			break;
		case EHttpRequestStatus::Succeeded:
			break;
		default:
			break;
		}

		// ���� �ڵ� Ȯ��
		int32 responseCode = Response->GetResponseCode();
		//gm->SetLogText(FString::Printf(TEXT("Response Code: %d"), responseCode));

	}
}
