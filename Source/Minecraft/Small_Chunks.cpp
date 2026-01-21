// Fill out your copyright notice in the Description page of Project Settings.


#include "Small_Chunks.h"

// Sets default values
ASmall_Chunks::ASmall_Chunks()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VoxelCount = 0;				//Sets initial voxel count to 0

	BaseChunkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Voxel Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChunkMesh(TEXT("/Game/Voxel/Mesh_Voxel"));
	BaseChunkMesh->SetStaticMesh(ChunkMesh.Object);															//Sets mesh for visual representations of chunk in editor.

	UStaticMesh* Mesh = ChunkMesh.Object;
}

float ASmall_Chunks::GetVoxelMeasurement() const		//Getter for the base voxel size, used for placement of placed blocks in character
{
	return BaseVoxelMeasurement;
}

FVector ASmall_Chunks::GetVoxelScale() const		//getter for the voxel scale inside the chunk, used to scale placed blocks in character
{
	return VoxelScale;
}

// Called when the game starts or when spawned
void ASmall_Chunks::BeginPlay()
{
	Super::BeginPlay();
	SpawnParams.OverrideLevel = GetLevel();				//Ensures voxels spawn in same level as chunk

	SpawnLoc = GetActorLocation();					//Sets chunk starting location to spawn from based on in game actor.

	VoxelScale = FVector(BaseVoxelMeasurement, BaseVoxelMeasurement, BaseVoxelMeasurement);		//Used to scale voxels inside the chunk

	for (int z = 0; z < CHUNK_Z; z++)						//Spawns voxels into a chunk on a 3d array loop using the X,Y and Z axis.
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int x = 0; x < CHUNK_X; x++)
			{

				AVoxel* SpawnedVoxel = GetWorld()->SpawnActor<AVoxel>(VoxelToSpawn,SpawnLoc,SpawnRot,SpawnParams);

				if (SpawnedVoxel)
				{
					SpawnedVoxel->SetActorScale3D(VoxelScale);
				}

				else
				{
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Voxel spawn failed"));
				}

				chunk[x][y][z] = SpawnedVoxel;

				VoxelCount++;		//Added to voxel count for keeping track of chunk
				SpawnLoc.X += (100 * BaseVoxelMeasurement);		//moves position of next spawns voxel.
			}

			SpawnLoc.X -= (CHUNK_X * (100 * BaseVoxelMeasurement));
			SpawnLoc.Y += (100 * BaseVoxelMeasurement);
		}
	
		SpawnLoc.Y -= (CHUNK_Y * (100 * BaseVoxelMeasurement));
		SpawnLoc.Z += (100 * BaseVoxelMeasurement);
	}

}

// Called every frame
void ASmall_Chunks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

