// Fill out your copyright notice in the Description page of Project Settings.


#include "Diamond.h"

ADiamond::ADiamond()
{

	static ConstructorHelpers::FObjectFinder<UMaterial>DiamondMat(TEXT("/Game/Voxel/M_Diamond_Mat"));			//Checks for valid material

	if (DiamondMat.Succeeded())																					//Sets material if valid
	{
		BaseVoxelMesh->SetMaterial(0, DiamondMat.Object);
	}

	VoxelType = EVoxelType::DIAMOND;												//Sets voxel type to diamond on enum

	XPValue = 25;																//Sets xp value of diamond
}
