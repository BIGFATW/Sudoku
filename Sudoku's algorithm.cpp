#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>
using namespace std;
vector<int> lay[9][9];
bool ended;
#define IN(hy,hx) (x==hx&&y==hy)
#define ON_A_LINE(x1,y1,x2,y2) (x1==x2 || y1==y2)
#define IN_A_BLOCK(x1,y1,x2,y2) (x1/3==x2/3 && y1/3==y2/3)//(BLOCK(x1,y1)==BLOCK(x2,y2))
#define RELATED(x1,y1,x2,y2) (ON_A_LINE(x1,y1,x2,y2) || IN_A_BLOCK(x1,y1,x2,y2))
#define SURED(x,y) (lay[x][y].size()!=1 ? -1 : lay[x][y][0])
#define CONFIRM(x,y,NUM)lay[x][y].clear();lay[x][y].push_back(NUM);
#define MoveTo(veca,vecb)for(int mi=0;mi<9;mi++) for(int mj=0;mj<9;mj++) vecb[mi][mj]=veca[mi][mj];
inline int BLOCK(int x, int y)
{
    if (IN(0, 0) || IN(0, 1) || IN(0, 2) || IN(0, 3) || IN(0, 4) || IN(1, 1) || IN(1, 2) || IN(1, 3) || IN(2, 2)) return 1;
    if (IN(1, 0) || IN(2, 0) || IN(3, 0) || IN(4, 0) || IN(5, 0) || IN(2, 1) || IN(3, 1) || IN(4, 1) || IN(3, 2)) return 2;
    if (IN(6, 0) || IN(7, 0) || IN(8, 0) || IN(5, 1) || IN(6, 1) || IN(7, 1) || IN(4, 2) || IN(5, 2) || IN(6, 2)) return 3;
    if (IN(2, 3) || IN(1, 4) || IN(2, 4) || IN(0, 5) || IN(1, 5) || IN(2, 5) || IN(0, 6) || IN(1, 6) || IN(0, 7)) return 4;
    if (IN(3, 3) || IN(4, 3) || IN(5, 3) || IN(3, 4) || IN(4, 4) || IN(5, 4) || IN(3, 5) || IN(4, 5) || IN(5, 5)) return 5;
    if (IN(8, 1) || IN(7, 2) || IN(8, 2) || IN(6, 3) || IN(7, 3) || IN(8, 3) || IN(6, 4) || IN(7, 4) || IN(6, 5)) return 6;
    if (IN(2, 6) || IN(3, 6) || IN(4, 6) || IN(1, 7) || IN(2, 7) || IN(3, 7) || IN(0, 8) || IN(1, 8) || IN(2, 8)) return 7;
    if (IN(5, 6) || IN(4, 7) || IN(5, 7) || IN(6, 7) || IN(3, 8) || IN(4, 8) || IN(5, 8) || IN(6, 8) || IN(7, 8)) return 8;
    if (IN(8, 4) || IN(7, 5) || IN(8, 5) || IN(6, 6) || IN(7, 6) || IN(8, 6) || IN(7, 7) || IN(8, 7) || IN(8, 8)) return 9;
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
inline bool FAILED()
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
inline void OUT()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cout << lay[i][j][0] << " ";
        }
        cout << endl;
    }

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
inline void RAND()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            lay[i][j] = { 1,2,3,4,5,6,7,8,9 };
    ::srand((unsigned)time(NULL));
    for (int times = 0; times < 5; times++)
    {
        int ranx = rand() % 9, rany = rand() % 9;
        if (SURED(ranx, rany) != -1)    continue;
        lay[ranx][rany] = ABLED(ranx, rany);
        int lo = rand() % lay[ranx][rany].size();
        int num = lay[ranx][rany][lo];
        CONFIRM(ranx, rany, num);
    }
}
inline void init()
{
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
    if (DONE() || FAILED()) return;
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
        int num = lay[xgoal][ygoal][i];
        vector<int> para[9][9];
        MoveTo(lay, para);
        CONFIRM(xgoal, ygoal, num);
        init();
        if (DONE()) return;
        MoveTo(para, lay);
    }
}
int main(int argc, const char* argv[])
{
    RAND();
    init();
    OUT();
    ::system("PAUSE");
    return 0;
}
