// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinecraftCharacter.h"
#include "MinecraftProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include <DrawDebugHelpers.h>

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMinecraftCharacter

AMinecraftCharacter::AMinecraftCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	//// Create VR Controllers.
	//R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	//R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	//R_MotionController->SetupAttachment(RootComponent);
	//L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	//L_MotionController->SetupAttachment(RootComponent);

	//// Create a gun and attach it to the right-hand VR controller.
	//// Create a gun mesh component
	//VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	//VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	//VR_Gun->bCastDynamicShadow = false;
	//VR_Gun->CastShadow = false;
	//VR_Gun->SetupAttachment(R_MotionController);
	//VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	//VR_MuzzleLocation->SetupAttachment(VR_Gun);
	//VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	//VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	Inventory = CreateDefaultSubobject<UInventory_Component>(TEXT("Inventory"));		//Inventory component

	CurrentLevel = 1;		//Initialise Current level
	CurrentXP = 0;			//Initialise Current XP
	CurrentXPCap = CurrentLevel * CurrentLevel + 10;			//Initialise Current xp cap
	//CheckXP();		//No longer needed

	SelectedVoxelType = EVoxelType::NONE;		//Sets default to non for selected voxel type

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Pickaxe(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/Pickaxe.Pickaxe'"));			//Looks for valid material

	if (Pickaxe.Succeeded())																					//Sets the material if valid, gives an error if not.
	{
		PickaxeMesh = Pickaxe.Object;
	}

	FP_Gun->SetSkeletalMesh(PickaxeMesh);

}

int AMinecraftCharacter::GetCurrentXP()			//XP and player level Getters
{
	return CurrentXP;
}

int AMinecraftCharacter::GetCurrentLevel()
{
	return CurrentLevel;
}

int AMinecraftCharacter::GetCurrentXPCap()
{
	return CurrentXPCap;
}

void AMinecraftCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FP_Gun->SetRelativeScale3D(FVector(0.02, 0.03, 0.03));
	FP_Gun->SetRelativeLocation(FVector(3.50934, 18.557436, -4.711196));
	FP_Gun->SetRelativeRotation(FRotator(0.984527, 3.511641, 69.215355));



	//// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	//if (bUsingMotionControllers)
	//{
	//	VR_Gun->SetHiddenInGame(false, true);
	//	Mesh1P->SetHiddenInGame(true, true);
	//}
	//else
	//{
	//	VR_Gun->SetHiddenInGame(true, true);
	//	Mesh1P->SetHiddenInGame(false, true);
	//}

	UE_LOG(LogTemp, Warning, TEXT("Character XP at BeginPlay = %d"), CurrentXP);


	CheckXP();		//Ensures XP and Level values are valid when starting game
}

void AMinecraftCharacter::TraceForward()
{
	FVector Location;
	FRotator Rotation;
	FHitResult HitResult;

	GetController()->GetPlayerViewPoint(Location, Rotation);		//Gets player camera positions and direction

	FVector StartPoint = Location;
	FVector EndPoint = StartPoint + (Rotation.Vector() * 1200);

	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Cyan, false, 2.0f);		//Draws debug line so trace is visable

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,StartPoint, EndPoint, ECC_Visibility, TraceParams);

	if (bHit)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("You have hit %s"), *HitResult.GetActor()->GetName()));		//States what was hit

		DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, 2.0f);		//Draws box at impact point

		//TargetVoxel = Cast<AVoxel>(HitResult.GetActor());		//No longer needed

		AVoxel* HitVoxel = Cast<AVoxel>(HitResult.GetActor());		//Checks that the actor hit was a voxel.
		if (HitVoxel)
		{
			CurrentXP += HitVoxel->Interact();	//Gives XP for hitting voxel
			CheckXP();		//Checks XP and runs level up if needed

			if (Inventory)		//Adds the voxel to the inventory
			{
				Inventory->AddVoxel(HitVoxel->VoxelType, 1);

				/*if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,FString::Printf(TEXT("Collected 1 x %s"),*UEnum::GetValueAsString(HitVoxel->VoxelType)));
				}*/
			}

			return; // Stop here so nothing else unnecessary runs
		}


		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteraction_Interface::StaticClass()))		//If actor hit isnt a voxel. Runs the base game interaction. Which is nothing currently.
		{
			CurrentXP += Cast<IInteraction_Interface>(HitResult.GetActor())->Interact();
			CheckXP();
		}
	}

}

