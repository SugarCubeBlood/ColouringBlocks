#pragma once
#include <UObject/ObjectMacros.h>


UENUM(BlueprintType)

enum class EVoxelType : uint8								//Enum to store and be used for each indiviual voxel type.
{
	NONE UMETA(DisplayName = "None"),
	BASE_VOXEL UMETA(DisplayName = "Base Voxel"),
	GRASS UMETA(DisplayName = "Grass"),
	DIRT UMETA(DisplayName = "Dirt"),
	COBBLESTONE UMETA(DisplayName = "Cobblestone"),
	BEDROCK UMETA(DisplayName = "Bedrock"),
	DIAMOND UMETA(DisplayName = "Diamond"),
	RUBY UMETA(DisplayName = "Ruby")
};