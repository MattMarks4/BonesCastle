// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item.h"
#include "InventoryComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoomArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	BoomArm->SetupAttachment(RootComponent);
	BoomArm->TargetArmLength = 300.f;
	BoomArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(BoomArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->Capacity = 16;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	isJumping = false;

	firstPerson = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isJumping)
	{
		Jump();
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AMyCharacter::LookRight);

	PlayerInputComponent->BindAxis("Zoom", this, &AMyCharacter::Zoom);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::CheckJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::CheckJump);

	PlayerInputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AMyCharacter::ChangeCamera);
}

void AMyCharacter::MoveForward(float Axis)
{
	if (Axis != 0.f)
	{
		const FRotator ControlDirection = GetControlRotation();
		const FRotator YawDirection = FRotator(0, ControlDirection.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::MoveRight(float Axis)
{
	if (Axis != 0.f)
	{
		const FRotator ControlDirection = GetControlRotation();
		const FRotator YawDirection = FRotator(0, ControlDirection.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::LookUp(float Axis)
{
	
	AddControllerPitchInput(Axis);
	if(Axis)
	{ 
		float temp = 0;

		if (firstPerson)
		{
			temp = Camera->GetRelativeRotation().Pitch + Axis;
			if (temp > -65 && temp < 65)
			{
				Camera->AddLocalRotation(FRotator(Axis, 0, 0));
			}
		}
		else
		{
			temp = BoomArm->GetRelativeRotation().Pitch + Axis;
			if (temp > -65 && temp < 25)
			{
				BoomArm->AddLocalRotation(FRotator(Axis, 0, 0));
			}
		}
	}	
}

void AMyCharacter::LookRight(float Axis)
{
	AddControllerYawInput(Axis);
}

void AMyCharacter::Zoom(float value)
{
	if (value)
	{
		float ZoomAmount = BoomArm->TargetArmLength + (value * -30);
		if (ZoomAmount > 100 && ZoomAmount < 500)
		{
			BoomArm->TargetArmLength = ZoomAmount;
		}
	}
}

void AMyCharacter::ChangeCamera()
{
	if (firstPerson)
	{
		BoomArm->TargetArmLength = 300.f;
		BoomArm->SetRelativeRotation(FRotator(-45, 0, 0));
		Camera->SetRelativeRotation(FRotator(0, 0, 0));
		Camera->AttachTo(BoomArm, USpringArmComponent::SocketName);
		firstPerson = false;
	}
	else
	{
		Camera->AttachTo(RootComponent);
		firstPerson = true;
	}
}

void AMyCharacter::CheckJump()
{
	if (isJumping) isJumping = false;
	else isJumping = true;
}

void AMyCharacter::UseItem(UItem* Item)
{
	if (Item)
	{
		Item->OnUse(this);
		Item->Use(this);
	}
}
