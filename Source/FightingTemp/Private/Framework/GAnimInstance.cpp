// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FightingTemp/Private/Player/FightingTempCharacter.h"

void UGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMovemmentComp = OwnerCharacter->GetCharacterMovement();
	}

	FightingTempCharacter = Cast<AFightingTempCharacter>(TryGetPawnOwner());
}

void UGAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter && OwnerMovemmentComp)
	{
		//bIsFlipped = FightingTempCharacter->IsFlipped();
		Speed = OwnerCharacter->GetVelocity().Length();
		bIsJumping = OwnerMovemmentComp->IsFalling();
	}
}
