#include<Windows.h>
#include"Sudoku.h"
#define KEY_DOWN(VK_N) (GetAsyncKeyState(VK_N) & 0x8000)
#define Line(x1,y1,x2,y2)MoveToEx(hdc,x1,y1,nullptr);LineTo(hdc,x2,y2);
#define generate if(mode==2){HINSTANCE hRslt; hRslt = ShellExecute(NULL, L"open", L"Sudoku generator.exe", NULL, NULL, SW_SHOWNORMAL); assert(hRslt > (HINSTANCE)HINSTANCE_ERROR); }
int selectx = -1, selecty = -1, selectv = -1, chances, inputnum = -1, game = 0, mode = 0;
float timeSS = 0;
bool WIN()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (!out[i][j].display) return 0;
	return 1;
}
LRESULT CALLBACK WindowProc(HWND window, UINT umsg, WPARAM wp, LPARAM lp);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc = { 0 };
	wc.lpszClassName = L"Sudoku by Rooney";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	RegisterClass(&wc);
	HWND window = CreateWindow(L"Sudoku by Rooney", L"Sudoku", WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 420, 540, 0, 0, hInstance, 0);
	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);
	MSG msg;
	bool __win = 0, __lose = 0;
	hok = CreateWindow(L"BUTTON", L"OK", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 20, 450, 80, 30, window, (HMENU)_OK, 0, 0);
	hcancel = CreateWindow(L"BUTTON", L"CANCEL", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 110, 450, 80, 30, window, (HMENU)_CANCEL, 0, 0);
	hrestart = CreateWindow(L"BUTTON", L"RESTART", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 450, 80, 30, window, (HMENU)_RESTART, 0, 0);
	hreturn = CreateWindow(L"BUTTON", L"RETURN", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 290, 450, 80, 30, window, (HMENU)_RETURN, 0, 0);
	hone = CreateWindow(L"BUTTON", L"1", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 20, 390, 30, 30, window, (HMENU)_ONE, 0, 0);
	htwo = CreateWindow(L"BUTTON", L"2", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 60, 390, 30, 30, window, (HMENU)_TWO, 0, 0);
	hthree = CreateWindow(L"BUTTON", L"3", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 100, 390, 30, 30, window, (HMENU)_THREE, 0, 0);
	hfour = CreateWindow(L"BUTTON", L"4", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 140, 390, 30, 30, window, (HMENU)_FOUR, 0, 0);
	hfive = CreateWindow(L"BUTTON", L"5", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 180, 390, 30, 30, window, (HMENU)_FIVE, 0, 0);
	hsix = CreateWindow(L"BUTTON", L"6", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 220, 390, 30, 30, window, (HMENU)_SIX, 0, 0);
	hseven = CreateWindow(L"BUTTON", L"7", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 260, 390, 30, 30, window, (HMENU)_SEVEN, 0, 0);
	height = CreateWindow(L"BUTTON", L"8", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 300, 390, 30, 30, window, (HMENU)_EIGHT, 0, 0);
	hnine = CreateWindow(L"BUTTON", L"9", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 390, 30, 30, window, (HMENU)_NINE, 0, 0);
	hclassic = CreateWindow(L"BUTTON", L"Classic Mode", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 145, 130, 35, window, (HMENU)_CLASSIC, 0, 0);
	hirregular = CreateWindow(L"BUTTON", L"Irregular Mode", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 310, 130, 35, window, (HMENU)_IRREGULAR, 0, 0);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (chances == 0) __lose = 1;
		if (WIN() && game) __win = 1;
		if (__win)
		{
			game = 0;
			char strmsg[36];
			sprintf_s(strmsg, "You win!\nYou used %.1f seconds!", timeSS);
			TCHAR outmsg[36];
			MultiByteToWideChar(CP_ACP, 0, strmsg, strlen(strmsg), outmsg, strlen(strmsg));
			MessageBox(window, outmsg, L"You win", MB_OK);
			__win = 0;
		}
		if (__lose)
		{
			MessageBox(window, L"You lose,Good Luck at Next time!", L"You lose", MB_OK);
			selectx = selecty = selectv = inputnum = -1;
			chances = 3;
			timeSS = 0;
			generate;
			prepare(mode);
			InvalidateRect(window, 0, 1);
			__lose = 0;
		}
	}
}
int NUM(int n)
{
	int result = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (out[i][j].display && out[i][j].value == n) result++;
		}
	}
	return result;
}
LRESULT CALLBACK WindowProc(HWND window, UINT umsg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	int px, py;
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		int input = LOWORD(wp) - '0';
		if (input < 1 || input>9) break;
		selectv = input;
		InvalidateRect(window, 0, 1);
		break;
	}
	case WM_COMMAND:
		if (mode)
		{
			switch (LOWORD(wp))
			{
			case _OK:
				if (selectx == -1 || selecty == -1 || selectv == -1) break;
				if (out[selectx][selecty].value == selectv)
				{
					out[selectx][selecty].display = 1;
					selectv = selectx = selecty = inputnum = -1;
					InvalidateRect(window, 0, 1);
				}
				else
				{
					MessageBox(window, L"You input the wrong number!", L"warning", MB_ICONWARNING);
					chances--;
					selectv = selectx = selecty = inputnum = -1;
					InvalidateRect(window, 0, 1);
				}
				break;
			case _CANCEL:
				selectx = selecty = selectv = inputnum = -1;
				InvalidateRect(window, 0, 1);
				break;
			case _RESTART:
				selectx = selecty = selectv = inputnum = -1;
				chances = 3;
				timeSS = 0;
				game = 1;
				generate;
				prepare(mode);
				InvalidateRect(window, 0, 1);
				break;
			case _RETURN:
				mode = 0;
				InvalidateRect(window, 0, 1);
				break;
			case _ONE:
				if (selectx != -1 && selecty != -1) { selectv = 1; }
				inputnum = 1; InvalidateRect(window, 0, 1);
				break;
			case _TWO:
				if (selectx != -1 && selecty != -1) { selectv = 2; }
				inputnum = 2; InvalidateRect(window, 0, 1);
				break;
			case _THREE:
				if (selectx != -1 && selecty != -1) { selectv = 3; }
				inputnum = 3; InvalidateRect(window, 0, 1);
				break;
			case _FOUR:
				if (selectx != -1 && selecty != -1) { selectv = 4; }
				inputnum = 4; InvalidateRect(window, 0, 1);
				break;
			case _FIVE:
				if (selectx != -1 && selecty != -1) { selectv = 5; }
				inputnum = 5; InvalidateRect(window, 0, 1);
				break;
			case _SIX:
				if (selectx != -1 && selecty != -1) { selectv = 6; }
				inputnum = 6; InvalidateRect(window, 0, 1);
				break;
			case _SEVEN:
				if (selectx != -1 && selecty != -1) { selectv = 7; }
				inputnum = 7; InvalidateRect(window, 0, 1);
				break;
			case _EIGHT:
				if (selectx != -1 && selecty != -1) { selectv = 8; }
				inputnum = 8; InvalidateRect(window, 0, 1);
				break;
			case _NINE:
				if (selectx != -1 && selecty != -1) { selectv = 9; }
				inputnum = 9; InvalidateRect(window, 0, 1);
				break;
			}
		}
		else
		{
			switch (LOWORD(wp))
			{
			case _CLASSIC:
				mode = 1;
				break;
			case _IRREGULAR:
				mode = 2;
				break;
			}
			selectx = selecty = selectv = inputnum = -1;
			chances = 3;
			timeSS = 0;
			game = 1;
			generate;
			prepare(mode);
			InvalidateRect(window, 0, 1);
		}
		break;
	case WM_TIMER:
		if (game)
		{
			timeSS += 0.1;
			RECT rec;
			rec.left = 220;
			rec.top = 480;
			rec.right = 300;
			rec.bottom = 500;
			InvalidateRect(window, &rec, 1);
		}
		break;
	case WM_CREATE:
		generate;
		prepare(mode);
		timeSS = 0;
		chances = 3;
		game = 0;
		SetTimer(window, 1, 100, NULL);
		break;
	case WM_LBUTTONDOWN:
		px = LOWORD(lp), py = HIWORD(lp);
		selectv = -1;
		if (px < 20 || px > 380 || py < 20 || py > 380 || out[(px - 20) / 40][(py - 20) / 40].display)
		{
			selectx = selecty = inputnum = -1;
			InvalidateRect(window, 0, 1);
			break;
		}
		selectx = (px - 20) / 40;
		selecty = (py - 20) / 40;
		if (inputnum != -1)selectv = inputnum;
		InvalidateRect(window, 0, 1);
		break;
	case WM_PAINT:
	{
		if (mode)
		{
			ShowWindow(hok, SW_SHOW);
			ShowWindow(hcancel, SW_SHOW);
			ShowWindow(hrestart, SW_SHOW);
			ShowWindow(hreturn, SW_SHOW);
			ShowWindow(hone, SW_SHOW);
			ShowWindow(htwo, SW_SHOW);
			ShowWindow(hthree, SW_SHOW);
			ShowWindow(hfour, SW_SHOW);
			ShowWindow(hfive, SW_SHOW);
			ShowWindow(hsix, SW_SHOW);
			ShowWindow(hseven, SW_SHOW);
			ShowWindow(height, SW_SHOW);
			ShowWindow(hnine, SW_SHOW);
			ShowWindow(hclassic, SW_HIDE);
			ShowWindow(hirregular, SW_HIDE);
			LOGFONT lfont, numfont;
			ZeroMemory(&lfont, sizeof(lfont));
			lfont.lfHeight = 30;            numfont.lfHeight = 20;
			lfont.lfWeight = 900;           numfont.lfWeight = 100;
			lfont.lfCharSet = 0;            numfont.lfCharSet = 0;
			lstrcpy(lfont.lfFaceName, L"Times New Roman");
			lstrcpy(numfont.lfFaceName, L"Times New Roman");
			HFONT font = CreateFontIndirect(&lfont), ofont = CreateFontIndirect(&numfont);
			char strchances[30], strtimes[80];
			sprintf_s(strchances, "spare chances: %d", chances);
			sprintf_s(strtimes, "Time : %.1f", timeSS);
			TCHAR outchances[30], outtimes[80];
			MultiByteToWideChar(CP_ACP, 0, strchances, strlen(strchances) + 1, outchances, strlen(strchances) + 1);
			MultiByteToWideChar(CP_ACP, 0, strtimes, strlen(strtimes), outtimes, strlen(strtimes));
			HPEN redpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)), bluepen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0)), black = CreatePen(0, 0, RGB(0, 0, 0)),
				numpen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
			HDC hdc = BeginPaint(window, &ps);
			TextOut(hdc, 20, 480, outchances, strlen(strchances));
			TextOut(hdc, 220, 480, outtimes, strlen(strtimes));
			if (mode == 2)
			{
				SelectObject(hdc, bluepen);
				for (int i = -1; i < 9; i++)
				{
					for (int j = -1; j < 9; j++)
					{
						int a1 = BLOCK(i, j);
						int a2 = BLOCK(i + 1, j);
						int a3 = BLOCK(i, j + 1);
						if (a1 != a2)
						{
							Line(i * 40 + 60, j * 40 + 20, i * 40 + 60, j * 40 + 60);
						}
						if (a1 != a3)
						{
							Line(i * 40 + 20, j * 40 + 60, i * 40 + 60, j * 40 + 60);
						}
					}
				}
				SelectObject(hdc, black);
			}
			for (int i = 1; i <= 9; i++)
			{
				switch (NUM(i))
				{
				case 0: TextOut(hdc, i * 40 - 10, 420, L"0", 1); break;
				case 1: TextOut(hdc, i * 40 - 10, 420, L"1", 1); break;
				case 2: TextOut(hdc, i * 40 - 10, 420, L"2", 1); break;
				case 3: TextOut(hdc, i * 40 - 10, 420, L"3", 1); break;
				case 4: TextOut(hdc, i * 40 - 10, 420, L"4", 1); break;
				case 5: TextOut(hdc, i * 40 - 10, 420, L"5", 1); break;
				case 6: TextOut(hdc, i * 40 - 10, 420, L"6", 1); break;
				case 7: TextOut(hdc, i * 40 - 10, 420, L"7", 1); break;
				case 8: TextOut(hdc, i * 40 - 10, 420, L"8", 1); break;
				case 9: TextOut(hdc, i * 40 - 10, 420, L"9", 1); break;
				}
			}
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
					LineTo(hdc, i * 40 + 60, j * 40 + 20);
					MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
					LineTo(hdc, i * 40 + 20, j * 40 + 60);
					MoveToEx(hdc, i * 40 + 60, j * 40 + 60, nullptr);
					LineTo(hdc, i * 40 + 60, j * 40 + 20);
					MoveToEx(hdc, i * 40 + 60, j * 40 + 60, nullptr);
					LineTo(hdc, i * 40 + 20, j * 40 + 60);
					if (!(i % 3) && !(j % 3) && mode != 2)
					{
						SelectObject(hdc, bluepen);
						MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
						LineTo(hdc, i * 40 + 140, j * 40 + 20);
						MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
						LineTo(hdc, i * 40 + 20, j * 40 + 140);
						MoveToEx(hdc, i * 40 + 140, j * 40 + 140, nullptr);
						LineTo(hdc, i * 40 + 140, j * 40 + 20);
						MoveToEx(hdc, i * 40 + 140, j * 40 + 140, nullptr);
						LineTo(hdc, i * 40 + 20, j * 40 + 140);
						SelectObject(hdc, black);
					}
					if (i == selectx && j == selecty)
					{
						SelectObject(hdc, redpen);
						MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
						LineTo(hdc, i * 40 + 60, j * 40 + 20);
						MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
						LineTo(hdc, i * 40 + 20, j * 40 + 60);
						MoveToEx(hdc, i * 40 + 60, j * 40 + 60, nullptr);
						LineTo(hdc, i * 40 + 60, j * 40 + 20);
						MoveToEx(hdc, i * 40 + 60, j * 40 + 60, nullptr);
						LineTo(hdc, i * 40 + 20, j * 40 + 60);
						SelectObject(hdc, black);
					}
					if ((out[i][j].display && out[i][j].value == inputnum) || (selectx == i && selecty == j && selectv == inputnum && selectv != -1))
					{
						SelectObject(hdc, numpen);
						MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
						LineTo(hdc, i * 40 + 60, j * 40 + 20);
						MoveToEx(hdc, i * 40 + 20, j * 40 + 20, nullptr);
						LineTo(hdc, i * 40 + 20, j * 40 + 60);
						MoveToEx(hdc, i * 40 + 60, j * 40 + 60, nullptr);
						LineTo(hdc, i * 40 + 60, j * 40 + 20);
						MoveToEx(hdc, i * 40 + 60, j * 40 + 60, nullptr);
						LineTo(hdc, i * 40 + 20, j * 40 + 60);
						SelectObject(hdc, black);
					}
					if (out[i][j].display)
					{
						SelectObject(hdc, font);
						switch (out[i][j].value)
						{
						case 1: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"1", 1); break;
						case 2: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"2", 1); break;
						case 3: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"3", 1); break;
						case 4: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"4", 1); break;
						case 5: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"5", 1); break;
						case 6: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"6", 1); break;
						case 7: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"7", 1); break;
						case 8: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"8", 1); break;
						case 9: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"9", 1); break;
						}
					}
					if (i == selectx && j == selecty)
					{
						switch (selectv)
						{
						case 1: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"1", 1); break;
						case 2: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"2", 1); break;
						case 3: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"3", 1); break;
						case 4: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"4", 1); break;
						case 5: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"5", 1); break;
						case 6: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"6", 1); break;
						case 7: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"7", 1); break;
						case 8: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"8", 1); break;
						case 9: TextOut(hdc, i * 40 + 30, j * 40 + 25, L"9", 1); break;
						}
					}
				}
			}
			EndPaint(window, &ps);
		}
		else
		{
			ShowWindow(hok, SW_HIDE);
			ShowWindow(hcancel, SW_HIDE);
			ShowWindow(hrestart, SW_HIDE);
			ShowWindow(hreturn, SW_HIDE);
			ShowWindow(hone, SW_HIDE);
			ShowWindow(htwo, SW_HIDE);
			ShowWindow(hthree, SW_HIDE);
			ShowWindow(hfour, SW_HIDE);
			ShowWindow(hfive, SW_HIDE);
			ShowWindow(hsix, SW_HIDE);
			ShowWindow(hseven, SW_HIDE);
			ShowWindow(height, SW_HIDE);
			ShowWindow(hnine, SW_HIDE);
			ShowWindow(hclassic, SW_SHOW);
			ShowWindow(hirregular, SW_SHOW);
		}
		break;
	}
	case WM_CLOSE:
		DestroyWindow(window);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(window, umsg, wp, lp);
}