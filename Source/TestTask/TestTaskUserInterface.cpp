// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskUserInterface.h"

#include "Components/TextBlock.h"

void UTestTaskUserInterface::IncreasePlayerScore()
{
	PlayerScore += 1;
	UpdatePlayerInterface();
}

void UTestTaskUserInterface::UpdatePlayerInterface()
{
	TextBlockPlayerScore->SetText(FText::AsNumber(PlayerScore));
}
