#include "CGame.h"
#include <iostream>

CGame::CGame()
{
    Init();
}

void CGame::Init()
{
    system("COLOR 02");
    //初始化背景
    for (size_t nRow = 0; nRow < BACK_ROWS; nRow++)
    {
        for (size_t nCol = 0; nCol < BACK_COLS; nCol++)
        {
            if ((nCol == 0) || (nCol == BACK_COLS - 1) || (nRow == BACK_ROWS - 1))
            {
                m_aryBack[nRow][nCol] = wall;
            }
            else
            {
                m_aryBack[nRow][nCol] = emty;
            }
        }
    }
    m_isExceed = false;
    m_nGoal = 0;
    // 初始化方块
    NewBlock();
    // 使用初始化方块
    UseNewBlock();
    // 创建下一个新方块
    NewBlock();
}

void CGame::UseNewBlock()
{
    m_nCubeType = m_nNextCubeType;
    m_nCubeIdx = m_nNextCubeIdx;

    memcpy(m_aryCube, m_nextAryCube, sizeof(m_aryCube));

    //初始坐标
    m_ptCube.SetRowCol(0, BACK_COLS / 2 - CUBE_COLS / 2);
}

void CGame::NewBlock()
{
  srand(time(NULL));
  m_nNextCubeType = rand() % 4;
  m_nNextCubeIdx = rand() % 4;

  memcpy(m_nextAryCube, GetCube(m_nNextCubeType, m_nNextCubeIdx), sizeof(m_nextAryCube));

  //初始坐标
  m_ptCube.SetRowCol(0, BACK_COLS / 2 - CUBE_COLS / 2);
}

void CGame::MoveDown()
{
    if (!m_isExceed) {
        return;
    }
  int nRow = 0;
  int nCol = 0;
  m_ptCube.GetRowCol(nRow, nCol);
  m_ptCube.SetRowCol(++nRow, nCol);

  if (IsCollision())
  {
    //碰撞了，固定方块
    m_ptCube.SetRowCol(--nRow, nCol);
    FixBlock();

    //使用新方块
    UseNewBlock();

    NewBlock();
    //消行
    ClearRow();
  }
}

void CGame::MoveRight()
{
    if (!m_isExceed) {
        return;
    }
  int nRow = 0;
  int nCol = 0;
  m_ptCube.GetRowCol(nRow, nCol);
  m_ptCube.SetRowCol(nRow, ++nCol);

  if (IsCollision())
  {
    //碰撞了，还原
    m_ptCube.SetRowCol(nRow, --nCol);
  }
}

void CGame::MoveLeft()
{
    if (!m_isExceed) {
        return;
    }
  int nRow = 0;
  int nCol = 0;
  m_ptCube.GetRowCol(nRow, nCol);
  m_ptCube.SetRowCol(nRow, --nCol);

  if (IsCollision())
  {
    //碰撞了，还原
    m_ptCube.SetRowCol(nRow, ++nCol);
  }
}

void CGame::MoveUp()
{
    if (!m_isExceed) {
        return;
    }
  int nOldIdx = m_nCubeIdx++;
  if (m_nCubeIdx == 4)
  {
    m_nCubeIdx = 0;
  }
  memcpy(m_aryCube, GetCube(m_nCubeType, m_nCubeIdx), sizeof(m_aryCube));

  if (IsCollision())
  {
    //碰撞了，还原
    m_nCubeIdx = nOldIdx;
    memcpy(m_aryCube, GetCube(m_nCubeType, m_nCubeIdx), sizeof(m_aryCube));
  }

}

void CGame::ShowGame()
{
  system("cls");

  //临时将方块写入背景
  FixBlock();


  //显示背景
  for (size_t nRow = 0; nRow < BACK_ROWS; nRow++)
  {
    for (size_t nCol = 0; nCol < BACK_COLS; nCol++)
    {
      if (m_aryBack[nRow][nCol] == wall)
      {
        std::cout << "■";
      }
      else
      {
        std::cout << "  ";
      }
    }
    //换行，显示下一行
    std::cout << std::endl;
  }

  ShowMenu();
  //清除临时写入背景的方块数据
  ClearBlock();

}

void CGame::ShowNextCube()
{
    for (size_t nRow = 0; nRow < CUBE_ROWS; nRow++)
    {
        for (size_t nCol = 0; nCol < CUBE_COLS; nCol++)
        {
            if (m_nextAryCube[nRow][nCol] == wall)
            {
                std::cout << "■";
            }
            else
            {
                std::cout << "  ";
            }
        }
        //换行，显示下一行
        std::cout << std::endl;
    }
}

