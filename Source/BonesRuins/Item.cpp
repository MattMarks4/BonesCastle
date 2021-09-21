// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "InventoryComponent.h"


UItem::UItem()
{

}

void UItem::Use(AMyCharacter* MyCharacter)
{
	if (MyCharacter)
	{
		if (PlayerInventory)
		{
			PlayerInventory->RemoveItem(this);
		}
	}
}
