// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VoxelCount = 0;

}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnParams.OverrideLevel = GetLevel();

	bool switcher = true;

	FVector VoxelScale = FVector(BaseVoxelMeasurement, BaseVoxelMeasurement, BaseVoxelMeasurement);

	for (int z = 0; z < CHUNK_Z; z++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int x = 0; x < CHUNK_X; x++)
			{
				if (!switcher)
				{
					chunk[x][y][z] = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				}
				else
				{
					chunk[x][y][z] = (ACobblestone*)GetWorld()->SpawnActor(ACobblestone::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				}

				if (chunk[x][y][z]) chunk[x][y][z]->SetActorScale3D(VoxelScale);
				else 
				{ 
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Set Actor Scale Error")); 
				}
				
				VoxelCount++;
				SpawnLoc.X += (100 * BaseVoxelMeasurement);
			}
			
			SpawnLoc.X -= (CHUNK_X * (100 * BaseVoxelMeasurement));
			SpawnLoc.Y += (100 * BaseVoxelMeasurement);
		}
		switcher = false;
		SpawnLoc.Y -= (CHUNK_Y * (100* BaseVoxelMeasurement));
		SpawnLoc.Z += (100 * BaseVoxelMeasurement);
	}
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Emerald, FString::Printf(TEXT("We have %d Voxel"),VoxelCount));

}

