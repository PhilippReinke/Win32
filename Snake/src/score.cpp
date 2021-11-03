#include "../include/score.h"
#include <algorithm>

void Score::setScore(int score){
	iCurrentlyScore = score;
}

void Score::setName(std::string n){
	name = n;
}

void Score::saveScore(){
	std::ofstream file;

	file.open("snake.ph", std::ios::out | std::ios::app);
	file << iCurrentlyScore << '\n' << name << '\n';
	file.close();
}

void Score::resetScore(){
	std::ofstream file;

	file.open("snake.ph", std::ios::trunc);
	file.close();
}

void Score::operator++(){
	iCurrentlyScore++;
}

void Score::operator+=(int val){
	iCurrentlyScore += val;
}

std::wstring Score::convertScore(){
	std::wstringstream os;
	os << iCurrentlyScore;

	std::wstring wstring = os.str();
	return wstring;
}

bool Score::readScore(){
	std::ifstream file("snake.ph");
	std::string szPuffer;
	int iPuffer{ 10 };
	
	if (!file)
		return 1;

	// score leeren
	score.clear();

	while (getline(file, szPuffer)){
		// Punktezahl auslesen
		iPuffer = std::stoi(szPuffer, nullptr, 10);

		// Name auslesen
		getline(file, szPuffer);

		// speichern
		score.insert(std::make_pair(iPuffer, szPuffer));
	}

	return 0;
}

bool Score::readName(HWND hWnd, int cxClient, int cyClient){
	HWND hDialog;

	// neues Fenster erstellen

	return 0;
}