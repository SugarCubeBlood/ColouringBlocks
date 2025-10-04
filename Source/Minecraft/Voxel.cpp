// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel.h"

// Sets default values
AVoxel::AVoxel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseVoxelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Voxel Mesh"));
	BaseVoxelMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Base Voxel Material"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>VoxelMesh(TEXT("/Game/Voxel/Mesh_Voxel"));
	static ConstructorHelpers::FObjectFinder<UMaterial>VoxelMaterial(TEXT("/Game/Voxel/M_Voxel_Mat"));

	if (VoxelMesh.Succeeded())
	{
		BaseVoxelMesh->SetStaticMesh(VoxelMesh.Object);
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not found voxel mesh"));
	}

	if (VoxelMaterial.Succeeded())
	{
		BaseVoxelMaterial = VoxelMaterial.Object;
		BaseVoxelMesh->SetMaterial(0, BaseVoxelMaterial);
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not found voxel material"));
	}

}

// Called when the game starts or when spawned
void AVoxel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoxel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

