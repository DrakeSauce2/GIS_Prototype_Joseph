// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ValueGauge.h"

#include "Components/ProgressBar.h"

void UValueGauge::NativeConstruct()
{
	Super::NativeConstruct();

	ProgressBar->SetFillColorAndOpacity(FillColor);
}

void UValueGauge::SetValue(float NewVal, float NewMaxValue)
{
	if (NewMaxValue == 0)
	{
		return;
	}

	float Percent = NewVal / NewMaxValue;
	ProgressBar->SetPercent(Percent);
}
