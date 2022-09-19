#include <Windows.h>
#include <string>
#include "Header.h"
#include "resource2.h"

HINSTANCE hInstForDialog;

HBITMAP hTestBmp, hWallBmp, hFloorBmp, hFakeChestBmp, hTreasureChestBmp, hHoleBmp, hCrossbowBmp, hBeartrapBmp, hBeartrapClosedBmp, hPlayerFrontBmp, hPlayerBackBmp, hPlayerRightBmp, hPlayerLeftBmp,
hPlayerDownBmp, hArrowBmp, hCrutchBmp, hFogBmp, hLoseScreenBmp1, hWinScreenBmp1, hLoseScreenBmp2, hWinScreenBmp2, hTreasureChestOpenBmp, hChooseScreenBmp1, hChooseScreenBmp2, hBackgroundBmp,
hNumber0Bmp, hNumber1Bmp, hNumber2Bmp, hNumber3Bmp, hCrouchIco0Bmp, hCrouchIco1Bmp, hTreasureIco0Bmp, hTreasureIco1Bmp,
hNumber0Bmp2, hNumber1Bmp2, hNumber2Bmp2, hNumber3Bmp2, hCrouchIco0Bmp2, hCrouchIco1Bmp2, hTreasureIco0Bmp2, hTreasureIco1Bmp2;

char map[10][10] = {
	'w','w','w','w','w','_','w','w','w','w',
	'w','h','_','w','w','_','h','w','f','w',
	'w','f','_','_','_','_','_','w','_','w',
	'w','w','w','w','w','_','h','w','_','w',
	'_','_','_','_','_','_','w','w','_','w',
	'w','w','_','w','w','_','w','_','_','_',
	'w','c','_','_','_','_','w','b','w','w',
	'w','_','_','w','_','h','w','_','_','w',
	'w','f','h','w','_','w','w','t','h','w',
	'w','w','w','w','_','w','w','w','w','w'
};

char map1[10][10];
char fogmap1[10][10];
char map2[10][10];
char fogmap2[10][10];


xOy root1, root2;
Player player1, player2;
int time = 0;
bool IfCrossbowWork1 = true;
bool IfCrossbowWork2 = true;

