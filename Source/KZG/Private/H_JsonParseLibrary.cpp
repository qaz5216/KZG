// Fill out your copyright notice in the Description page of Project Settings.


#include "H_JsonParseLibrary.h"
#include "Json.h"

FString UH_JsonParseLibrary::JsonParse(const FString& originData)
{
	FString parsedData;

	// Reader�� �����.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);

	// reader�� �Ľ̵� ����� ���� json Object�� �����Ѵ�.
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	// �ص��Ѵ�.
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
	// json object�� �����ϰ� ���� �ִ´�.
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> kv : source)
	{
		jsonObj->SetStringField(kv.Key, kv.Value);
	}

	// Writer�� �����ϰ�, json Object�� ���ڵ��Ѵ�.
	FString jsonData;

	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&jsonData);
	FJsonSerializer::Serialize(jsonObj.ToSharedRef(), writer);

	return jsonData;
}