void AMinecraftCharacter::TraceForwardRight()
{
	FVector Location;
	FRotator Rotation;
	FHitResult HitResult;

	GetController()->GetPlayerViewPoint(Location, Rotation);		//Gets player camera positions and direction

	FVector StartPoint = Location;
	FVector EndPoint = StartPoint + (Rotation.Vector() * 1200);

	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 2.0f);		//Draws debug line so trace is visable

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, TraceParams);

	AVoxel* Voxel = Cast<AVoxel>(HitResult.GetActor());		//Checks if the trace has hit comething and it is a voxel. If not it returns.
	if (!bHit || !Voxel)
	{
		return;
	}

	AChunk* Chunk = Voxel->GetOwningChunk();		//Getter for the chunk the voxel hit belongs to, if its not a chunk returns.
	if (!Chunk)
	{
		return;
	}

	// placement logic to place down blocks.

	if (SelectedVoxelType == EVoxelType::NONE)
	{
		return;
	}

	if (!Inventory->HasVoxel(SelectedVoxelType))
	{
		return;
	}

	const float BlockSize = 100.0f * Chunk->GetVoxelMeasurement();		//Gets the size of the voxel from the chunk

	FVector BaseLocation = Voxel->GetActorLocation();			// Gets the voxels aligned location instead of the hit location
	FVector PlaceLocation = BaseLocation + (HitResult.ImpactNormal * BlockSize);

	PlaceLocation.X = FMath::RoundToFloat(PlaceLocation.X / BlockSize) * BlockSize;		//Places voxels on the grid so its aligned with exisiting voxels.
	PlaceLocation.Y = FMath::RoundToFloat(PlaceLocation.Y / BlockSize) * BlockSize;
	PlaceLocation.Z = FMath::RoundToFloat(PlaceLocation.Z / BlockSize) * BlockSize;

	TSubclassOf<AVoxel> ClassToSpawn = Inventory->GetVoxelClass(SelectedVoxelType);		//Gets the voxel type from the invetory based on the currently selected voxel type.
	if (!ClassToSpawn)
	{
		return;
	}

	AVoxel* NewVoxel = GetWorld()->SpawnActor<AVoxel>(ClassToSpawn, PlaceLocation, FRotator::ZeroRotator);		//Spawns the voxel thats is being placed. Placing aligned with other voxels from above calculation.

	if (NewVoxel)
	{
		NewVoxel->SetOwningChunk(Chunk);		//Sets the ownership of the new voxel to the chunk.
		NewVoxel->SetActorScale3D(Chunk->GetVoxelScale());		//Sets the scale of the new voxel based on the chunks scale.
	}

	Inventory->RemoveVoxel(SelectedVoxelType, 1);	//removes the voxel from the inventory

}

int AMinecraftCharacter::Interact()
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("??"));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMinecraftCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	//Sets up input for scrolling through voxels
	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &AMinecraftCharacter::ScrollUp);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &AMinecraftCharacter::ScrollDown);


	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMinecraftCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire Right", IE_Pressed, this, &AMinecraftCharacter::OnFireRight);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMinecraftCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMinecraftCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMinecraftCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMinecraftCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMinecraftCharacter::LookUpAtRate);
}

int AMinecraftCharacter::GetPlayerXP()		//XP Getter, returns current xp value
{
	return CurrentXP;
}

void AMinecraftCharacter::CheckXP()			//Checks current XP against Current XP cap, If higher than cap, levels up character and sets the new level xp cap higher. Scales over time.
{
	while (CurrentXP >= CurrentXPCap)
	{
		CurrentLevel++;

		CurrentXP = CurrentXP - CurrentXPCap;

		CurrentXPCap = CurrentLevel * CurrentLevel + 10;
	}
}

void AMinecraftCharacter::ScrollVoxelSelection(int Direction)
{
	if (Direction == 0 || !Inventory)		//No input or no inventory returns nothing
	{
		return;
	}

	TArray<EVoxelType> SelectableTypes;			//Makes array of selectable voxel types

	for (const auto& Pair : Inventory->Inventory)		// Adds voxel types that the player  more than 0 of in inventory to the selectable types.
	{
		if (Pair.Value > 0)
		{
			SelectableTypes.Add(Pair.Key);
		}
	}

	if (SelectableTypes.Num() == 0)		//If inventory is empty no voxel can be selected and sets the default selection to none
	{
		SelectedVoxelType = EVoxelType::NONE;
		return;
	}

	int Index = SelectableTypes.Find(SelectedVoxelType);		//Creates an index  of the items in the selected voxel type array

	if (Index == INDEX_NONE)		//Resets the index if the voxel type cant be found.
	{
		Index = 0;
	}

	Index += Direction;		// Apply scroll direction (+1 = next, -1 = previous)

	if (Index >= SelectableTypes.Num()) Index = 0;		//Wrap around for scrolling, if index goes pass the number of types available it sets it back to the first one
	if (Index < 0) Index = SelectableTypes.Num() - 1;		//If scoll is less that zero, Wraps back round to the last item in the index.

	SelectedVoxelType = SelectableTypes[Index];		// Update selected voxel type
}

void AMinecraftCharacter::ScrollUp()
{
	ScrollVoxelSelection(1.0f);			//Scroll forward through voxel types
}

void AMinecraftCharacter::ScrollDown()
{
	ScrollVoxelSelection(-1.0f);		//Scoll back through voxel types.
}

void AMinecraftCharacter::OnFire()
{
	// if (GEngine) GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Emerald, FString::Printf(TEXT("My XP is: %d"), CurrentXP));

	TraceForward();		//Left click used for mining voxels
	//Interact();
}

void AMinecraftCharacter::OnFireRight()
{
	TraceForwardRight();		//Right click used for placing voxels.
}

void AMinecraftCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMinecraftCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AMinecraftCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AMinecraftCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AMinecraftCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMinecraftCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMinecraftCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMinecraftCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMinecraftCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMinecraftCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMinecraftCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMinecraftCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