void CGame::ShowMenu()
{
    std::cout << " 下一个方块 " << " 当前分数：" << m_nGoal << " 重新开始(enter) "<< " 暂停(esc) " << " 开始(space) " << std::endl;
    ShowNextCube();
}

int CGame::GetGoal()
{
    return m_nGoal;
}

void CGame::Restart()
{
    Init();
}

bool CGame::GetIsExceed()
{
    return m_isExceed;
}

void CGame::SetIsExceed(bool exceed)
{
    m_isExceed = exceed;
}

bool CGame::IsCollision()
{
  int nCubeRow = 0;
  int nCubeCol = 0;
  m_ptCube.GetRowCol(nCubeRow, nCubeCol);

  for (size_t nRow = 0; nRow < CUBE_ROWS; nRow++)
  {
    for (size_t nCol = 0; nCol < CUBE_COLS; nCol++)
    {
      if ((m_aryCube[nRow][nCol] == wall) 
        &&(m_aryBack[nCubeRow + nRow][nCubeCol + nCol] == wall))
      {
        return true;
      }
    }
  }
  return false;
}

void CGame::FixBlock()
{
  int nCubeRow = 0;
  int nCubeCol = 0;
  m_ptCube.GetRowCol(nCubeRow, nCubeCol);

  for (size_t nRow = 0; nRow < CUBE_ROWS; nRow++)
  {
    for (size_t nCol = 0; nCol < CUBE_COLS; nCol++)
    {
      if (m_aryCube[nRow][nCol] == wall)
      {
        m_aryBack[nCubeRow + nRow][nCubeCol + nCol] = m_aryCube[nRow][nCol];
      }
    }
  }
}

void CGame::ClearBlock()
{
  int nCubeRow = 0;
  int nCubeCol = 0;
  m_ptCube.GetRowCol(nCubeRow, nCubeCol);

  for (size_t nRow = 0; nRow < CUBE_ROWS; nRow++)
  {
    for (size_t nCol = 0; nCol < CUBE_COLS; nCol++)
    {
      if (m_aryCube[nRow][nCol] == wall)
      {
        m_aryBack[nCubeRow + nRow][nCubeCol + nCol] = emty;
      }
    }
  }
}

void CGame::ClearRow()
{
  //从倒数第二行开始
  for (size_t nRow = BACK_ROWS-2; nRow > 0; nRow-- )
  {
    if (IsCanClear(nRow))
    {
      m_nGoal++;
      MoveRows(nRow);

      //继续判断此行
      ++nRow;
    }
  }
}

bool CGame::IsCanClear(int nRow)
{
  //如果此行全为wall，则可以消除
  for (size_t nCol = 1; nCol < BACK_COLS-1; nCol++)
  {
    if (m_aryBack[nRow][nCol] == emty)
    {
      return false;
    }
  }
  return true;
}

void CGame::MoveRows(int nRowStart)
{
  //从第nRowStart行开始，上一行拷贝到下一行
  for (size_t nRow = nRowStart; nRow > 0; nRow--)
  {
    memcpy(&m_aryBack[nRow][0], &m_aryBack[nRow -1][0], BACK_COLS);
  }
}


char CGame::m_aryBlocks[][CUBE_COLS] = {
  /*
  ■■  ■■■■  ■     ■■
  ■■       ■■■      ■■
  */
#if 1
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,

  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,

  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,

  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
#endif

  //■■■■
#if 1
  1,1,1,1,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,

  1,0,0,0,
  1,0,0,0,
  1,0,0,0,
  1,0,0,0,

  1,1,1,1,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,

  1,0,0,0,
  1,0,0,0,
  1,0,0,0,
  1,0,0,0,
#endif
  //   ■ 
  //  ■■■
#if 1
  0,1,0,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0,

  1,0,0,0,
  1,1,0,0,
  1,0,0,0,
  0,0,0,0,

  1,1,1,0,
  0,1,0,0,
  0,0,0,0,
  0,0,0,0,

  0,1,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0,
#endif

  //■■
  //  ■■
#if 1
  1,1,0,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0,

  0,0,1,0,
  0,1,1,0,
  0,1,0,0,
  0,0,0,0,

  1,1,0,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0,

  0,0,1,0,
  0,1,1,0,
  0,1,0,0,
  0,0,0,0
#endif
};
