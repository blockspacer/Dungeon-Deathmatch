// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonPlayerController.h"
#include <CollisionQueryParams.h>
#include <UnrealNames.h>
#include <GameFramework/Actor.h>
#include <DrawDebugHelpers.h>
#include "Interactable.h"
#include "DungeonHUD.h"
#include "DungeonCharacter.h"
#include <GameFramework/PlayerController.h>
#include "Item.h"
#include "InventoryEquipmentSlotWidget.h"

// Console command for drawing interaction cast debug shapes
static int32 DebugInteraction = 0;
FAutoConsoleVariableRef CVARDebugInteraction(
	TEXT("Dungeon.DebugInteraction"),
	DebugInteraction,
	TEXT("Draw debug shapes for interaction casts"),
	ECVF_Cheat);

ADungeonPlayerController::ADungeonPlayerController()
{
	InteractionCastLenth = 400.0f;
	InteractionSweepRadius = 100.0f;
	PlayerForwardInteractionDistance = 10.0f;
}

void ADungeonPlayerController::Tick(float DeltaTime)
{
	CheckFocus();
}

AActor* ADungeonPlayerController::GetFocusedInteractable()
{
	return FocusedInteractable;
}

UInventoryEquipmentSlotWidget* ADungeonPlayerController::GetHoveringInventoryEquipmentSlot()
{
	return HoveringInventoryEquipmentSlot;
}

void ADungeonPlayerController::SetHoveringInventoryEquipmientSlot(UInventoryEquipmentSlotWidget* InventoryEquipmentSlot)
{
	HoveringInventoryEquipmentSlot = InventoryEquipmentSlot;
}

void ADungeonPlayerController::OnInventoryKeyPressed()
{
	ADungeonHUD* DungeonHUD = Cast<ADungeonHUD>(GetHUD());
	if (DungeonHUD)
	{
		if (DungeonHUD->IsCharacterMenuVisible())
		{
			DungeonHUD->HideCharacterMenu();
			bShowMouseCursor = false;
			SetPawnCanLook(true);
		}
		else
		{
			DungeonHUD->ShowCharacterMenu();
			bShowMouseCursor = true;
			SetPawnCanLook(false);
		}
	}
}

void ADungeonPlayerController::OnEscapeKeyPressed()
{
	ADungeonHUD* DungeonHUD = Cast<ADungeonHUD>(GetHUD());
	if (DungeonHUD)
	{
		DungeonHUD->HideCharacterMenu();
		bShowMouseCursor = false;
		SetPawnCanLook(false);	
	}
}

void ADungeonPlayerController::OnUseInventoryItemKeyPressed()
{

}

void ADungeonPlayerController::OnDropInventoryItemKeyPressed()
{
	ADungeonHUD* DungeonHUD = Cast<ADungeonHUD>(GetHUD());
	if (DungeonHUD && DungeonHUD->IsCharacterMenuVisible() && HoveringInventoryEquipmentSlot)
	{
		AItem* SlottedItem = HoveringInventoryEquipmentSlot->GetItem();
		ADungeonCharacter* DungeonPawn = Cast<ADungeonCharacter>(GetPawn());
		if (DungeonPawn)
		{
			DungeonPawn->Server_TryDropItem(SlottedItem);
		}
	}
}

TArray<FInputActionKeyMapping> ADungeonPlayerController::GetKeyForAction(FName ActionName)
{
	return PlayerInput->GetKeysForAction(ActionName);
}

void ADungeonPlayerController::Server_SetFocusedInteractable_Implementation(AActor* Interactable)
{
	FocusedInteractable = Interactable;
}

bool ADungeonPlayerController::Server_SetFocusedInteractable_Validate(AActor* Actor)
{
	return true;
}

void ADungeonPlayerController::SetPawnCanLook(bool bCanLook)
{
	ADungeonCharacter* DungeonPawn = Cast<ADungeonCharacter>(GetPawn());
	if (DungeonPawn)
	{
		DungeonPawn->SetCanLook(bCanLook);
	}
}

