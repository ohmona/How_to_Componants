// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CollidingPawnMovementComponent.h"

// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent -> InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	//Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Succeeded"));
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.0f));
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Hello")); }
	/* NOTE : Although you can see the asset location of our Static Mesh
	asset in the code, hard-coding asset paths is not generally 
	considered 	the best way to load assets. It is usually preferred 
	to create the Component itself in code if the class needs to use it, 
	and then select the assets in the Unreal Editor. However, it is 
	possible to do this directly in code, and could be faster
	for programmers	debugging or building new features.*/

	// Create a particle system that we can activate or deactivate
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = true;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	// Use a spring arm to give the camera smooth, natural-felling mation.
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	//Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	/* NOTE: Unlike the other Components we've seen so far, we don't need to 
	attach this Component to our own Component hierarchy. This is 
	because our other Components are all types of Scene Components,
	which inherently require physical locations. Movement Controllers, 
	however, are not Scene Components and do not represent physical
	objects, so the concept of existing at a physical location or being 
	physically attached to another Component does not apply to them.*/
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::ParticleToggle);

	InputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);
}

UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ACollidingPawn::MoveForward(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ACollidingPawn::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void ACollidingPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void ACollidingPawn::ParticleToggle()
{
	if (OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}