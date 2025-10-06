// Fill out your copyright notice in the Description page of Project Settings.


#include "Cobblestone.h"

ACobblestone::ACobblestone()
{

	static ConstructorHelpers::FObjectFinder<UMaterial>CobblestoneMat(TEXT("/Game/Voxel/M_Cobblestone_Mat"));
	
	if (CobblestoneMat.Succeeded())
	{
		BaseVoxelMesh->SetMaterial(0, CobblestoneMat.Object);
	}
}