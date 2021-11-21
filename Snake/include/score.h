#ifndef SCORE_H
#define SCORE_H

#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
#include <map>

class Score
{
private:
	unsigned int iCurrentlyScore = 0;		// momentaner Punktestand
	std::string name = "unknown";			// Name des Spielers
	std::multimap<int, std::string> score;	// gespeicherte Punktestände

public:
	void setScore(int score);
	void setName(std::string n);

	std::string getName(){ return name; }

	void saveScore();
	bool readScore();
	void resetScore();

	void operator++();
	void operator+= (int val);

	std::multimap <int, std::string> getScore(){ return score; }
	std::wstring convertScore();

	DLGPROC CALLBACK ProcDialog(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
#endif
