// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	VoxelCount = 0;				//Sets initial voxel count to 0

	BaseChunkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Voxel Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChunkMesh(TEXT("/Game/Voxel/Mesh_Voxel"));
	BaseChunkMesh->SetStaticMesh(ChunkMesh.Object);															//Sets mesh for visual representations of chunk in editor.

	UStaticMesh* Mesh = ChunkMesh.Object;
}

float AChunk::GetVoxelMeasurement() const		//Getter for the base voxel size, used for placement of placed blocks in character
{
	return BaseVoxelMeasurement;
}

FVector AChunk::GetVoxelScale() const		//getter for the voxel scale inside the chunk, used to scale placed blocks in character
{
	return VoxelScale;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnParams.OverrideLevel = GetLevel();				//Ensures voxels spawn in same level as chunk

	SpawnLoc = GetActorLocation();					//Sets chunk starting location to spawn from based on in game actor.

	//bool switcher = true;			//No longer in use.

	VoxelScale = FVector(BaseVoxelMeasurement, BaseVoxelMeasurement, BaseVoxelMeasurement);		//Used to scale voxels inside the chunk

	for (int z = 0; z < CHUNK_Z; z++)						//Spawns voxels into a chunk on a 3d array loop using the X,Y and Z axis.
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int x = 0; x < CHUNK_X; x++)
			{
				int Seed = FMath::RandRange(1, 100);

				chunk[x][y][z] = (ABedrock*)GetWorld()->SpawnActor(ABedrock::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				chunk[x][y][z]->SetOwningChunk(this);		//Each of these assigns chunk ownership.

				//if (z == 0)		//Sets bedrock to alwasy be on bottom layer
				//{
				//	chunk[x][y][z] = (ABedrock*)GetWorld()->SpawnActor(ABedrock::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				//	chunk[x][y][z]->SetOwningChunk(this);		//Each of these assigns chunk ownership.
				//}
				//else if (Seed == 100)		//Allows diamond to spawn anywhere above bottom layer of bedrock
				//{
				//	chunk[x][y][z] = (ADiamond*)GetWorld()->SpawnActor(ADiamond::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				//	chunk[x][y][z]->SetOwningChunk(this);
				//}
				//else if (z >=1 && z <=4)		//Allows stone to spawn on layers 1 to 4
				//{
				//	chunk[x][y][z] = (ACobblestone*)GetWorld()->SpawnActor(ACobblestone::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				//	chunk[x][y][z]->SetOwningChunk(this);
				//}
				//else if (z >= 5 && z <=6)		// Allows dirt to spawn on layers 5 and 6
				//{
				//	chunk[x][y][z] = (ADirt*)GetWorld()->SpawnActor(ADirt::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				//	chunk[x][y][z]->SetOwningChunk(this);
				//}
				//else if(z == 7)		//Sets grass to spawn on the top layer
				//{
				//	Seed = 0;
				//	chunk[x][y][z] = (AGrass*)GetWorld()->SpawnActor(AGrass::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				//	chunk[x][y][z]->SetOwningChunk(this);
				//}
				

			/*	if (FMath::RandRange(0, 100) == 100)			No longer in use
				{
					chunk[x][y][z] = (ADiamond*)GetWorld()->SpawnActor(ADiamond::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
				}
				else
				{
					if (!switcher)
					{
						chunk[x][y][z] = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
					}
					else
					{
						chunk[x][y][z] = (ADirt*)GetWorld()->SpawnActor(ADirt::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);
					}

					
				}*/


				if (chunk[x][y][z]) chunk[x][y][z]->SetActorScale3D(VoxelScale);					//Applies the set scale for the voxels to the chunk, gives an error if unable to set the scale.
				else
				{
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Set Actor Scale Error"));
				}

				VoxelCount++;		//Added to voxel count for keeping track of chunk
				SpawnLoc.X += (100 * BaseVoxelMeasurement);		//moves position of next spawns voxel.
			}
			
			SpawnLoc.X -= (CHUNK_X * (100 * BaseVoxelMeasurement));
			SpawnLoc.Y += (100 * BaseVoxelMeasurement);
		}
		//switcher = false;			//No longer in use
		SpawnLoc.Y -= (CHUNK_Y * (100* BaseVoxelMeasurement));
		SpawnLoc.Z += (100 * BaseVoxelMeasurement);
	}
	//chunk[(FMath::RandRange(0, CHUNK_X - 1))][(FMath::RandRange(0, CHUNK_Y - 1))][(FMath::RandRange(0, CHUNK_Z - 1))] = (ADiamond*)GetWorld()->SpawnActor(ADiamond::StaticClass(), &SpawnLoc, &SpawnRot, SpawnParams);			//No longer in use
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Emerald, FString::Printf(TEXT("We have %d Voxel"),VoxelCount));		//Debug message. No longer in use

}

