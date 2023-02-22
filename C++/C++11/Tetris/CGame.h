#pragma once
#include "CPoint.h"
#ifndef CGAME_DEF
#define CGAME_DEF
#define GetCube(nCubeType,nCubeIdx) ((char*)m_aryBlocks + nCubeType*64 + nCubeIdx*16)
#endif

class CGame
{
public:
  CGame();
  CGame(const CGame& obj) =delete;
  ~CGame()=default;

  void UseNewBlock();
  void NewBlock();
  void MoveDown();  //下移
  void MoveRight(); //右移
  void MoveLeft();  //左移
  void MoveUp();    //上移（旋转）
  void ShowGame();  //显示
  void ShowNextCube(); // 显示下一个方块
  void ShowMenu(); // 显示菜单
  int GetGoal();  // 获取分数
  void Restart();
  bool GetIsExceed(); // 查看当前游戏状态
  void SetIsExceed(bool); // 设置当前游戏状态
  

private:
  static const size_t CUBE_ROWS = 4;  //方块行数
  static const size_t CUBE_COLS = 4;  //方块列数
  static const size_t BACK_ROWS = 16; //背景行数
  static const size_t BACK_COLS = 16; //背景列数
  void Init();

  enum Flag
  {
    emty, //空
    wall  //墙
  };

  static char m_aryBlocks[][CUBE_COLS];

  bool IsCollision();
  void FixBlock();
  void ClearBlock();
  void ClearRow();
  bool IsCanClear(int nRow);
  void MoveRows(int nRowIdx);
private:
  char m_aryCube[CUBE_ROWS][CUBE_COLS]; //方块
  char m_nextAryCube[CUBE_ROWS][CUBE_COLS]; //下个方块
  char m_aryBack[BACK_ROWS][BACK_COLS]; //背景
  CCubePoint m_ptCube;                  //方块坐标
  size_t m_nCubeType;                   //当前方块类型
  size_t m_nCubeIdx;                    //当前方块索引
  size_t m_nNextCubeType;               //下个方块类型
  size_t m_nNextCubeIdx;                //下个方块索引
  int m_nGoal;
  bool m_isExceed;
};

