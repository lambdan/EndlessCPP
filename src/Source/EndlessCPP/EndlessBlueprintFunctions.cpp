// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessBlueprintFunctions.h"

TArray<int64> UEndlessBlueprintFunctions::TopScores(const TArray<int64> ScoresIn, const int TopAmount)
{
	TArray<int64> SortedArray;
	SortedArray.Add(0);

	for(auto & a : ScoresIn)
	{
		for(int i = 0; i < SortedArray.Num(); i++)
		{
			if(a > SortedArray[i])
			{
				SortedArray.Insert(a, i);
				break;
			}
		}
	}

	SortedArray.Remove(0);
	
	// avoid returning 3 scores when there's only 1 etc.
	int AmountToReturn = FMath::Min(SortedArray.Num(), TopAmount);

	TArray<int64> ResultArray;
	for(int i = 0; i < AmountToReturn; i++)
	{
		ResultArray.Add(SortedArray[i]);
	}

	return ResultArray;
	
}
