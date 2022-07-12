// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestTaskUserInterface.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TESTTASK_API UTestTaskUserInterface : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	int32 PlayerScore{ 0 };
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockPlayerScore;

public:
	void IncreasePlayerScore();

protected:
	void UpdatePlayerInterface();
};
