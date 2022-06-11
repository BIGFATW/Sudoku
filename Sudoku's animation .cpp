#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>
#include<Windows.h>
#include<conio.h>
using namespace std;
vector<int> lay[9][9];
bool ended;
int restart = 0;
#define INN(hy,hx) (x==hx&&y==hy)
#define ON_A_LINE(x1,y1,x2,y2) (x1==x2 || y1==y2)
#define IN_A_BLOCK(x1,y1,x2,y2) (BLOCK(x1,y1)==BLOCK(x2,y2))//(x1/3==x2/3 && y1/3==y2/3)
#define RELATED(x1,y1,x2,y2) (ON_A_LINE(x1,y1,x2,y2) || IN_A_BLOCK(x1,y1,x2,y2))
#define SURED(x,y) (lay[x][y].size()!=1 ? -1 : lay[x][y][0])
#define REMOVE(x,y,ONE) lay[x][y].erase(find(lay[x][y].begin(), lay[x][y].end(), ONE), find(lay[x][y].begin(), lay[x][y].end(), ONE) + 1);
#define CONFIRM(x,y,NUM)                                    \
lay[x][y].clear();                                            \
lay[x][y].push_back(NUM);
#define MoveTo(veca,vecb)\
for(int mi=0;mi<9;mi++)\
for(int mj=0;mj<9;mj++) vecb[mi][mj]=veca[mi][mj];
inline int BLOCK(int x, int y)
{
    if (INN(0, 0) || INN(0, 1) || INN(0, 2) || INN(0, 3) || INN(0, 4) || INN(1, 1) || INN(1, 2) || INN(1, 3) || INN(2, 2)) return 1;
    if (INN(1, 0) || INN(2, 0) || INN(3, 0) || INN(4, 0) || INN(5, 0) || INN(2, 1) || INN(3, 1) || INN(4, 1) || INN(3, 2)) return 2;
    if (INN(6, 0) || INN(7, 0) || INN(8, 0) || INN(5, 1) || INN(6, 1) || INN(7, 1) || INN(4, 2) || INN(5, 2) || INN(6, 2)) return 3;
    if (INN(2, 3) || INN(1, 4) || INN(2, 4) || INN(0, 5) || INN(1, 5) || INN(2, 5) || INN(0, 6) || INN(1, 6) || INN(0, 7)) return 4;
    if (INN(3, 3) || INN(4, 3) || INN(5, 3) || INN(3, 4) || INN(4, 4) || INN(5, 4) || INN(3, 5) || INN(4, 5) || INN(5, 5)) return 5;
    if (INN(8, 1) || INN(7, 2) || INN(8, 2) || INN(6, 3) || INN(7, 3) || INN(8, 3) || INN(6, 4) || INN(7, 4) || INN(6, 5)) return 6;
    if (INN(2, 6) || INN(3, 6) || INN(4, 6) || INN(1, 7) || INN(2, 7) || INN(3, 7) || INN(0, 8) || INN(1, 8) || INN(2, 8)) return 7;
    if (INN(5, 6) || INN(4, 7) || INN(5, 7) || INN(6, 7) || INN(3, 8) || INN(4, 8) || INN(5, 8) || INN(6, 8) || INN(7, 8)) return 8;
    if (INN(8, 4) || INN(7, 5) || INN(8, 5) || INN(6, 6) || INN(7, 6) || INN(8, 6) || INN(7, 7) || INN(8, 7) || INN(8, 8)) return 9;
}
inline bool DONE()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (SURED(i, j) == -1)
            {
                return 0;
            }
        }
    }
    return 1;
}
inline bool FAIL()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!lay[i][j].size())
            {
                return 1;
            }
        }
    }
    return 0;

}
inline vector<int> ABLED(int x, int y)
{
    vector<int> RESULT = { 1,2,3,4,5,6,7,8,9 };
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (x == i && y == j) continue;
            if (!RELATED(x, y, i, j)) continue;
            if (SURED(i, j) == -1) continue;
            vector<int>::iterator LOCATION = find(RESULT.begin(), RESULT.end(), SURED(i, j));
            if (LOCATION == RESULT.end()) continue;
            RESULT.erase(LOCATION, LOCATION + 1);
        }
    }
    if (RESULT != lay[x][y]) ended = 0;
    return RESULT;
}
void init(int step = 0)
{
    ended = 0;
    while (!ended)
    {
        ended = 1;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (SURED(i, j) == -1)
                    lay[i][j] = ABLED(i, j);
            }
        }
    }
    if (DONE() || FAIL()) return;
    int xgoal = 0, ygoal = 0, sizegoal = 9;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if ((sizegoal > lay[i][j].size()) && SURED(i, j) == -1)
            {
                xgoal = i, ygoal = j;
                sizegoal = lay[xgoal][ygoal].size();
            }
        }
    }
    lay[xgoal][ygoal] = ABLED(xgoal, ygoal);
    for (int i = 0; i < lay[xgoal][ygoal].size(); i++)
    {
        vector<int> para[9][9];
        MoveTo(lay, para)
            int nu = lay[xgoal][ygoal][i];
        CONFIRM(xgoal, ygoal, nu)
            restart++;
        SetConsoleCursorPosition(GetStdHandle((((DWORD)-11))), { 0,0 });
        for (int hm = 0;hm < 9;hm++)
        {
            for (int hn = 0;hn < 9;hn++) { if (SURED(hm, hn) == -1) cout << "N ";else cout << lay[hm][hn][0] << " "; }
            cout << endl;
        }
        cout << endl;
        //Sleep(1);
        init(step + 1);
        if (DONE()) return;
        MoveTo(para, lay)
    }
}
int main(int argc, const char* argv[])
{
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(GetStdHandle((STD_OUTPUT_HANDLE)), &info);
    info.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle((STD_OUTPUT_HANDLE)), &info);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            lay[i][j] = { 1,2,3,4,5,6,7,8,9 };
        }
    }
    srand((unsigned)time(NULL));
    for (int times = 0; times < 5; times++)
    {
        int ranx = rand() % 9, rany = rand() % 9;
        if (SURED(ranx, rany) != -1)    continue;
        lay[ranx][rany] = ABLED(ranx, rany);
        int lo = rand() % lay[ranx][rany].size();
        int num = lay[ranx][rany][lo];
        CONFIRM(ranx, rany, num);
    }
    init();
    SetConsoleCursorPosition(GetStdHandle((((DWORD)-11))), { 0,0 });
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cout << lay[i][j][0] << " ";
        }
        cout << endl;
    }
    cout << "try * " << restart << endl;
    ::_getch();
    return 0;
}
