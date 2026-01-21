// Fill out your copyright notice in the Description page of Project Settings.


#include "Bedrock.h"

ABedrock::ABedrock()
{
	static ConstructorHelpers::FObjectFinder<UMaterial>BedrockMat(TEXT("/Game/Voxel/M_Bedrock_Mat"));			//Looks for valid material

	if (BedrockMat.Succeeded())																					//Sets the material if valid, gives an error if not.
	{
		BaseVoxelMesh->SetMaterial(0, BedrockMat.Object);
	}

	VoxelType = EVoxelType::BEDROCK;											//Sets the voxel type to bedorck

	XPValue = 0;																//Sets the XP value for bedrock voxel
}

int ABedrock::Interact()												//Interaction system for bedrock voxel. Does nothing as bedrock set to not be destoryed. Just returns the XP value which is 0.
{
	return XPValue;
}
