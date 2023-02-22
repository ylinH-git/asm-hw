// tagStudent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct tagBirthDay
{
	int year;
	int month;
	int day;
};

struct tagHealth
{
	char *agency;
	double dbHeight;
	double dbWeight;
};

struct tagSchool
{
	char *name;
	int nClass;
	int nGrade;
};
struct tagStudent
{
	char *name;
	char gander;
	struct tagBirthDay birthDay;
	struct tagHealth health;
	struct tagSchool school;
};

#define GET_OFFSET(s, m) (size_t)&((struct s *)NULL)->m
int main(int argc, char* argv[])
{
	// zp = 8
	struct tagStudent student = {
		"zhangsan", // 0
			'm', // 4
		{
			1993, 
			4,
			10,
		},  // 8
		{
			"hospital",
			180.0f,
			70.12f,
		}, // 24
		{
			"school",
				3,
				3
		}, // 48
	};
	
	printf("%s\r\n",student.name);
	printf("%c\r\n",student.gander);
	printf("%d-%d-%d\r\n", student.birthDay.year, student.birthDay.month,student.birthDay.day);
	printf("%s-%f-%f\r\n", student.health.agency,  student.health.dbHeight, student.health.dbWeight);
	printf("%s-%d-%d\r\n", student.school.name,  student.school.nClass, student.school.nGrade);

	printf("%d\r\n",GET_OFFSET(tagStudent, name));
	printf("%d\r\n",GET_OFFSET(tagStudent, gander));
	printf("%d\r\n",GET_OFFSET(tagStudent, birthDay));
	printf("%d\r\n",GET_OFFSET(tagStudent, health));
	printf("%d\r\n",GET_OFFSET(tagStudent, school));
	getchar();
	return 0;
}

