// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskGameInstance.h"

void UTestTaskGameInstance::SetPlayerScore(int32 Score)
{
	PlayerScore = Score;

}

int32 UTestTaskGameInstance::GetPlayerScore()
{
	return PlayerScore;
}

