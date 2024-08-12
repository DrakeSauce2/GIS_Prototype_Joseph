// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyBaseGameInstance.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class UMyBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Selection")
	TSubclassOf<class AGCharacterBase> SelectedCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Controller")
	bool isDeviceForMultiplePlayers;
};
