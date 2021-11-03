#include "../include/snake.h"

void Snake::resetSnake(){
	// Schlange zurückseten
	iLength = -1;
	iRichtung = 4;

	SetRect(&rectSnakeHead, 2,
		2,
		Schlangenbreite + 2,
		Schlangenbreite + 2);
}

int Snake::scrutinyRules(int cyClient, int cxClient){
	if (rectSnakeHead.bottom > cyClient - 2 || rectSnakeHead.right > cxClient - 2 ||
		rectSnakeHead.left < 2 || rectSnakeHead.top < 2)
		return 1;	// Regeln nicht eingehalten

	else{
		for (int i = 0; i < iLength; i++){
			if (rectBody[i].bottom == rectSnakeHead.bottom && rectBody[i].right == rectSnakeHead.right)
				return 1;	// Regeln nicht eingehalten
		}
	}

	// Regeln werden eingehalten
	return 0;
}

void Snake::spendFood(HDC hDC, int cyClient, int cxClient){
	int bottom, right;

	// wenn Nahrung ausgeteilt, überspringe den Rest
	if (bFood == TRUE)
		FillRect(hDC, &rectFood, hBrushFood);

	else{
		// Schlange verlängern
		iLength++;

		// Nahrung austeilen
		// zufällige Koordinaten ermitteln
		srand((UINT)time(0));
		bottom = 2 + (Schlangenbreite + 1) * ((int)rand() % ((cyClient - 5) / (Schlangenbreite + 1)) + 1);
		right = 2 + (Schlangenbreite + 1) * ((int)rand() % ((cxClient - 5) / (Schlangenbreite + 1)) + 1);

		// Rechteck setzen
		SetRect(&rectFood, right - Schlangenbreite + 4,
			bottom - Schlangenbreite + 4,
			right - 4,
			bottom - 4);

		// Nahrung zeichnen
		FillRect(hDC, &rectFood, hBrushFood);
		bFood = TRUE;
	}
}

void Snake::savePosition(){
	// verschiebe die gespeicherten Werte
	for (int i = 0; i < iLength; i++){
		rectBody[iLength - i] = rectBody[iLength - (i + 1)];
	}

	rectBody[0] = rectSnakeHead;
}

void Snake::drawSnake(HDC hDC){
	// zeichne den Kopf der Schlange
	FillRect(hDC, &rectSnakeHead, hBrush);

	// zeichne den Rest des Körpers
	for (int i = 0; i < iLength; i++)
		FillRect(hDC, &rectBody[i], hBrush);
}

void Snake::shrinkSnake(HDC hDC, RECT rectUmrandung){
	while(iLength >= 0){
		// lösche das letzte Glied der Schlange
		FillRect(hDC, &rectBody[iLength--], hBackground);

		// Umrandung zeichnen
		FrameRect(hDC, &rectUmrandung, hBrushFood);

		// warte
		Sleep(50);
	}
	// lösche den Kopf und Nahrung
	FillRect(hDC, &rectSnakeHead, hBackground);
	FillRect(hDC, &rectFood, hBackground);
}

HBRUSH Snake::get_hBrush()const{
	return hBrush;
}

HBRUSH Snake::get_hBrushFood()const{
	return hBrushFood;
}

BOOL Snake::get_bFood()const{
	return bFood;
}

RECT Snake::get_rectSnakeHead()const{
	return rectSnakeHead;
}

RECT Snake::get_rectFood()const{
	return rectFood;
}

unsigned Snake::get_iRichtung()const{
	return iRichtung;
}

unsigned Snake::get_iTime()const{
	return iTime;
}

void Snake::set_iRichtung(unsigned iR){
	iRichtung = iR;
}

void Snake::set_bFood(BOOL bF){
	bFood = bF;
}

void Snake::set_iTime(unsigned iT){
	iTime = iT;
}

void Snake::set_rectSnakeHead(LONG iLeft, LONG iTop, LONG iRight, LONG iBottom){
	rectSnakeHead.left = iLeft;
	rectSnakeHead.top = iTop;
	rectSnakeHead.right = iRight;
	rectSnakeHead.bottom = iBottom;
}

void Snake::set_Schlangenbreite(int iWidthHeight)
{
	Schlangenbreite = iWidthHeight;
}