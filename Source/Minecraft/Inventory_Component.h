// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Voxel.h"
#include "Bedrock.h"
#include "Cobblestone.h"
#include "Diamond.h"
#include "Dirt.h"
#include "Grass.h"
#include "Bedrock.h"
#include "Ruby.h"
#include "Inventory_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInventory_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory_Component();								//Constructor for inventory component

	UFUNCTION(BlueprintCallable)
	void AddVoxel(EVoxelType Type, int Amount = 1);			//Adds voxels of specific type into the inventory based on the enum

	UFUNCTION(BlueprintCallable)
	int GetVoxelCount(EVoxelType Type) const;					//returns the amount of specific type of voxels already in the inventory

	UFUNCTION(BlueprintCallable)								//Getter for the inventory
	const TMap<EVoxelType, int>& GetInventory() const { return Inventory; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)				//TMap used for inventory to store both the voxel type and the amount. Exposed so the UI can call upon it.
	TMap<EVoxelType, int> Inventory;

	UFUNCTION(BlueprintCallable, Category = "Inventory")		//Used to check if the inventory has voxels
		bool HasVoxel(EVoxelType Type) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")		//Removes voxel from inventory
	bool RemoveVoxel(EVoxelType Type, int Amount = 1);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AVoxel> GetVoxelClass(EVoxelType Type) const;	//Selects voxel type



	UPROPERTY(EditAnywhere, BlueprintReadWrite)				//voxel classes used when spawning placed blocks
	TSubclassOf<AVoxel> GrassClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVoxel> DirtClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVoxel> CobblestoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVoxel> DiamondClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVoxel> BedrockClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVoxel> RubyClass;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
