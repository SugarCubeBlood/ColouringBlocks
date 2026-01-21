// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel.h"
#include "Bedrock.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API ABedrock : public AVoxel
{
	GENERATED_BODY()
	
private:


public:
	ABedrock();						//Constructor for bedrock voxel

	virtual int Interact() override;		//Overide for interaction system on base voxel as bedrock is different to other voxels, doesnt get destroyed etc.

};
