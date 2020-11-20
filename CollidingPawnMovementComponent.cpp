// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawnMovementComponent.h"

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		//if we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
};
/*This code will move our Pawn smoothly around the world,
sliding off of surfaces where appropriate. There is no 
gravity applied to our Pawn, and its maximum speed is hard-coded 
to 150 Unreal Units per second.*/

/*NOTE : This TickComponent function makes use of a few of the 
powerful features offered by the UPawnMovementComponent class.

 - ConsumeInputVector : reports and clears the value of a built-in 
variable that we will use to store our movement inputs.

 - SafeMoveUpdatedComponent : uses Unreal Engine physics to move our 
Pawn Movement Component while respecting solid barriers.

 - SlideAlongSurface : handles the calculations and physics involved 
with sliding smoothly along collision surfaces like walls and ramps
when a move results in a collision, rather than simply stopping in
place and sticking to the wall or ramp.


There are more features included in Pawn Movement Components that
are worthy of examination, but they are not needed for the scope 
of this tutorial. Looking at other classes, such as Floating Pawn Movement,
Spectator Pawn Movement, or Character Movement Component, could provide 
additional usage examples and ideas.*/

