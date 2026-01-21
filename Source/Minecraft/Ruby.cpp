// Fill out your copyright notice in the Description page of Project Settings.


#include "Ruby.h"

ARuby::ARuby()
{
	static ConstructorHelpers::FObjectFinder<UMaterial>RubyMat(TEXT("/Game/Voxel/M_Ruby_Mat"));								//Checks for valid material

	if (RubyMat.Succeeded())																									//Sets material if valid
	{
		BaseVoxelMesh->SetMaterial(0, RubyMat.Object);
	}

	VoxelType = EVoxelType::RUBY;																				//Sets voxel type to grass on enum

	XPValue = 1;																								//Sets xp value of grass
}