void ADungeonPlayerController::CheckFocus()
{
	//if (IsLocalController())
	//{
	//	// Only run interaction checks when this controller is possessing a pawn
	//	APawn* PlayerPawn = GetPawn();
	//	if (!PlayerPawn)
	//	{
	//		if (FocusedInteractable)
	//		{
	//			FocusedInteractable->OnUnfocused();
	//			// Throw event for UI updates
	//			if (OnInteractableUnfocused.IsBound())
	//			{
	//				//OnInteractableUnfocused.Broadcast();
	//			}
	//			FocusedInteractable = nullptr;
	//			Server_SetFocusedInteractable(nullptr);
	//		}

	//		return;
	//	}

	//	// Just do a single sphere trace in front of the player and find the closest object
	//	AActor* ClosestInteractable = nullptr;
	//	TArray<FHitResult> SphereOutHits;

	//	// We don't want to detect interactables behind the player
	//	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	//	FVector PlayerForwadVector = PlayerPawn->GetActorForwardVector();
	//	FVector PlayerForwardLocation = PlayerLocation + (PlayerForwadVector * PlayerForwardInteractionDistance);
	//	FVector SphereSweepStart = PlayerLocation + (PlayerForwadVector * InteractionSweepRadius);
	//	FVector SphereSweepEnd = SphereSweepStart;

	//	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionSweepRadius);

	//	bool DidSphereSweepHit = GetWorld()->SweepMultiByChannel(SphereOutHits, SphereSweepStart, SphereSweepEnd, FQuat::Identity, TRACE_INTERACTABLE, Sphere);
	//	if (DebugInteraction)
	//	{
	//		DrawDebugSphere(GetWorld(), SphereSweepStart, InteractionSweepRadius, 32, FColor::Magenta);
	//		DrawDebugLine(GetWorld(), PlayerLocation, PlayerForwardLocation, FColor::Purple);
	//	}

	//	// If anything was hit by the sphere sweep, process those interactables
	//	if (DidSphereSweepHit)
	//	{
	//		// Find hit interactable closest to cursor
	//		for (FHitResult SweepHit : SphereOutHits)
	//		{
	//			AActor* HitActor = SweepHit.Actor;
	//			IInteractable* HitInteractable = Cast<IInteractable>(SweepHit.Actor);
	//			if (HitInteractable && IInteractable::Execute_GetCanInteract(HitActor))
	//			{
	//				if (ClosestInteractable)
	//				{
	//					float HitDistance = FVector::Distance(PlayerForwardLocation, HitActor->GetActorLocation());
	//					float ClosestDistance = FVector::Distance(PlayerForwardLocation, ClosestInteractable->GetActorLocation());
	//					if (HitDistance < ClosestDistance)
	//					{
	//						ClosestInteractable = HitActor;
	//					}
	//				}
	//				else
	//				{
	//					ClosestInteractable = HitActor;
	//				}
	//			}
	//		}
	//	}

	//	// If interactable was found, set as focus and render outline
	//	if (ClosestInteractable)
	//	{
	//		if (FocusedInteractable)
	//		{
	//			if (FocusedInteractable == ClosestInteractable)
	//			{
	//				return;
	//			}
	//			FocusedInteractable->OnUnfocused();
	//			if (DebugInteraction)
	//			{
	//				UE_LOG(LogTemp, Log, TEXT("ADungeonPlayerController::CheckFocus - Stopped focusing %s"), *FocusedInteractable->GetName());
	//			}
	//		}
	//		Server_SetFocusedInteractable(ClosestInteractable);
	//		FocusedInteractable = ClosestInteractable;
	//		FocusedInteractable->OnFocused();
	//		// Throw event for UI updates
	//		if (OnInteractableFocused.IsBound())
	//		{
	//			//OnInteractableFocused.Broadcast(FocusedInteractable);
	//		}
	//		if (DebugInteraction)
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("ADungeonPlayerController::CheckFocus - Started focusing %s"), *FocusedInteractable->GetName());
	//		}
	//	}
	//	else
	//	{
	//		if (FocusedInteractable)
	//		{
	//			FocusedInteractable->OnUnfocused();
	//			// Throw event for UI updates
	//			if (OnInteractableUnfocused.IsBound())
	//			{
	//				//OnInteractableUnfocused.Broadcast();
	//			}
	//			if (DebugInteraction)
	//			{
	//				UE_LOG(LogTemp, Log, TEXT("ADungeonPlayerController::CheckFocus - Stopped focusing %s"), *FocusedInteractable->GetName());
	//			}
	//			Server_SetFocusedInteractable(nullptr);
	//			FocusedInteractable = nullptr;
	//		}
	//	}
	//	
	//}
}