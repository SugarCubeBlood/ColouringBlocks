// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel.h"

// Sets default values
AVoxel::AVoxel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseVoxelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Voxel Mesh"));
	BaseVoxelMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Base Voxel Material"));

	RootComponent = BaseVoxelMesh;		//Sets the mesh as the root component for collision box

	static ConstructorHelpers::FObjectFinder<UStaticMesh>VoxelMesh(TEXT("/Game/Voxel/Mesh_Voxel"));
	static ConstructorHelpers::FObjectFinder<UMaterial>VoxelMaterial(TEXT("/Game/Voxel/M_Voxel_Mat"));			//Looks for mesh and material for voxel

	if (VoxelMesh.Succeeded())
	{
		BaseVoxelMesh->SetStaticMesh(VoxelMesh.Object);															//Sets mesh

		UStaticMesh* Mesh = VoxelMesh.Object;

		Mesh->BodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;		// Force complex collision on the mesh as for some reason this couldnt be done inside unreal.
		Mesh->BodySetup->bHasCookedCollisionData = true;
		Mesh->BodySetup->CreatePhysicsMeshes();


		BaseVoxelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);		//Sets up collision on voxel so they can bit hit by traces for and mining/placing blocks.
		BaseVoxelMesh->SetCollisionObjectType(ECC_WorldStatic);
		BaseVoxelMesh->SetCollisionResponseToAllChannels(ECR_Block);
		BaseVoxelMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not found voxel mesh"));		//Gives error if mesh cant be set
	}

	if (VoxelMaterial.Succeeded())
	{
		BaseVoxelMaterial = VoxelMaterial.Object;																//Sets material if valid, gives errror if not
		BaseVoxelMesh->SetMaterial(0, BaseVoxelMaterial);
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not found voxel material"));
	}
	
	
	VoxelType = EVoxelType::BASE_VOXEL;										//Sets the base voxel type from the Enum
	
	XPValue = 1;															//Sets the base XP value for voxel
}

void AVoxel::SetOwningChunk(AChunk* Chunk)		//Stores reference to the chunk the voxel belongs to.
{
	OwningChunk = Chunk;
}

AChunk* AVoxel::GetOwningChunk() const		//Resturns the chunk the voxel belongs to.
{
	return OwningChunk;
}

int AVoxel::Interact()				//Interaction system for the voxel when called upon when mining
{
	Destroy();

	return XPValue;
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

