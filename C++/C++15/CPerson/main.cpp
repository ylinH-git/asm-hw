#include <iostream>
#include "CPerson.h"

int main()
{
    CTeacherCadre cabre(27, "zhangsan", eGender::male, "班主任", "高级教师","深圳", "131****4687", 20000.00);
    cabre.Show();
}

