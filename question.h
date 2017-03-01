#ifndef _question_H_
#define _question_H_

#include "Point.h"

class question
{
	Point pos;
	bool flag;
	bool RightFlag = true, LeftFlag = true, EqFlag = true;
	char exercise[256];
	int num_of_element = 0;
	int res = 0, left = 0, right = 0, OriginalLeft, OriginalRight, OriginalRes;
	char operator1 = '*', OriginalOperator;

public:
	//question(char op , Point p, int kn = -1, int s = -1) : operator1(op), known(kn), res(s), pos(p) {} // c'tor
	question(Point p, char op, int _left, int _right, int _res, int _numelem, bool _flag1, bool _flag2, bool _flag3)
		: pos(p), operator1(op), left(_left), right(_right), res(_res), num_of_element(_numelem), LeftFlag(_flag1), RightFlag(_flag2), EqFlag(_flag3) , // c'tor
		OriginalLeft(_left), OriginalRight(_right), OriginalRes(_res), OriginalOperator(op)
	{}
	question() {}; // empty c'tor
	Point GetPos() {return pos;	}// returns the point position 
	int res1()  {	return res;	}// this is the result of the equation
	char Op() {	return operator1;}// this is the operator (+,-,*,/)
	void setLeft(int x) { left = x; }
	void setRight(int x) { right = x; }
	void setRes(int x) { res = x; }
	void setOp(char x) { operator1 = x; }
	int getLeft()const { return left; }
	int getRight()const { return right; }
	int getRes()const { return res; }
	void setFlagToTrue() { flag = true; }
	bool getFlag()const { return flag; }
	void getExercise(char*g){strcpy_s(exercise, g);}
	void setExercise(int &lft, int& rgt, int& rs, char& oper, int&numOfElem, bool& flag1, bool& flag2, bool& flag3);
	void setExercise(int &lft, int& rgt, int& rs, char& oper, int&numOfElem, bool& flag1, bool& flag2, bool& flag3, char*tempE);
	int getNumberfromChars(int& i)const;
	bool getRightFlag()const { return RightFlag; }
	bool getLeftFlag()const { return LeftFlag; }
	bool getEqFlag() const{ return EqFlag; }
	int numOfKnownElems() const{ return num_of_element; }
	void setTargilForAutoReplay()
	{
		left = OriginalLeft;
		right = OriginalRight;
		res = OriginalRes;
		operator1 = OriginalOperator;
	}
	void initQ()
	{
		RightFlag = true; LeftFlag = true; EqFlag = true;
		num_of_element = 0;
	}

};



#endif
