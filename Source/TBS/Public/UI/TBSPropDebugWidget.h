// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TBSPropDebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API UTBSPropDebugWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetProps(TArray<FProp> InProps);

	UPROPERTY(BlueprintReadWrite)
	FString DebugString = FString(TEXT(""));	

private:
	TArray<FProp> Props;

	void UpdateDebugString();
};
