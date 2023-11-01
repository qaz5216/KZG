// Fill out your copyright notice in the Description page of Project Settings.


#include "H_JsonParseLibrary.h"
#include "Json.h"

FString UH_JsonParseLibrary::JsonParse(const FString& originData)
{
	FString parsedData;

	// Reader를 만든다.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);

	// reader로 파싱된 결과를 담을 json Object를 선언한다.
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	// 해독한다.
	if (FJsonSerializer::Deserialize(reader, result))
	{
		TArray<TSharedPtr<FJsonValue>> parsedDataArr = result->GetArrayField("items");

		for (TSharedPtr<FJsonValue> pd : parsedDataArr)
		{
			FString rightNumber = pd->AsObject()->GetStringField("Number");
			parsedData = rightNumber;
			//parsedData.Append(FString::Printf(TEXT("rightNumber: %s\n"), *rightNumber));
		}
	}

	return parsedData;
}

FString UH_JsonParseLibrary::MakeJson(const TMap<FString, FString> source)
{
	// json object를 생성하고 값을 넣는다.
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> kv : source)
	{
		jsonObj->SetStringField(kv.Key, kv.Value);
	}

	// Writer를 생성하고, json Object를 인코딩한다.
	FString jsonData;

	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&jsonData);
	FJsonSerializer::Serialize(jsonObj.ToSharedRef(), writer);

	return jsonData;
}
