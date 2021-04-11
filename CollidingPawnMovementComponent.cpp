// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawnMovementComponent.h"

/*
* Pawn Movement Component에는 흔한 물리 함수성에 도움이 되는 강력한 내장 기능이 몇가지 들어있어,
* 여러가지 폰 유형에 무브먼트 코드를 공유하기가 좋습니다. 컴포넌트를 사용하여 함수성을 분리시켜 놓는 것은 
* 매우 좋은 습관인데, 프로젝트의 덩치가 커지면서 폰도 복잡해지기 때문입니다.
*/

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 모든 것이 아직 유효한지, 이동 가능한지 확인합니다.
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    // ACollidingPawn::Tick 에 설정한 무브먼트 벡터를 구(한 뒤 소거)합니다.
    FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // 무언가에 부딛혔으면, 돌아가 봅니다.
        if (Hit.IsValidBlockingHit())
        {
            SlideAlongSurface(DesiredMovementThisFrame, 1.0f - Hit.Time, Hit.Normal, Hit);
        }
    }
};

/*
* 
* 이 TickComponent 함수는 UPawnMovementComponent 클래스에 제공되는 강력한 기능을 몇가지 활용하고 있습니다.
* ConsumeInputVector() 는 미동입력을 저장하는데 사용할 내장변수값을 보고 및 소거합니다.
* SafeMoveUpdatedComponent() 는 언리얼엔진 피직스를 사용하여 입체장애물을 피해 PawnMovementComponent 를 이동시킵니다.
* SlideAlongSurface() 는 이동하다가 충돌 발생시 그냥 제자리에 멈춰 벽이나 경사로에 "달라붙기" 보다는, 그 표면을 타고 부드럽게 
* 미끄러지도록 하는 데 관련된 물리 계산 처리를 합니다.
* 
*/