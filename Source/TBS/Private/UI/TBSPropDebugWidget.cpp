// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSPropDebugWidget.h"

void UTBSPropDebugWidget::SetProps(TArray<FProp> InProps)
{
	Props = InProps;
	UpdateDebugString();
}

void UTBSPropDebugWidget::UpdateDebugString()
{
	DebugString = FString::Printf(TEXT("%i Props"), Props.Num());

	for (auto& Prop : Props)
	{
		DebugString += FString::Printf(TEXT("\n#%i: Coord: (%i, %i, %i), Dim (%i, %i, %i), Rot %i"),
			Prop.Id,
			Prop.Coordinates.X,
			Prop.Coordinates.Y,
			Prop.Coordinates.Z,
			Prop.Dimensions.X,
			Prop.Dimensions.Y,
			Prop.Dimensions.Z,
			Prop.Rotation
		);
	}
}