int start, finish1, finish2;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	
	WNDCLASS MainClass = {0};
	MainClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	MainClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	MainClass.hInstance = hInst;
	MainClass.lpszClassName = L"MainWndClass";
	MainClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	MainClass.lpfnWndProc = MainProcedure;
	MainClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	if (!RegisterClassW(&MainClass)) { return -1; }
	MSG	MainMessage = { 0 };

	HWND hMainWnd = CreateWindow(L"MainWndClass", L"Лабіринт",  WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE , 100, 100, 1296, 778, NULL, NULL, NULL, NULL); //1296, 778 

	HDC dc = GetDC(hMainWnd);

	int heght = 720;
	int wight = 1280;
	
	root1.x = 68;
	root1.y = 50;
	root2.x = 724;
	root2.y = 50;


	SetBitmaps(hInst);
	
	hInstForDialog = hInst;
	
	
	SetTimer(hMainWnd, 1, 500, NULL);

	while (1) {
		if (PeekMessageA(&MainMessage, NULL, 0, 0, PM_REMOVE)) {
			if (MainMessage.message == WM_QUIT) {
				KillTimer(hMainWnd, 1);
			break;
		}
	
			TranslateMessage(&MainMessage);
			DispatchMessage(&MainMessage);

			//Оброка закривання капкану милицею для гравця 1 та 2
			if (player1.status == 'p' && player1.x == 8 && player1.y == 7 && map1[6][7] == 'b') {
				if (player1.CroutchCount > 0) {
					player1.CroutchCount--;
					map1[6][7] = 's';
				}
				else
					SetTimer(hMainWnd, 22, 300, NULL);
			}
			if (player2.status == 'p' && player2.x == 8 && player2.y == 7 && map2[6][7] == 'b') {
				if (player2.CroutchCount > 0) {
					player2.CroutchCount--;
					map2[6][7] = 's';
				}
				else
					SetTimer(hMainWnd, 33, 300, NULL);
			}
		
		}
	
		HDC memDC = CreateCompatibleDC(dc);
		HBITMAP memBM = CreateCompatibleBitmap(dc, wight, heght);
		SelectObject(memDC, memBM);
		
		//Контекст пам'яті для ігрових зон гравця 1 та 2
		
		HDC Player1AreaDC = CreateCompatibleDC(dc);
		HBITMAP Player1AreaBM = CreateCompatibleBitmap(dc, 500, 500);
		SelectObject(Player1AreaDC, Player1AreaBM);
		HDC Player2AreaDC = CreateCompatibleDC(dc);
		HBITMAP Player2AreaBM = CreateCompatibleBitmap(dc, 500, 500);
		SelectObject(Player2AreaDC, Player2AreaBM);

		//Фоновий малюнок

		HDC backgroundDC = CreateCompatibleDC(dc);
		HBITMAP  backgroundBM = CreateCompatibleBitmap(dc, 1280, 720);
		SelectObject(backgroundDC, backgroundBM);
		SelectObject(backgroundDC, hBackgroundBmp);
		BitBlt(memDC, 0, 0, wight, heght, backgroundDC, 0, 0, SRCCOPY);
		
		//Панель гравця 1 

		HDC Number1DC = CreateCompatibleDC(dc);
		HBITMAP  Number1BM = CreateCompatibleBitmap(dc, 66, 103);
		SelectObject(Number1DC, Number1BM);

		if(player1.FashGranadeCount == 0)SelectObject(Number1DC, hNumber0Bmp);
		if(player1.FashGranadeCount == 1)SelectObject(Number1DC, hNumber1Bmp);
		if(player1.FashGranadeCount == 2)SelectObject(Number1DC, hNumber2Bmp);
		if(player1.FashGranadeCount == 3)SelectObject(Number1DC, hNumber3Bmp);

		if(player1.status == 'p')BitBlt(memDC, 124, 592, 66, 103, Number1DC, 0, 0, SRCCOPY);

		HDC Ico1DC = CreateCompatibleDC(dc);
		HBITMAP  Ico1BM = CreateCompatibleBitmap(dc, 100, 100);
		SelectObject(Ico1DC, Ico1BM);

		if (player1.CroutchCount == 0)SelectObject(Ico1DC, hCrouchIco0Bmp);
		if (player1.CroutchCount == 1)SelectObject(Ico1DC, hCrouchIco1Bmp);
	
		if (player1.status == 'p')BitBlt(memDC, 200, 592, 100, 100, Ico1DC, 0, 0, SRCCOPY);

		if (!player1.treasure)SelectObject(Ico1DC, hTreasureIco0Bmp);
		if (player1.treasure)SelectObject(Ico1DC, hTreasureIco1Bmp);

		if (player1.status == 'p')BitBlt(memDC, 320, 592, 100, 100, Ico1DC, 0, 0, SRCCOPY);

		//Панель гравця 2 

		HDC Number2DC = CreateCompatibleDC(dc);
		HBITMAP  Number2BM = CreateCompatibleBitmap(dc, 66, 103);
		SelectObject(Number2DC, Number2BM);

		if (player2.FashGranadeCount == 0)SelectObject(Number2DC, hNumber0Bmp2);
		if (player2.FashGranadeCount == 1)SelectObject(Number2DC, hNumber1Bmp2);
		if (player2.FashGranadeCount == 2)SelectObject(Number2DC, hNumber2Bmp2);
		if (player2.FashGranadeCount == 3)SelectObject(Number2DC, hNumber3Bmp2);

		if (player2.status == 'p')BitBlt(memDC, 780, 592, 66, 103, Number2DC, 0, 0, SRCCOPY);

		HDC Ico2DC = CreateCompatibleDC(dc);
		HBITMAP  Ico2BM = CreateCompatibleBitmap(dc, 100, 100);
		SelectObject(Ico2DC, Ico2BM);

		if (player2.CroutchCount == 0)SelectObject(Ico2DC, hCrouchIco0Bmp2);
		if (player2.CroutchCount == 1)SelectObject(Ico2DC, hCrouchIco1Bmp2);

		if (player2.status == 'p')BitBlt(memDC, 856, 592, 100, 100, Ico2DC, 0, 0, SRCCOPY);

		if (!player2.treasure)SelectObject(Ico2DC, hTreasureIco0Bmp2);
		if (player2.treasure)SelectObject(Ico2DC, hTreasureIco1Bmp2);

		if (player2.status == 'p')BitBlt(memDC, 976, 592, 100, 100, Ico2DC, 0, 0, SRCCOPY);

		//Мальвання ігрової зони гравця 1

		if(player1.status == 'p')Draw(Player1AreaDC, player1, map1, IfCrossbowWork1, hMainWnd, fogmap1);
		if (player1.status == 'l') {
			SelectObject(Player1AreaDC, hLoseScreenBmp1);
		}
		if (player1.status == 'w') {
			SelectObject(Player1AreaDC, hWinScreenBmp1);
		}
		if (player1.status == 'c') {
			SelectObject(Player1AreaDC, hChooseScreenBmp1);
		}

		//Мальвання ігрової зони гравця 2

		if (player2.status == 'p')Draw(Player2AreaDC, player2, map2, IfCrossbowWork2, hMainWnd, fogmap2);
		if (player2.status == 'l') {
			SelectObject(Player2AreaDC, hLoseScreenBmp2);
		}
		if (player2.status == 'w') {
			SelectObject(Player2AreaDC, hWinScreenBmp2);
		}
		if (player2.status == 'c') {
			SelectObject(Player2AreaDC, hChooseScreenBmp2);
		}

		BitBlt(memDC, root1.x, root1.y, 500, 500, Player1AreaDC, 0, 0, SRCCOPY);
		BitBlt(memDC, root2.x, root2.y, 500, 500, Player2AreaDC, 0, 0, SRCCOPY);
		BitBlt(dc, 0, 0, wight, heght, memDC, 0, 0, SRCCOPY);

		DeleteDC(Ico1DC);
		DeleteObject(Ico1BM);
		DeleteDC(Number1DC);
		DeleteObject(Number1BM);
		DeleteDC(Ico2DC);
		DeleteObject(Ico2BM);
		DeleteDC(Number2DC);
		DeleteObject(Number2BM);
		DeleteDC(backgroundDC);
		DeleteObject(backgroundBM);
		DeleteDC(Player1AreaDC);
		DeleteObject(Player1AreaBM);
		DeleteDC(Player2AreaDC);
		DeleteObject(Player2AreaBM);
		DeleteDC(memDC);
		DeleteObject(memBM);

	}
	return 0;
}


LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_CHAR: {
		//Клавіші управління гравцем 1

		if (player1.status == 'p' && player2.ifchose) {

			switch (wp) {
			case 'w':
				player1.rotation = 'b';
				if (player1.y - 1 > 0 && Collision(player1.x, player1.y - 1, map1)) {
					player1.position = 'u';
					player1.y--;
					PlayerView(fogmap1, player1.x, player1.y);
					Sleep(100);
				}
				break;
			case 'a':
				player1.rotation = 'l';
				if (player1.x - 1 > 0 && Collision(player1.x - 1, player1.y, map1)) {
					player1.position = 'u';
					player1.x--;
					PlayerView(fogmap1, player1.x, player1.y);
					Sleep(100);
				}
				break;
			case 's':
				player1.rotation = 'f';
				if (player1.y + 1 <= 10 && Collision(player1.x, player1.y + 1, map1)) {
					player1.position = 'u';
					player1.y++;
					PlayerView(fogmap1, player1.x, player1.y);
					Sleep(100);
				}
				break;
			case 'd':
				player1.rotation = 'r';
				if (player1.x + 1 <= 10 && Collision(player1.x + 1, player1.y, map1)) {
					player1.position = 'u';
					player1.x++;
					PlayerView(fogmap1, player1.x, player1.y);
					Sleep(100);
				}
				break;
			case 'f':
				player1.position = 'd';
				break;
			case 'e': {
				if (player1.rotation == 'f' && player1.y < 10) {
					interaction(player1, player1.x, player1.y + 1, map1, IfCrossbowWork1, fogmap1);
				}
				if (player1.rotation == 'b' && player1.y > 1) {
					interaction(player1, player1.x, player1.y - 1, map1, IfCrossbowWork1, fogmap1);
				}
				if (player1.rotation == 'l' && player1.x > 1) {
					interaction(player1, player1.x - 1, player1.y, map1, IfCrossbowWork1, fogmap1);
				}
				if (player1.rotation == 'r' && player1.x < 10) {
					interaction(player1, player1.x + 1, player1.y, map1, IfCrossbowWork1, fogmap1);
				}
				break;
			}
			case 'q':
				FlashGranadeProccesing(player1, fogmap1, map1);
				break;

			}
			if (player1.x == 2 && player1.y == 3 && map1[2][1] == 'k') {
				player1.CroutchCount++;
				map1[2][1] = '_';
			}
			if (player1.treasure && ((player1.x == 5 && player1.y == 10) || (player1.x == 1 && player1.y == 5) || (player1.x == 6 && player1.y == 1) || (player1.x == 10 && player1.y == 6))) {
				player1.status = 'w';
				finish1 = GetTickCount();
				if (player2.status == 'w') {
				std::wstring wstr = L"Час Гравця 1:";
				wstr += std::to_wstring((finish1 - start) / 60000);
				wstr += L" хвилин ";
				wstr += std::to_wstring((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000);
				wstr += L" секунд ";
				wstr += std::to_wstring(finish1 - start - ((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000) * 1000);
				wstr += L" мілісекунд ";
				wstr += L"\nЧас Гравця 2:";
				wstr += std::to_wstring((finish1 - start) / 60000);
				wstr += L" хвилин ";
				wstr += std::to_wstring((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000);
				wstr += L" секунд ";
				wstr += std::to_wstring(finish2 - start - ((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000) * 1000);
				wstr += L" мілісекунд ";
				MessageBox(hWnd,wstr.c_str(), L"Переміг гравець 2", MB_OK);
				}
				if (player2.status == 'l') {
					std::wstring wstr = L"Час Гравця 1:";
					wstr += std::to_wstring((finish1 - start) / 60000);
					wstr += L" хвилин ";
					wstr += std::to_wstring((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000);
					wstr += L" секунд ";
					wstr += std::to_wstring(finish1 - start - ((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000) * 1000);
					wstr += L" мілісекунд ";
					MessageBox(hWnd, wstr.c_str(), L"Переміг гравець 1", MB_OK);
				}

			}
		}
		
		//Клавіші управління гравцем 2

		if (player2.status == 'p') {

			switch (wp) {
			case 'u':
				player2.rotation = 'b';
				if (player2.y - 1 > 0 && Collision(player2.x, player2.y - 1, map2)) {
					player2.position = 'u';
					player2.y--;
					PlayerView(fogmap2, player2.x, player2.y);
					Sleep(100);
				}
				break;
			case 'h':
				player2.rotation = 'l';
				if (player2.x - 1 > 0 && Collision(player2.x - 1, player2.y, map2)) {
					player2.position = 'u';
					player2.x--;
					PlayerView(fogmap2, player2.x, player2.y);
					Sleep(100);
				}
				break;
			case 'j':
				player2.rotation = 'f';
				if (player2.y + 1 <= 10 && Collision(player2.x, player2.y + 1, map2)) {
					player2.position = 'u';
					player2.y++;
					PlayerView(fogmap2, player2.x, player2.y);
					Sleep(100);
				}
				break;
			case 'k':
				player2.rotation = 'r';
				if (player2.x + 1 <= 10 && Collision(player2.x + 1, player2.y, map2)) {
					player2.position = 'u';
					player2.x++;
					PlayerView(fogmap2, player2.x, player2.y);
					Sleep(100);
				}
				break;
			case 'l':
				player2.position = 'd';
				break;
			case 'i': {
				if (player2.rotation == 'f' && player2.y < 10) {
					interaction(player2, player2.x, player2.y + 1, map2, IfCrossbowWork2, fogmap2);
				}
				if (player2.rotation == 'b' && player2.y > 1) {
					interaction(player2, player2.x, player2.y - 1, map2, IfCrossbowWork2, fogmap2);
				}
				if (player2.rotation == 'l' && player2.x > 1) {
					interaction(player2, player2.x - 1, player2.y, map2, IfCrossbowWork2, fogmap2);
				}
				if (player2.rotation == 'r' && player2.x < 10) {
					interaction(player2, player2.x + 1, player2.y, map2, IfCrossbowWork2, fogmap2);
				}
				break;
			}
			case 'y':
				FlashGranadeProccesing(player2, fogmap2, map2);
				break;

			}
			if (player2.x == 2 && player2.y == 3 && map2[2][1] == 'k') {
				player2.CroutchCount++;
				map2[2][1] = '_';
			}
			if (player2.treasure && ((player2.x == 5 && player2.y == 10) || (player2.x == 1 && player2.y == 5) || (player2.x == 6 && player2.y == 1) || (player2.x == 10 && player2.y == 6))) {
				player2.status = 'w';
				finish2 = GetTickCount();
				if (player1.status == 'w') {
					std::wstring wstr = L"Час Гравця 1:";
					wstr += std::to_wstring((finish1 - start) / 60000);
					wstr += L" хвилин ";
					wstr += std::to_wstring((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000);
					wstr += L" секунд ";
					wstr += std::to_wstring(finish1 - start - ((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000) * 1000);
					wstr += L" мілісекунд ";
					wstr += L"\nЧас Гравця 2:";
					wstr += std::to_wstring((finish2 - start) / 60000);
					wstr += L" хвилин ";
					wstr += std::to_wstring((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000);
					wstr += L" секунд ";
					wstr += std::to_wstring(finish2 - start - ((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000) * 1000);
					wstr += L" мілісекунд ";
					MessageBox(hWnd, wstr.c_str(), L"Переміг гравець 1", MB_OK);
				}
				if (player1.status == 'l') {
					std::wstring wstr =  L"Час Гравця 2:";
					wstr += std::to_wstring((finish2 - start) / 60000);
					wstr += L" хвилин ";
					wstr += std::to_wstring((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000);
					wstr += L" секунд ";
					wstr += std::to_wstring(finish2 - start - ((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000) * 1000);
					wstr += L" мілісекунд ";
					MessageBox(hWnd, wstr.c_str(), L"Переміг гравець 2", MB_OK);
				}
			}
		}
	
		break;}
		case WM_TIMER:
		{
			switch (wp) {
			case 22:
				KillTimer(hWnd, 22);
				if (player1.x == 8 && player1.y == 7) {
					player1.status = 'l';
					if (player2.status == 'w') {
						std::wstring wstr = L"Час Гравця 2:";
						wstr += std::to_wstring((finish2 - start) / 60000);
						wstr += L" хвилин ";
						wstr += std::to_wstring((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000);
						wstr += L" секунд ";
						wstr += std::to_wstring(finish2 - start - ((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000) * 1000);
						wstr += L" мілісекунд ";
						MessageBox(hWnd, wstr.c_str(), L"Переміг гравець 2", MB_OK);
					}
					if (player2.status == 'l') {
						MessageBox(hWnd, L"Обидва гравці програли", L"Ніхто не переміг", MB_OK);
					}
				}
				break;
			case 33:
				KillTimer(hWnd, 33);
				if (player2.x == 8 && player2.y == 7) {
					player2.status = 'l';

					if (player1.status == 'w') {
						std::wstring wstr = L"Час Гравця 1:";
						wstr += std::to_wstring((finish1 - start) / 60000);
						wstr += L" хвилин ";
						wstr += std::to_wstring((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000);
						wstr += L" секунд ";
						wstr += std::to_wstring(finish1 - start - ((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000) * 1000);
						wstr += L" мілісекунд ";
						MessageBox(hWnd, wstr.c_str(), L"Переміг гравець 1", MB_OK);
					}
					if (player1.status == 'l') {
						MessageBox(hWnd, L"Обидва гравці програли", L"Ніхто не переміг", MB_OK);
					}
				}
				break;
			case 1:
				if (time >= 3)time = 0;
				else time++;
				break;
				}
		
		}
		
	case WM_COMMAND: {
		switch(LOWORD(wp)) {
		case ID_START:
			lvl1(map1);
			fog(fogmap1);
			lvl1(map2);
			fog(fogmap2);
			player1.status = 'c';
			player2.status = 'c';
			player1.treasure = 0;
			player2.treasure = 0;
			player1.FashGranadeCount = 3;
			player2.FashGranadeCount = 3;
			IfCrossbowWork1 = true;
			IfCrossbowWork1 = true;
			CreateDialog(hInstForDialog, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)(Choose1DialogProc));
			break;
		case ID_RULES:
			CreateDialog(hInstForDialog, MAKEINTRESOURCE(IDD_RULES), hWnd, (DLGPROC)(RulesProc));
			break;
		case ID_ABOUT:
			CreateDialog(hInstForDialog, MAKEINTRESOURCE(IDD_ABOUT), hWnd, (DLGPROC)(RulesProc));
			break;
		}
		}break;
	case WM_DESTROY:
		PostQuitMessage(0);
			break;
	default:	return DefWindowProc(hWnd, msg, wp, lp);
	}
}

int CALLBACK Choose1DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//процетдура обробки діалогового вікна
{


	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case  WM_COMMAND:

		switch (wParam) {
		case ID_PLAYER1_BUTTON1:
			player1.x = 5;
			player1.y = 10;
			player1.status = 'p';
			player1.ifchose = true;
			PlayerView(fogmap1, player1.x, player1.y);
			EndDialog(hDlg, LOWORD(wParam));
			CreateDialog(hInstForDialog, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, (DLGPROC)(Choose2DialogProc));
			break;
		case ID_PLAYER1_BUTTON2:
			player1.x = 1;
			player1.y = 5;
			player1.status = 'p';
			player1.ifchose = true;
			PlayerView(fogmap1, player1.x, player1.y);
			EndDialog(hDlg, LOWORD(wParam));
			CreateDialog(hInstForDialog, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, (DLGPROC)(Choose2DialogProc));
			break;
		case ID_PLAYER1_BUTTON3:
			player1.x = 6;
			player1.y = 1;
			player1.status = 'p';
			player1.ifchose = true;
			PlayerView(fogmap1, player1.x, player1.y);
			EndDialog(hDlg, LOWORD(wParam));
			CreateDialog(hInstForDialog, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, (DLGPROC)(Choose2DialogProc));
			break;
		case ID_PLAYER1_BUTTON4:
			player1.x = 10;
			player1.y =6;
			player1.status = 'p';
			player1.ifchose = true;
			PlayerView(fogmap1, player1.x, player1.y);
			EndDialog(hDlg, LOWORD(wParam));
			CreateDialog(hInstForDialog, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, (DLGPROC)(Choose2DialogProc));
			break;
		}

	}
	return FALSE;
}
int CALLBACK Choose2DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//процетдура обробки діалогового вікна
{


	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case  WM_COMMAND:

		switch (wParam) {
		case ID_PLAYER2_BUTTON1:
			player2.x = 5;
			player2.y = 10;
			player2.status = 'p';
			player2.ifchose = true;
			PlayerView(fogmap2, player2.x, player2.y);
			start = GetTickCount();
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case ID_PLAYER2_BUTTON2:
			player2.x = 1;
			player2.y = 5;
			player2.status = 'p';
			player2.ifchose = true;
			PlayerView(fogmap2, player2.x, player2.y);
			start = GetTickCount();
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case ID_PLAYER2_BUTTON3:
			player2.x = 6;
			player2.y = 1;
			player2.status = 'p';
			player2.ifchose = true;
			PlayerView(fogmap2, player2.x, player2.y);
			start = GetTickCount();
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case ID_PLAYER2_BUTTON4:
			player2.x = 10;
			player2.y = 6;
			player2.status = 'p';
			player2.ifchose = true;
			PlayerView(fogmap2, player2.x, player2.y);
			start = GetTickCount();
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}

	}
	return FALSE;
}
int CALLBACK RulesProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//процетдура обробки діалогового вікна
{


	switch (message)
	{
	case WM_INITDIALOG:
		
		return TRUE;
		break;

	case  WM_COMMAND:

		switch (wParam) {
	
		}
		break;
		
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return FALSE;
}

void Draw(HDC &memDC, Player &player, char lvl[10][10], bool IfCrossbowWork, HWND hWnd, char fog[10][10]) {
	HDC bmpDC = CreateCompatibleDC(memDC);
	BITMAP bm;
	HBITMAP tempBM = hFloorBmp;
	

		
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (fog[i][j] == 'f')tempBM = hFogBmp;
				else{
					switch (lvl[i][j]) {
					case'_':
						tempBM = hFloorBmp;
						break;
					case'w':
						tempBM = hWallBmp;
						break;
					case'h':
						tempBM = hHoleBmp;
						break;
					case'f':
						tempBM = hFakeChestBmp;
						break;
					case't':
						tempBM = hTreasureChestBmp;
						break;
					case'c':
						tempBM = hCrossbowBmp;
						break;
					case'b':
						tempBM = hBeartrapBmp;
						break;
					case'k':
						tempBM = hCrutchBmp;
						break;
					case's':
						tempBM = hBeartrapClosedBmp;
						break;
					case'o':
						tempBM = hTreasureChestOpenBmp;
						break;
					default:
						tempBM = hTestBmp;
						break;
					}
					}
				SelectObject(bmpDC, tempBM);
				GetObject(tempBM, sizeof(bm), (LPSTR)&bm);
				if (player.x == 8 && player.y == 7 && lvl[6][7] == 'b') {
					BitBlt(memDC, j * 50, i * 50, bm.bmWidth, bm.bmHeight, bmpDC, 0, 0, NOTSRCCOPY);
				}else
					BitBlt(memDC, j * 50, i * 50, bm.bmWidth, bm.bmHeight, bmpDC, 0, 0, SRCCOPY);
			}
		}
		if(player.position =='d')tempBM = hPlayerDownBmp;
		else {
			switch (player.rotation) {
			case 'f':
				tempBM = hPlayerFrontBmp;
				break;
			case 'b':
				tempBM = hPlayerBackBmp;
				break;
			case 'l':
				tempBM = hPlayerLeftBmp;
				break;
			case 'r':
				tempBM = hPlayerRightBmp;
				break;
			default:
				tempBM = hTestBmp;
				break;
			}
		}
		SelectObject(bmpDC, tempBM);
		GetObject(tempBM, sizeof(bm), (LPSTR)&bm);
		BitBlt(memDC, (player.x-1) * 50, (player.y-1) * 50, bm.bmWidth, bm.bmHeight, bmpDC, 0, 0, SRCCOPY);
		//arrow
		if (IfCrossbowWork && fog[6][2+time] != 'f') {
			SelectObject(bmpDC, hArrowBmp);
			GetObject(hArrowBmp, sizeof(bm), (LPSTR)&bm);
			BitBlt(memDC, (2 + time) * 50, 6 * 50 + 17, 49, 13, bmpDC, 0, 0, SRCCOPY);

			if (player.position == 'u' && player.x == (3 + time) && player.y == 7) {
				player.status = 'l';
				if (player2.status == 'w' && player1.status == 'l') {
					std::wstring wstr = L"Час Гравця 2:";
					wstr += std::to_wstring((finish2 - start) / 60000);
					wstr += L" хвилин ";
					wstr += std::to_wstring((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000);
					wstr += L" секунд ";
					wstr += std::to_wstring(finish2 - start - ((finish2 - start - ((finish2 - start) / 60000) * 60000) / 1000) * 1000);
					wstr += L" мілісекунд ";
					MessageBox(hWnd, wstr.c_str(), L"Переміг гравець 2", MB_OK);
				}
				if (player1.status == 'w' && player2.status == 'l') {
					std::wstring wstr = L"Час Гравця 1:";
					wstr += std::to_wstring((finish1 - start) / 60000);
					wstr += L" хвилин ";
					wstr += std::to_wstring((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000);
					wstr += L" секунд ";
					wstr += std::to_wstring(finish1 - start - ((finish1 - start - ((finish1 - start) / 60000) * 60000) / 1000) * 1000);
					wstr += L" мілісекунд ";
					MessageBox(hWnd, wstr.c_str(), L"Переміг гравець 1", MB_OK);
				}
				if (player1.status == 'l' && player2.status == 'l') {
					MessageBox(hWnd, L"Обидва гравці програли", L"Ніхто не переміг", MB_OK);
				}
			}
		}


	DeleteDC(bmpDC);
}


void SetBitmaps(HINSTANCE hInst) {
	hTestBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TEST));
	hWallBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WALL));
	hFloorBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FLOOR));
	hHoleBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HOLE));
	hFakeChestBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FAKE_CHEST));
	hTreasureChestBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREASURE_CHEST));
	hTreasureChestOpenBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREASURE_CHEST_OPEN));
	hCrossbowBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CROSSBOW));
	hBeartrapBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BEARTRAP));
	hBeartrapClosedBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BEARTRAP_CLOSED));
	hPlayerFrontBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYER_FRONT));
	hPlayerBackBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYER_BACK));
	hPlayerRightBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYER_RIGHT));
	hPlayerLeftBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYER_LEFT));
	hPlayerDownBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYER_DOWN));
	hArrowBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ARROW));
	hCrutchBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CRUTCH));
	hFogBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOG));
	hLoseScreenBmp1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LOSE_SCREEN));
	hWinScreenBmp1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WIN_SCREEN));
	hLoseScreenBmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LOSE_SCREEN));
	hWinScreenBmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WIN_SCREEN));
	hChooseScreenBmp1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHOOSE_SCREEN));
	hChooseScreenBmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHOOSE_SCREEN));
	hBackgroundBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
	hNumber0Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER0));
	hNumber1Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER1));
	hNumber2Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER2));
	hNumber3Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER3));
	hCrouchIco0Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CRUTCHICO0));
	hCrouchIco1Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CRUTCHICO1));
	hTreasureIco0Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREASUREICO0));
	hTreasureIco1Bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREASUREICO1));
	hNumber0Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER0));
	hNumber1Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER1));
	hNumber2Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER2));
	hNumber3Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER3));
	hCrouchIco0Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CRUTCHICO0));
	hCrouchIco1Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CRUTCHICO1));
	hTreasureIco0Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREASUREICO0));
	hTreasureIco1Bmp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREASUREICO1));
}
bool Collision(int x, int y,char lvl[10][10]) {
	if (lvl[y-1][x-1] == 'w' || lvl[y-1][x-1] == 'c' || lvl[y - 1][x - 1] == 'h' || lvl[y - 1][x - 1] == 'f' || lvl[y - 1][x - 1] == 't' )return false;
	else return true;
}
void lvl1(char lvl[10][10]) {
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)lvl[i][j] = map[i][j];
	}
}
void fog(char fogmap[10][10]) {
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)fogmap[i][j] = 'f';
	}
}
void interaction(Player &player,int x, int y, char lvl[10][10], bool &IfCrossbowWork, char fog[10][10]) {
	//перша пара дірок
	if (x == 7 && y == 4) {
		lvl[1][8] = 'h';
		player.rotation = 'f';
		player.x = 9;
		player.y = 3;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 9, 2);
	}
	if (x == 9 && y == 2) {
		lvl[1][8] = 'h';
		player.rotation = 'l';
		player.x = 7;
		player.y = 3;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 7, 4);
	}
	//друга пара дірок
	if (x == 2 && y == 9) {
		lvl[8][1] = 'h';
		player.rotation = 'b';
		player.x = 9;
		player.y = 8;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 9, 9);
	}
	if (x == 9 && y == 9) {
		lvl[8][1] = 'h';
		player.rotation = 'r';
		player.x = 2;
		player.y = 8;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 2, 9);
	}
	//третя пара дірок
	if (x == 2 && y == 2) {
		player.rotation = 'b';
		player.x = 3;
		player.y = 8;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 3, 9);
	}
	if (x == 3 && y == 9) {
		player.rotation = 'r';
		player.x = 3;
		player.y = 2;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 2, 2);
	}
	//четверта пара дірок
	if (x == 6 && y == 8) {
		player.rotation = 'f';
		player.x = 7;
		player.y = 3;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 7, 2);
	}
	if (x == 7 && y == 2) {
		player.rotation = 'l';
		player.x = 5;
		player.y = 8;
		PlayerView(fog, player.x, player.y);
		PlayerView(fog, 6, 8);
	}
	//скриня з милицею
	if (x == 2 && y == 3) {
		lvl[2][1] = 'k';
	}
	//вимкнення арбалету
	if (x == 2 && y == 7) {
		IfCrossbowWork = false;
	}
	//скриня з скарбом
	if (x == 8 && y == 9 && lvl[8][7] == 't') {
		player.treasure = true;
		lvl[8][7] = 'o';
	}
}
void PlayerView(char fog[10][10], int playerx, int playery) {
	int x = playerx - 1;
	int y = playery - 1;
	fog[y][x] = ' ';
	if (y > 0)fog[y-1][x] = ' ';
	if (y > 0 && x > 0)fog[y-1][x-1] = ' ';
	if (y > 0 && x < 9)fog[y-1][x+1] = ' ';
	if (y < 9)fog[y+1][x] = ' ';
	if (y < 9 && x > 0)fog[y+1][x-1] = ' ';
	if (y < 9 && x < 9)fog[y+1][x+1] = ' ';
	if (x > 0)fog[y][x-1] = ' ';
	if (x < 9)fog[y][x+1] = ' ';
}
void FlashGranadeProccesing(Player& player, char fog[10][10], char lvl[10][10]) {
	if (player.FashGranadeCount > 0) {
		player.FashGranadeCount--;
		int x = player.x;
		int y = player.y;
		if (player.rotation == 'b') {
			while (Collision(x, y, lvl) && y > 1) {
				fog[y - 1][x - 1] = ' ';
				y--;
			}
			y++;
		}
		if (player.rotation == 'f') {
			while (Collision(x, y, lvl) && y < 10) {
				fog[y - 1][x - 1] = ' ';
				y++;
			}
			y--;
		}
		if (player.rotation == 'l') {
			while (Collision(x, y, lvl) && x > 1) {
				fog[y - 1][x - 1] = ' ';
				x--;
			}
			x++;
		}
		if (player.rotation == 'r') {
			while (Collision(x, y, lvl) && x < 10) {
				fog[y - 1][x - 1] = ' ';
				x++;
			}
			x--;
		}
		if(Collision(x, y - 1, lvl) && y - 1 >= 1) {
			PlayerView(fog, x, y - 1);
		}
		if (Collision(x, y + 1, lvl) && y + 1 <= 10) {
			PlayerView(fog, x, y + 1);
		}
		if (Collision(x - 1, y, lvl) && x - 1 >= 1) {
			PlayerView(fog, x - 1, y);
		}
		if (Collision(x + 1, y, lvl) && x + 1 <= 10) {
			PlayerView(fog, x + 1, y);
		}
	}
}
