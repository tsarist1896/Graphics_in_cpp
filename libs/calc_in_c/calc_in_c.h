#ifndef _PARSE_MATH_EX_H
#define _PARSE_MATH_EX_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Структуры ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

struct Variable {
	// информация о переменной (имя и значение)
	char	  * name;
	double		value;

	Variable  * next;
	Variable  * prev;
};

struct VariablesList {
	// список переменных и их значений
	Variable *head;

	int		  size;
};



struct MathAtom {
	// Математический атом (операция либо вещественное число)
	double		value;
	char	  * operation;
	//int priority; int number;
	MathAtom  * next;
	MathAtom  * prev;
};

struct ExpressionList {
	// Список математических атомов
	MathAtom *head;
	MathAtom *tail;
	
	int		  size;
};


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Прототипы ф-ий ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

void				AddVariable(VariablesList *list, double value, char *name);
double				CalculateMathExpression(ExpressionList *list, VariablesList *var_list);
MathAtom		  * CreateMathAtom(double value, char *operation);
ExpressionList	  * CreateMathList();
VariablesList	  * CreateVariablesList();
void				DeleteMathList(ExpressionList *dList);
MathAtom		  * DeleteNthMathElement(ExpressionList *list, MathAtom *NthElement);
void				DeleteVarList(VariablesList *dList);
int					EmptyMathList(ExpressionList *list);
Variable		  * FindVariable(VariablesList *list, char *name, char *target = "get");
MathAtom		  * GetHeadMathList(ExpressionList *list);
MathAtom		  * GetTailMathList(ExpressionList *list);
int					IsIdentStr(char *first_str, char *second_str);
int					IsLetter(char ch);
int					IsNumSign(char sign);
int					IsSpaceChar(char ch);
int					IsVarChar(char ch, int position = 0);
MathAtom		  * NumberParser(char *expression, int *cChar);
double				ParserDoubleFromStr(char *strDouble);
double				ParserMathExpression(char *expression, VariablesList *varList = NULL);
void				ParserStrMathExpression(ExpressionList *list, VariablesList *var_list, char *expression);
double				PowTen(int num);
void				PushEndAtom(ExpressionList *list, MathAtom *atom);
MathAtom		  * ReturnTrigonometryValue(VariablesList *var_list, char *expression, int *cChar, char *tfunc);
char			  * Substr(char *string, int from, int to);
MathAtom		  * TrigonometryParser(VariablesList *var_list, char *expression, int *cChar);
void				VariableInitializer(char *expression, VariablesList *varList);
Variable		  * VariableParser(VariablesList *list, char *expression, int *carentChar);

#endif
