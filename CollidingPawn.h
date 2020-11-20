// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"

UCLASS()
class HOWTO_COMPONENTS_API ACollidingPawn : public APawn
{
	GENERATED_BODY()

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ParticleToggle();

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UParticleSystemComponent *OurParticleSystem;
	/*
	NOTE : We will use this variable to keep a record of the
	Particle System component we will create later.
	You can create a component without creating
	a variable to record, but if you want to use
	it in code, you need to store it in a class
	member variable like this. */

	UPROPERTY(EditAnywhere)
	class UCollidingPawnMovementComponent* OurMovementComponent;

};
