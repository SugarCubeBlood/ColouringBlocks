// Fill out your copyright notice in the Description page of Project Settings.


#include "Cobblestone.h"

ACobblestone::ACobblestone()
{

	static ConstructorHelpers::FObjectFinder<UMaterial>CobblestoneMat(TEXT("/Game/Voxel/M_Cobblestone_Mat"));				//Checks for valid material
	
	if (CobblestoneMat.Succeeded())																							//Sets material if valid
	{
		BaseVoxelMesh->SetMaterial(0, CobblestoneMat.Object);
	}

	VoxelType = EVoxelType::COBBLESTONE;									//Sets voxel type to cobblestone based on enum

	XPValue = 10;															//Sets xp value of cobblestone
}