// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Voxel.h"
#include "Cobblestone.h"
#include "Chunk.generated.h"

//#define CHUNK_X 3
constexpr int CHUNK_X = 4;
constexpr int CHUNK_Y = 4;
constexpr int CHUNK_Z = 4;

UCLASS()
class MINECRAFT_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	int VoxelCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk");
	float BaseVoxelMeasurement = 1.f;

	AVoxel* chunk[CHUNK_X][CHUNK_Y][CHUNK_Z];

	FVector SpawnLoc;
	FRotator SpawnRot;
	FActorSpawnParameters SpawnParams;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
