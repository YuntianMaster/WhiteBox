// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DamagePropsWidget.h"
#include "Components/ProgressBar.h"

void UDamagePropsWidget::SetPercertage(float Percentage)
{
	ProgressBar_Main->SetPercent(Percentage);
}
