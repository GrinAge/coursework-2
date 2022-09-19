#pragma once


struct xOy {
	int x = 0;
	int y = 0;
};
struct Player {
	int x = 5, y = 10;
	char rotation = 'b';
	char position = 'u';
	int CroutchCount = 0;
	int FashGranadeCount = 3;
	char status = 'r';
	bool treasure = false;
	bool ifchose = false;

};
LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void Draw(HDC& memDC, Player& player, char lvl[10][10], bool IfCrossbowWork, HWND hWnd, char fog[10][10]);
void SetBitmaps(HINSTANCE hInst);
bool Collision(int x, int y, char lvl[10][10]);
void lvl1(char lvl[10][10]);
void interaction(Player& player, int x, int y, char lvl[10][10], bool &IfCrossbowWork, char fog[10][10]);
void fog(char fogmap[10][10]);
void PlayerView(char fog[10][10], int playerx, int playery);
void FlashGranadeProccesing(Player& player, char fog[10][10], char lvl[10][10]);
int CALLBACK Choose1DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int CALLBACK Choose2DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int CALLBACK RulesProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
