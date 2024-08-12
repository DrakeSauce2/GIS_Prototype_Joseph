// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/FightingTempCharacter.h"
#include "InputMappingContext.h" 
#include "FightingTempGameMode.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_Default UMETA(DisplayName = "Mocap"),
	VE_Mannequin UMETA(DisplayName = "Mannequin")
};

class AGCharacterBase;
/*
* 
*/
UCLASS(minimalapi)
class AFightingTempGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFightingTempGameMode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void SetPlayerControllerEnabled(AGCharacterBase* PlayerController, bool state);

	/*****************************************************/
	/*                       Timer                       */
	/*****************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float RoundStartTime = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float CountdownStartTime = 3.0f;

	FTimerHandle RoundTimerHandle;
	FTimerHandle CountdownTimerHandle;

	float RoundTimeRemaining = -1.0f;
	float CountdownTimeRemaining = -1.0f;

	void UpdateRoundTimer();
	void UpdateCountdownTimer();

	void StartRound();
	void EndRound();

	/*****************************************************/
	/*                      Spawning                     */
	/*****************************************************/
	void SpawnPlayers();
	AGCharacterBase* SpawnAndPossessCharacter(APlayerController* PlayerController, TSubclassOf<class AGCharacterBase> SelectedCharacterToSpawn, const FVector& SpawnLocation);

	AGCharacterBase* PlayerOne;
	AGCharacterBase* PlayerTwo;

	/*****************************************************/
	/*                      Widgets                      */
	/*****************************************************/
	void SpawnGameplayUI();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameplayUI> GameplayUIClass;

	UPROPERTY()
	UGameplayUI* GameplayUI;


};



