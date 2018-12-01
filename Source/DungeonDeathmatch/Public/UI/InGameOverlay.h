// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameOverlay.generated.h"

class UInventoryMenu;
class UEquipmentMenu;

/**
 * The main UI widget that contains all other menu widgets that the player can see while in a game.
 * This should be the only widget directly drawn to the viewport, and all other widgets will be accessed from here.
 */
UCLASS()
class DUNGEONDEATHMATCH_API UInGameOverlay : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/** 
	 * The name of the InventoryMenuWidget that is contained in this widget in the editor. 
	 * Used to find the widget from the WidgetTree.
	 */
	UPROPERTY(EditAnywhere, Category = "UI")
	FName InventoryMenuWidgetName;

	/**
	 * The name of the EquipmentMenuWidget that is contained in this widget in the editor.
	 * Used to find the widget from the WidgetTree.
	 */
	UPROPERTY(EditAnywhere, Category = "UI")
	FName EquipmentMenuWidgetName;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	UInventoryMenu* GetInventoryMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowInventoryMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideInventoryMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	UEquipmentMenu* GetEquipmentMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowEquipmentMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideEquipmentMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool AreInventoryAndEquipmentMenusVisible();
};
