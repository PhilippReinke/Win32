#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <time.h>
#include <Windows.h>

class Snake
{
public:
	int iLength = -1;											// Länge der Schlange -> Anzahl der Glieder der Schlange
	unsigned iRichtung = 4;										// (1) hoch | (2) runter | (3) links | (4) rechts
	BOOL bFood = false;											// Wurde Nahrung ausgeteilt
	RECT rectFood;												// Position der Nahrung
	RECT rectSnakeHead;											// Position des Kopfes
	RECT rectBody[100];											// speichert die Positionen der Kettenglieder der Schlange
	HBRUSH hBrush = CreateSolidBrush(RGB(131, 152, 255));		// Farbe der Umrandung und der Schlange
	HBRUSH hBrushFood = CreateSolidBrush(RGB(255, 152, 131));	// Farbe der Nahrung
	HBRUSH hBackground = CreateSolidBrush(RGB(240, 240, 240));	// Farbe des Hintergrundes
	unsigned iTime = 125;										// Zeit in ms bis Schlang versetzt wird
	unsigned int Schlangenbreite = 15;							// Länge und Breite der Kettenglieder der Schlange

public:
	void resetSnake();
	void spendFood(HDC hDC, int cyClient, int cxClient);
	void savePosition();
	void drawSnake(HDC hDC);
	void shrinkSnake(HDC hDC, RECT rectUmrandung);
	int scrutinyRules(int cyClient, int cxClient);

	HBRUSH get_hBrush() const;
	HBRUSH get_hBrushFood() const;
	BOOL get_bFood() const;
	RECT get_rectSnakeHead() const;
	RECT get_rectFood() const;
	unsigned get_iRichtung() const;
	unsigned get_iTime() const;

	void set_bFood(BOOL bF);
	void set_iRichtung(unsigned iR);
	void set_iTime(unsigned iT);
	void set_rectSnakeHead(LONG iLeft, LONG iTop, LONG iRight, LONG iBottom);
	void set_Schlangenbreite(int iWidthHeight);
};

#endif
