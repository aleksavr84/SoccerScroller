#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HighScore.generated.h"

UCLASS()
class SOCCERSCROLLER_API UHighScore : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HighScoreValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HighScoreText;
};
