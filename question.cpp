#include "question.h"

void question::setExercise(int &lft, int& rgt, int& rs, char& oper, int&numOfElem, bool& flag1, bool& flag2, bool& flag3)
{

	int i = 9, temp;

	if (exercise[i] == '_')
	{
		setLeft(-1);
		i++;
		flag1 = false;
	}
	else
	{
		temp = getNumberfromChars(i);
		left = temp;
		flag1 = true;
		num_of_element++;
	}

	operator1 = exercise[i];

	i++; // go the right var

	if (exercise[i] == '_')
	{
		right = -1;
		i++;
		flag2 = false;
	}
	else
	{
		temp = getNumberfromChars(i);
		right = temp;
		flag2 = true;
		num_of_element++;
	}

	i++; // skip the '=' char

	if (exercise[i] == '_')
	{
		res = -1;
		flag3 = false;
	}
	else
	{
		temp = getNumberfromChars(i);
		res = temp;
		flag3 = true;
		num_of_element++;
	}

	lft = left; rgt = right; rs = res; oper = operator1; numOfElem = num_of_element;
}
void question::setExercise(int &lft, int& rgt, int& rs, char& oper, int&numOfElem, bool& flag1, bool& flag2, bool& flag3, char*tempE)
{
	int f;
	for (f = 0; tempE[f] != '\0'; f++)
		exercise[f] = tempE[f];

	exercise[f] = '\0';

	int i = 9, temp;

	if (exercise[i] == '_')
	{
		setLeft(-1);
		i++;
		flag1 = false;
	}
	else
	{
		temp = getNumberfromChars(i);
		left = temp;
		flag1 = true;
		num_of_element++;
	}

	operator1 = exercise[i];

	i++; // go the right var

	if (exercise[i] == '_')
	{
		right = -1;
		i++;
		flag2 = false;
	}
	else
	{
		temp = getNumberfromChars(i);
		right = temp;
		flag2 = true;
		num_of_element++;
	}

	i++; // skip the '=' char

	if (exercise[i] == '_')
	{
		res = -1;
		flag3 = false;
	}
	else
	{
		temp = getNumberfromChars(i);
		res = temp;
		flag3 = true;
		num_of_element++;
	}

	lft = left; rgt = right; rs = res; oper = operator1; numOfElem = num_of_element;
}
int question::getNumberfromChars(int& i)const
{
	int res = 0;

	while (exercise[i] != '+' && exercise[i] != '-' && exercise[i] != '*' && exercise[i] != '/' && exercise[i] != '\0' && exercise[i] != '=')
	{
		res = res * 10 + exercise[i] - '0';
		i++;
	}
	return res;
}