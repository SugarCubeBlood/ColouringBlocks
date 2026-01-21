// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Voxel.h"
#include "Cobblestone.h"
#include "Diamond.h"
#include "Bedrock.h"
#include "Dirt.h"
#include "Grass.h"
#include "Small_Chunks.generated.h"

constexpr int CHUNK_X = 6;		//Chunk size along each axis
constexpr int CHUNK_Y = 6;
constexpr int CHUNK_Z = 6;

UCLASS()
class MINECRAFT_API ASmall_Chunks : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmall_Chunks();

	UPROPERTY(BlueprintReadOnly)		//Stores scale of voxels for the Chunk
		FVector VoxelScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVoxel> VoxelToSpawn;

	float GetVoxelMeasurement() const;		// Getter for voxel measurment
	FVector GetVoxelScale() const;		//Getter for voxel scale

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")		//Enables a base static mesh for voxel
		class UStaticMeshComponent* BaseChunkMesh;

	int VoxelCount;																		//intialises voxel count for chunk to keep track of voxel amount

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk");					//Sets up base voxel measurement for chunk and to allow scable chunk sizes inside unreal
	float BaseVoxelMeasurement = 1.f;

	AVoxel* chunk[CHUNK_X][CHUNK_Y][CHUNK_Z];											//3D arracy for chunk

	FVector SpawnLoc;																	//Spawn Parameters for voxels when making chunk
	FRotator SpawnRot;
	FActorSpawnParameters SpawnParams;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
