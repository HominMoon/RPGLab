#pragma once

UENUM(BlueprintType)
enum class EPlayerMoveState : uint8
{
	EPM_Idle UMETA(DisplayName = "Idle"),
	EPM_Jumped UMETA(DisplayName = "Jumped")
};