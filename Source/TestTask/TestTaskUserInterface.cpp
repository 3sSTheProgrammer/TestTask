// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskUserInterface.h"

#include "TestTaskGameInstance.h"
#include "Components/TextBlock.h"

void UTestTaskUserInterface::IncreasePlayerScore()
{
	PlayerScore += 1;
	UpdatePlayerInterface();
	if (UTestTaskGameInstance* GameInstance = Cast<UTestTaskGameInstance>(GetGameInstance()))
	{
		GameInstance->SetPlayerScore(PlayerScore);
	}
}

void UTestTaskUserInterface::NativeConstruct()
{
	Super::NativeConstruct();

	if (UTestTaskGameInstance* GameInstance = Cast<UTestTaskGameInstance>(GetGameInstance()))
	{
		PlayerScore = GameInstance->GetPlayerScore();
	}
	UpdatePlayerInterface();
}

void UTestTaskUserInterface::UpdatePlayerInterface()
{
	TextBlockPlayerScore->SetText(FText::AsNumber(PlayerScore));
}
