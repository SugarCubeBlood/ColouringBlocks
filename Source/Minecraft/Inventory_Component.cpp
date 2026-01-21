// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Component.h"

// Sets default values for this component's properties
UInventory_Component::UInventory_Component()		//Contructor, no additional added, just base UE code.
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	GrassClass = AGrass::StaticClass();
	DirtClass = ADirt::StaticClass();
	CobblestoneClass = ACobblestone::StaticClass();
	DiamondClass = ADiamond::StaticClass();
	BedrockClass = ABedrock::StaticClass();
	RubyClass = ARuby::StaticClass();

}

void UInventory_Component::AddVoxel(EVoxelType Type, int Amount)			//Add voxels to inventory or adds to count if type is already in there.
{
	if (Type == EVoxelType::BEDROCK || Type == EVoxelType::NONE)		//Stops bedrock from being collected and added to inventory
		return; 

	if (Inventory.Contains(Type))
	{
		Inventory[Type] += Amount;
	}
	else
	{
		Inventory.Add(Type, Amount);
	}

}

int32 UInventory_Component::GetVoxelCount(EVoxelType Type) const			//gets the voxel count from inside inventory, if it doesnt exist in the TMap it returns 0.
{
	const int* Count = Inventory.Find(Type);

	return Count ? *Count : 0;

}


// Called when the game starts
void UInventory_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventory_Component::HasVoxel(EVoxelType Type) const
{
	const int* Count = Inventory.Find(Type);		//Look for voxel type in the invetory map
	return Count && *Count > 0;		//Returns the voxel type and amount if type exists and if more than zero

}

bool UInventory_Component::RemoveVoxel(EVoxelType Type, int Amount)
{
	int* Count = Inventory.Find(Type);		//Looks for voxel in inventory

	if (!Count || *Count <= 0)		//If there isnt a voxel, or the amount is 0, reutrns false, nothing happens
	{
		return false;
	}

	*Count -= Amount;		//Subtracts the voxel amount of 1

	if (*Count <= 0)
	{
		Inventory.Remove(Type);		//If the amount hits zero, delets the type from inventory
	}

	return true;

}

TSubclassOf<AVoxel> UInventory_Component::GetVoxelClass(EVoxelType Type) const
{
	switch (Type)
	{
	case EVoxelType::GRASS:
		return GrassClass;

	case EVoxelType::DIRT:
		return DirtClass;

	case EVoxelType::COBBLESTONE:
		return CobblestoneClass;

	case EVoxelType::DIAMOND:
		return DiamondClass;
	
	case EVoxelType::BEDROCK:
		return BedrockClass;

	case EVoxelType::RUBY:
		return RubyClass;

	default:
		return nullptr;
	}

}


// Called every frame
void UInventory_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

