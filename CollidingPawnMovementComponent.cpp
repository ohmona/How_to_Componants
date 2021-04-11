// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawnMovementComponent.h"

/*
* Pawn Movement Component���� ���� ���� �Լ����� ������ �Ǵ� ������ ���� ����� ��� ����־�,
* �������� �� ������ �����Ʈ �ڵ带 �����ϱⰡ �����ϴ�. ������Ʈ�� ����Ͽ� �Լ����� �и����� ���� ���� 
* �ſ� ���� �����ε�, ������Ʈ�� ��ġ�� Ŀ���鼭 ���� ���������� �����Դϴ�.
*/

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ��� ���� ���� ��ȿ����, �̵� �������� Ȯ���մϴ�.
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    // ACollidingPawn::Tick �� ������ �����Ʈ ���͸� ��(�� �� �Ұ�)�մϴ�.
    FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // ���𰡿� �ε�������, ���ư� ���ϴ�.
        if (Hit.IsValidBlockingHit())
        {
            SlideAlongSurface(DesiredMovementThisFrame, 1.0f - Hit.Time, Hit.Normal, Hit);
        }
    }
};

/*
* 
* �� TickComponent �Լ��� UPawnMovementComponent Ŭ������ �����Ǵ� ������ ����� ��� Ȱ���ϰ� �ֽ��ϴ�.
* ConsumeInputVector() �� �̵��Է��� �����ϴµ� ����� ���庯������ ���� �� �Ұ��մϴ�.
* SafeMoveUpdatedComponent() �� �𸮾��� �������� ����Ͽ� ��ü��ֹ��� ���� PawnMovementComponent �� �̵���ŵ�ϴ�.
* SlideAlongSurface() �� �̵��ϴٰ� �浹 �߻��� �׳� ���ڸ��� ���� ���̳� ���ο� "�޶�ٱ�" ���ٴ�, �� ǥ���� Ÿ�� �ε巴�� 
* �̲��������� �ϴ� �� ���õ� ���� ��� ó���� �մϴ�.
* 
*/