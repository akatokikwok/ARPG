#include "UI_ICODragDrog.h"
#include "Components/Image.h"

void UUI_ICODragDrog::DrawICON(UTexture2D* InICON)
{
	ICON->SetBrushFromTexture(InICON);
}