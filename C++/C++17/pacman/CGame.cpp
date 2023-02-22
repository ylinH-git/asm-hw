#include "CGame.h"
#include <string>
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include "CBackground.h"
#include "common_def.h"
#include <stdlib.h>
#include <time.h>

using namespace std;
CGame::CGame(): m_pacman()
{
    srand(time(NULL));
	Init();
}

void CGame::Init()
{
    initgraph(GAME_WIDTH, GAME_HEIGHT + 40);
    // 初始化背景
    m_bg = CBackground(_T(".//res//background.png"));
    m_bg.Paint();
    outtextxy(BLOCK_SIZE / 2, 27 * BLOCK_SIZE, _T("score:"));
    // 初始化吃豆人
    m_pacman.SetImage(_T(".//res//pacman.png"), 4, 3);
    m_pacman.SetPosition(10 * BLOCK_SIZE - PERSON_SIZE / 2, 20 * BLOCK_SIZE - PERSON_SIZE / 2);
    m_pacman.SetDir(RIGHT_KEY);
    m_pacman.Paint();
    // 初始化怪物
    m_arrMonster[0].SetImage(_T(".//res//blue.png"), 4, 2);
    m_arrMonster[1].SetImage(_T(".//res//pink.png"), 4, 2);
    m_arrMonster[2].SetImage(_T(".//res//red.png"), 4, 2);
    m_arrMonster[3].SetImage(_T(".//res//orange.png"), 4, 2);

    m_arrMonster[0].SetPosition(1 * BLOCK_SIZE - PERSON_SIZE / 2, 1 * BLOCK_SIZE - PERSON_SIZE / 2);
    m_arrMonster[0].SetDir(RIGHT_KEY);
    m_arrMonster[0].Paint();

    m_arrMonster[1].SetPosition(19 * BLOCK_SIZE - PERSON_SIZE / 2, 1 * BLOCK_SIZE - PERSON_SIZE / 2);
    m_arrMonster[1].SetDir(LEFT_KEY);
    m_arrMonster[1].Paint();

    m_arrMonster[2].SetPosition(19 * BLOCK_SIZE - PERSON_SIZE / 2, 25 * BLOCK_SIZE - PERSON_SIZE / 2);
    m_arrMonster[2].SetDir(LEFT_KEY);
    m_arrMonster[2].Paint();

    m_arrMonster[3].SetPosition(1 * BLOCK_SIZE - PERSON_SIZE / 2, 25 * BLOCK_SIZE - PERSON_SIZE / 2);
    m_arrMonster[3].SetDir(RIGHT_KEY);
    m_arrMonster[3].Paint();
    outtextxy(3 * BLOCK_SIZE + BLOCK_SIZE / 2, 27 * BLOCK_SIZE, to_wstring(m_pacman.GetScore()).c_str());

    outtextxy(6 * BLOCK_SIZE + BLOCK_SIZE / 2, 27 * BLOCK_SIZE, _T("回车  关闭游戏"));
}

void CGame::Close()
{
    closegraph();
}

void CGame::Run(int cmd)
{
    for (int i = 0; i < 4; i++)
    {
        if (m_arrMonster[i].Eat(m_pacman))
        {
            outtextxy(BLOCK_SIZE / 2, 28 * BLOCK_SIZE, _T("Game over"));
            return;
        }
    }
    m_pacman.Clear(m_bg.GetBG());
    for (int i = 0; i < 4; i++)
    {
        m_arrMonster[i].Clear(m_bg.GetBGWBEAN());
    }
    m_pacman.Update(cmd);
    for (int i = 0; i < 4; i++)
    {
        m_arrMonster[i].Update();
    }
    m_pacman.Paint();
    for (int i = 0; i < 4; i++)
    {
        m_arrMonster[i].Paint();
    }
    m_pacman.Eat();
    outtextxy(3 * BLOCK_SIZE + BLOCK_SIZE / 2, 27 * BLOCK_SIZE, to_wstring(m_pacman.GetScore()).c_str());
    // 刷新显存
    FlushBatchDraw();
}
