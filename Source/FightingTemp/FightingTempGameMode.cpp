// Copyright Epic Games, Inc. All Rights Reserved.

#include "FightingTempGameMode.h"

#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "MyBaseGameInstance.h"

#include "Player/GFighterCharacter.h"
#include "Player/GCharacterBase.h"

#include "TimerManager.h"

#include "UObject/ConstructorHelpers.h"

#include "Widgets/GameplayUI.h"

AFightingTempGameMode::AFightingTempGameMode()
{
	// Start players as spectators to prevent auto-spawning of the player controller
	bStartPlayersAsSpectators = true;

	PrimaryActorTick.bCanEverTick = true;
}

void AFightingTempGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnGameplayUI();

	SpawnPlayers();

	SetPlayerControllerEnabled(PlayerOne, false);
	SetPlayerControllerEnabled(PlayerTwo, false);

	CountdownTimeRemaining = CountdownStartTime;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AFightingTempGameMode::UpdateCountdownTimer, 1.0f, true);
}

void AFightingTempGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFightingTempGameMode::SetPlayerControllerEnabled(AGCharacterBase* Player, bool state)
{
	if (!Player) return;
	
	Player->SetInputEnabled(state);
}

#pragma region Gameplay Functions

void AFightingTempGameMode::UpdateRoundTimer()
{
	if (!GameplayUI) return;

	RoundTimeRemaining -= 1.0f;

	FText Text = FText::AsNumber(FMath::FloorToInt(RoundTimeRemaining));
	GameplayUI->SetTimerValue(Text);

	if (RoundTimeRemaining <= 0)
	{
		EndRound();
	}
}

void AFightingTempGameMode::UpdateCountdownTimer()
{
	if (!GameplayUI) return;

	CountdownTimeRemaining -= 1.0f;

	FText Text = FText::AsNumber(FMath::FloorToInt(CountdownTimeRemaining));
	GameplayUI->SetCountdownValue(Text);

	if (CountdownTimeRemaining <= 0.0f)
	{
		StartRound();

		// Enable Player Movement

		if(PlayerOne)
			SetPlayerControllerEnabled(PlayerOne, true);

		if(PlayerTwo)
			SetPlayerControllerEnabled(PlayerTwo, true);

		GameplayUI->SetCountdownValue(FText::FromString("FIGHT!"));
	}

	if (CountdownTimeRemaining <= -0.5f) 
	{
		GameplayUI->SetCountdownValue(FText::FromString(""));

		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

void AFightingTempGameMode::StartRound()
{
	if (GetWorldTimerManager().IsTimerActive(RoundTimerHandle)) return;

	RoundTimeRemaining = RoundStartTime;

	if (GameplayUI) 
	{
		FText Text = FText::AsNumber(FMath::FloorToInt(RoundTimeRemaining));
		GameplayUI->SetTimerValue(Text);
	}

	GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &AFightingTempGameMode::UpdateRoundTimer, 1.0f, true);

	// Do some other logic here like making sure the player is fully healed and start fight countdown
}

void AFightingTempGameMode::EndRound()
{
	if (GetWorldTimerManager().IsTimerActive(RoundTimerHandle)) 
	{
		GetWorldTimerManager().ClearTimer(RoundTimerHandle);
	}

	// Maybe wait for a bit for animations or fade-in/fade-out then reset for next round or match end
}

#pragma endregion

#pragma region Player Spawning Functions

void AFightingTempGameMode::SpawnPlayers()
{
	UMyBaseGameInstance* GameInstance = Cast<UMyBaseGameInstance>(GetGameInstance());

	if (!GameInstance || !GameInstance->SelectedCharacterClass) return;

	// We don't need to create a local player for player 1 as its created for us automatically
	APlayerController* PlayerOneController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerOneController)
	{
		FVector SpawnLocation = FVector(810.0f, 10.0f, 212.0f); // Adjust as needed

		// TODO: Implement character selection choice properly
		PlayerOne = SpawnAndPossessCharacter(PlayerOneController, GameInstance->SelectedCharacterClass, SpawnLocation);
	}

	APlayerController* PlayerTwoController = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	if (PlayerTwoController) // Assign Selected Character To This Local Player Instance
	{
		FVector SpawnLocation = FVector(1290.0f, 10.0f, 212.0f); // Adjust as needed

		// TODO: Implement character selection choice properly
		PlayerTwo = SpawnAndPossessCharacter(PlayerTwoController, GameInstance->SelectedCharacterClass, SpawnLocation);
	}
}

AGCharacterBase* AFightingTempGameMode::SpawnAndPossessCharacter(APlayerController* PlayerController, TSubclassOf<class AGCharacterBase> SelectedCharacterToSpawn, const FVector& SpawnLocation)
{
	UMyBaseGameInstance* GameInstance = Cast<UMyBaseGameInstance>(GetGameInstance());
	if (!GameInstance) return nullptr;

	if (!PlayerController) return nullptr;

	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
	if (!LocalPlayer) return nullptr;

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem) return nullptr;

	InputSubsystem->AddMappingContext(GameInstance->InputMapping, 0);

	PlayerController->SetInputMode(FInputModeGameOnly());

	AGCharacterBase* InstancedCharacter = GetWorld()->SpawnActor<AGCharacterBase>(SelectedCharacterToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	if (!InstancedCharacter) return nullptr;

	PlayerController->Possess(InstancedCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Player spawned and possessed successfully!"));

	return InstancedCharacter;
}

#pragma endregion

void AFightingTempGameMode::SpawnGameplayUI()
{
	if (!GameplayUIClass) return;

	GameplayUI = CreateWidget<UGameplayUI>(GetWorld(), GameplayUIClass);
	if (GameplayUI)
	{
		GameplayUI->AddToViewport();
	}
}

