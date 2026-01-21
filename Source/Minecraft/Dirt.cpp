// Fill out your copyright notice in the Description page of Project Settings.


#include "Dirt.h"

ADirt::ADirt()
{
	static ConstructorHelpers::FObjectFinder<UMaterial>DirtMat(TEXT("/Game/Voxel/M_Dirt_Mat"));			//Checks for valid material

	if (DirtMat.Succeeded())																			//Sets material if valid
	{
		BaseVoxelMesh->SetMaterial(0, DirtMat.Object);
	}

	VoxelType = EVoxelType::DIRT;															//Sets voxel type to dirt on enum

	XPValue = 1;																			//Sets xp value of dirt
}
