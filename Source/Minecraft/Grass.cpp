// Fill out your copyright notice in the Description page of Project Settings.


#include "Grass.h"

AGrass::AGrass()
{
	static ConstructorHelpers::FObjectFinder<UMaterial>GrassMat(TEXT("/Game/Voxel/M_Grass_Mat"));								//Checks for valid material

	if (GrassMat.Succeeded())																									//Sets material if valid
	{
		BaseVoxelMesh->SetMaterial(0, GrassMat.Object);
	}

	VoxelType = EVoxelType::GRASS;																				//Sets voxel type to grass on enum

	XPValue = 1;																								//Sets xp value of grass
}
