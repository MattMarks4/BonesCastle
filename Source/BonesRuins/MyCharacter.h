// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class BONESRUINS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* BoomArm;

	    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			class UInventoryComponent* Inventory;


public:
	// Sets default values for this character's properties
	AMyCharacter();

	UFUNCTION(BlueprintCallable, Category = "Items")
		void UseItem(class UItem* Item);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void LookUp(float Axis);
	void LookRight(float Axis);

	void Zoom(float value);
	void ChangeCamera();
	UPROPERTY()
		bool firstPerson;

	void CheckJump();
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		bool isJumping;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
