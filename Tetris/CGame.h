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
  void MoveDown();  //����
  void MoveRight(); //����
  void MoveLeft();  //����
  void MoveUp();    //���ƣ���ת��
  void ShowGame();  //��ʾ
  void ShowNextCube(); // ��ʾ��һ������
  void ShowMenu(); // ��ʾ�˵�
  int GetGoal();  // ��ȡ����
  void Restart();
  bool GetIsExceed(); // �鿴��ǰ��Ϸ״̬
  void SetIsExceed(bool); // ���õ�ǰ��Ϸ״̬
  

private:
  static const size_t CUBE_ROWS = 4;  //��������
  static const size_t CUBE_COLS = 4;  //��������
  static const size_t BACK_ROWS = 16; //��������
  static const size_t BACK_COLS = 16; //��������
  void Init();

  enum Flag
  {
    emty, //��
    wall  //ǽ
  };

  static char m_aryBlocks[][CUBE_COLS];

  bool IsCollision();
  void FixBlock();
  void ClearBlock();
  void ClearRow();
  bool IsCanClear(int nRow);
  void MoveRows(int nRowIdx);
private:
  char m_aryCube[CUBE_ROWS][CUBE_COLS]; //����
  char m_nextAryCube[CUBE_ROWS][CUBE_COLS]; //�¸�����
  char m_aryBack[BACK_ROWS][BACK_COLS]; //����
  CCubePoint m_ptCube;                  //��������
  size_t m_nCubeType;                   //��ǰ��������
  size_t m_nCubeIdx;                    //��ǰ��������
  size_t m_nNextCubeType;               //�¸���������
  size_t m_nNextCubeIdx;                //�¸���������
  int m_nGoal;
  bool m_isExceed;
};

