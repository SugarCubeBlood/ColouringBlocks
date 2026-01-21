// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction_Interface.h"
#include "ProjectEnums.h"
#include "Voxel.generated.h"


UCLASS()
class MINECRAFT_API AVoxel : public AActor, public IInteraction_Interface
{
	GENERATED_BODY()
	
	UPROPERTY()
	class AChunk* OwningChunk;		//Pointer to chunk the voxel belongs to.

public:	
	AVoxel();															// voxel constructor
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	EVoxelType VoxelType;												//Enable change of voxel type on the base voxel. This is so it can be visiable on unreal.

	void SetOwningChunk(AChunk* Chunk);		//Getter and Setter for the chunk the voxel belongs to.
	AChunk* GetOwningChunk() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")		//Enables a base static mesh for voxel
	class UStaticMeshComponent* BaseVoxelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")		//Enables base material for voxel
	class UMaterial* BaseVoxelMaterial;

	int XPValue;														//initalise/set XP Value for base voxels

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual int Interact() override;									//Sets up base interaction system for voxel

};
