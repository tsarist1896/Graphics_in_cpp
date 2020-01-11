#include "calc_in_c.h"
#include <cmath>
#include <locale.h>
#include <stdio.h>
#include <string.h>





/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~ Пользовательские ф-ии ~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/



/**
 * Заталкивание переменной в начало списка переменных
 *
 * Заталкивание полученной переменной (в виде ссылки на неё) в начало списка переменных
 *
 * @param VariablesList   * list - список переменных
 * @param double			value - значение переменной
 * @param char			  * name - имя переменной
 *
 * @return void
 */
void AddVariable(VariablesList *list, double value, char *name) {
    if (Variable *var = FindVariable(list, name, "init")) {
        var->value = value; // присвоение нового значения
    }
    else {
        Variable *tmp = new Variable;

        if (tmp) {
            if (list->head == NULL) {
                tmp->next = NULL;
                tmp->prev = NULL;
            }
            else {
                Variable *carrentHead = list->head;
                tmp->next = list->head;
                tmp->prev = NULL;
                carrentHead->prev = tmp;
            }

            list->head = tmp;
            tmp->name = name;
            tmp->value = value;
        }
        else { //tmp == NULL
            std::cout << "\nОшибка в ф-ии AddVariable. Не было выделено памяти под заталкиваемую переменную.";
            getchar(); // ожидание нажатия клавиши
            exit(3); // завершаем программу и передаём код завершения
        }
    }
}





/**
 * Рассчёт списка-репрезентации математического выражения
 *
 * Производит рассчёт списка (математических атомов) репрезентирующего проанализированное
 * математическое выражение и возвращает результат расчёта (вещественное число).
 *
 * @param ExpressionList	* list - список-репрезентация математического выражения
 * @param VariablesList		* var_list - список переменных
 *
 * @return double
 */
double CalculateMathExpression(ExpressionList *list, VariablesList *var_list) {
    MathAtom *NthElement;
    MathAtom *prevElement;
    MathAtom *nextElement;


    for ((NthElement = list->head); list->size > 1 && NthElement->next; (NthElement = NthElement->next)) {
        // делаем число отрицательным если перед числом стоит знак "-", перед которым разположен другой знак
        // либо перед которым отсутсвует какой-либо атом
        prevElement = NthElement->prev;
        nextElement = NthElement->next;

        if ((prevElement == NULL || IsIdentStr(prevElement->operation, "-") || IsIdentStr(prevElement->operation, "+") ||
            IsIdentStr(prevElement->operation, "*") || IsIdentStr(prevElement->operation, "/")) &&
            IsIdentStr(NthElement->operation, "-") && !nextElement->operation) { // отрицательное число
            nextElement->value *= -1;

            DeleteNthMathElement(list, NthElement);
            NthElement = nextElement;
        }
    }


    for ((NthElement = list->head); NthElement; (NthElement = NthElement->next)) {
        // Выполнение операций умножения ("*") и деления ("/")
        prevElement = NthElement->prev;
        nextElement = NthElement->next;

        if (NthElement->operation != NULL && IsIdentStr(NthElement->operation, "*")) {
            prevElement->value *= nextElement->value;
            NthElement = DeleteNthMathElement(list, NthElement);
            NthElement = DeleteNthMathElement(list, nextElement);
        }
        else if (NthElement->operation != NULL && IsIdentStr(NthElement->operation, "/")) {
            prevElement->value /= nextElement->value;
            NthElement = DeleteNthMathElement(list, NthElement);
            NthElement = DeleteNthMathElement(list, nextElement);
        }
    }


    for ((NthElement = list->head); NthElement; (NthElement = NthElement->next)) {
        // Выполнение операций сложения ("+") и вычитания ("-")
        prevElement = NthElement->prev;
        nextElement = NthElement->next;

        if (NthElement->operation != NULL && IsIdentStr(NthElement->operation, "+")) {
            if (prevElement) {
                prevElement->value += nextElement->value;

                NthElement = DeleteNthMathElement(list, NthElement);
                NthElement = DeleteNthMathElement(list, nextElement);
            }
            else {
                NthElement = DeleteNthMathElement(list, NthElement);
            }
        }
        else if (NthElement->operation != NULL && IsIdentStr(NthElement->operation, "-")) {
            if (prevElement) {
                prevElement->value -= nextElement->value;
                NthElement = DeleteNthMathElement(list, NthElement);
                NthElement = DeleteNthMathElement(list, nextElement);
            }
            else {
                nextElement->value *= -1;
                NthElement = DeleteNthMathElement(list, NthElement);
            }

        }
    }
    //std::cout << "Возвращаем: " << list->head->value << "\n";
    return list->head->value;
}





/**
 * Создаёт математический атом
 *
 * Создаёт и возвращает ссылку на математический атом
 *
 * @param double	value  - вещественное число либо NULL
 * @param char	  * operation  - математическая операция либо NULL
 *
 * @return MathAtom *
 */
MathAtom *CreateMathAtom(double value, char *operation) {
    MathAtom *atom = new MathAtom;//(MathAtom*)malloc(sizeof(MathAtom));

    if (atom) {
        atom->next = NULL;
        atom->prev = NULL;

        atom->operation = operation;
        atom->value = value;
    }
    else { //atom == NULL
        std::cout <<  "\nОшибка в ф-ии CreateMathAtom. Не было выделено памяти под математический атом.";
        getchar(); // ожидание нажатия клавиши
        exit(3); // завершаем программу и передаём код завершения
    }

    return atom;
}





/**
 * Создаёт список математических атомов
 *
 * Создаёт список математических единиц (атомов) и возвращает ссылку на него
 *
 * @return ExpressionList *
 */
ExpressionList *CreateMathList() {
    // Выделяем память
    ExpressionList *list = new ExpressionList;//(ExpressionList*)malloc(sizeof(ExpressionList));

    // Инициализируем переменные
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}





/**
 * Создаёт список переменных
 *
 * Создаёт список инициализированных переменных и возвращает ссылку на него
 *
 * @return VariablesList *
 */
VariablesList *CreateVariablesList() {
    // Выделяем память
    VariablesList *list = new VariablesList;//(VariablesList*)malloc(sizeof(VariablesList));

    // Инициализируем переменные
    list->head = NULL;
    list->size = 0;

    return list;
}





/**
 * Удаляет список математических атомов
 *
 * Удаляет список математических единиц (атомов) и все его элементы
 *
 * @param ExpressionList	* dList "список"
 *
 * @return void
 */
void DeleteMathList(ExpressionList *dList) {
    MathAtom *atm = dList->head;
    MathAtom *exp_next = NULL;

    while (atm) { // Удаление элементов списка
        exp_next = atm->next;
        delete atm;//free(atm);
        atm = exp_next;
    }

    delete atm;
    //free(dList); // Удаление списка
}





/**
 * Удаление математического атома
 *
 * Удаляет математический элемент (атом) из списка мат. атомов и
 * возвращает следующий либо предыдущий атом
 *
 * @param ExpressionList    * list - список мат. атомов
 * @param MathAtom			* NthElement - удаляемый элемент
 *
 * @return MathAtom *
 */
MathAtom *DeleteNthMathElement(ExpressionList *list, MathAtom *NthElement) {
    MathAtom *prevElem = NthElement->prev;
    MathAtom *nextElem = NthElement->next;
    MathAtom *returnElem = prevElem;

    // удаляем первый элемент
    if (!prevElem) {
        nextElem->prev = NULL;
        list->head = nextElem;
    }
    else { // не первый элемент
        if (!nextElem) { // не имеет следующего элемента
            list->tail = prevElem;
        }
        else { // имеет следующий элемент
            nextElem->prev = prevElem;
        }
    }

    // удаляем последний элемент
    if (!nextElem) {
        prevElem->next = NULL;
        list->tail = prevElem;
    }
    else { // не последний элемент
        if (!prevElem) { // не имеет предыдущего элемента
            list->head = nextElem;
            returnElem = nextElem;
        }
        else { // имеет предыдущий элемент
            prevElem->next = nextElem;
        }
    }

    delete NthElement;
    //free(NthElement);
    list->size--;

    return returnElem;
}





/**
 * Удаляет список переменных
 *
 * Удаляет список переменных и все его элементы
 *
 * @param VariablesList	* dList "список"
 *
 * @return void
 */
void DeleteVarList(VariablesList *dList) {
    Variable *var = dList->head;
    Variable *var_next = NULL;

    while (var) { // Удаление элементов списка
        var_next = var->next;
        delete var;
        //free(var);
        var = var_next;
    }

    delete dList;
    //free(dList); // Удаление списка
}





/**
 * Проверка пустоты списка математических атомов
 *
 * Проверяет список математических атомов на наличие элементов
 *
 * @param MathExpression	* list - список математических атомов
 *
 * @return int (0 || 1)
 */
int EmptyMathList(ExpressionList *list) {
    return !(list->head) && !(list->tail);
}





/**
 * Получить значение переменной
 *
 * Возвращает ссылку на переменную из списка.
 *
 * @param VariablesList	  * list - список переменных
 * @param char				name - имя переменной
 * @param char			  * target - цель поиска ("get" - получение значения (по умолчанию); "init" - иницализация)
 *
 * @return Variable *
 */
Variable *FindVariable(VariablesList *list, char *name, char *target) {
    Variable *NthElement;
    Variable *returnVar = NULL;

    for (
        (NthElement = list->head);
        NthElement;
        (NthElement = NthElement->next)
        ) {

        if (!strcmp(NthElement->name, name)) {
            returnVar = NthElement;
            break;
        }
    }

    if (IsIdentStr(target, "get") && !returnVar) {
        // Если в процессе поиска указанной (не инициализируемой) в выражении переменной
        // таковая не была найдена в списке переменных
        std::cout <<  "\nОшибка. Переменная '" << name << "' не была инициализирована!";
        getchar(); // ожидание нажатия клавиши
        exit(3); // завершаем программу и передаём код завершения
    }
    else {
        return returnVar;
    }
}





/**
 * Голова списка математических атомов
 *
 * Возвращает ссылку на голову списка математических атомов
 *
 * @param MathExpression	* list - список математических атомов
 *
 * @return MathAtom *
 */
MathAtom *GetHeadMathList(ExpressionList *list) {
    return list->head;
}





/**
 * Хвост списка математических атомов
 *
 * Возвращает ссылку на хвост списка математических атомов
 *
 * @param MathExpression	* list - список математических атомов
 *
 * @return MathAtom *
 */
MathAtom *GetTailMathList(ExpressionList *list) {
    return list->tail;
}





/**
 * Проверка идентичности строк
 *
 * Проверяет две строки на идентичность
 *
 * @param char	* first_str - первая проверяемая строка
 * @param char	* second_str - вторая проверяемая строка
 *
 * @return int
 */
int IsIdentStr(char * first_str, char * second_str) {
    if (first_str != NULL && second_str != NULL && strcmp(first_str, second_str) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}





/**
 * Проверка символа на принадлежность диапазону [a-zA-Z]
 *
 * Проверяет является ли символ латинской буквой ([a-zA-Z])
 *
 * @param char ch - символ, который может быть латинской буквой
 *
 * @return int
 */
int IsLetter(char ch) {
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}





/**
 * Проверка символа на принадлежность к символам числа
 *
 * Проверяет получаемый символ на принадлежность к возможным символам числа
 *
 * @param char sign - символ, который может быть символом числа
 *
 * @return int
 */
int IsNumSign(char sign) {
    return (sign == '.' || sign == ',' || sign == 'e') || (sign >= '0' && sign <= '9');
}





/**
 * Проверка символа на принадлежость к одному из пробельных символов
 *
 * Проверяет является ли символ одним из пробельных символов
 *
 * @param char	ch - символ, проверяемый на принадлежность к одному из пробельных символов
 *
 * @return int
 */
int IsSpaceChar(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}





/**
 * Проверка символа на допустимость использования в имени переменной
 *
 * Проверяет является ли символ допустимым для использования в имени переменной
 *
 * @param char	ch - символ, проверяемый на допустимость использования в качестве имени переменной
 * @param int	position - позиция символа в имени переменной (по умолчанию - 0)
 *
 * @return int
 */
int IsVarChar(char ch, int position) {
    int result = (ch == '_' || IsLetter(ch));
    if (position)
        result += (ch >= '0' && ch <= '9');

    return result;
}





/**
 * Анализ числа в выражении
 *
 * Определяет границы числового значения в выражении, извлекает это значение, а так же
 * создаёт и возвращает мат. атом на основе этого значения.
 *
 * @param char	* expression - анализируемое выражение
 * @param int	* cChar - текущий анализируемый символ выражения
 *
 * @return MathAtom *
 */
MathAtom *NumberParser(char *expression, int *cChar) {
    int carentChar = *cChar;
    int char_from = carentChar++;
    int char_to = char_from;
    char c;

    while ((c = expression[carentChar]) != '\0' && (IsNumSign(c) || c == '-')) {
        char_to = carentChar++;
    }

    char *numb = Substr(expression, char_from, char_to);
    double dbl = ParserDoubleFromStr(numb);
    *cChar = carentChar - 1;

    return CreateMathAtom(dbl, NULL);
}





/**
 * Анализатор вещественного числа
 *
 * Переводит строку с числом из строкового представления в вещественное число (double).
 *
 * @param char * strDouble - строковое представление числа
 *
 * @return double
 */
double ParserDoubleFromStr(char *strDouble) {
    char c;
    int carentChar = 0;

    int numBooferFlag = 0; // Переключатель: 0 - в doubleNumBoofer и intNumBoofer мусор,
                           //                1 - в doubleNumBoofer и intNumBoofer содержится число
    int numPart = 0; // Переключатель: 0 - целая часть, 1 - дробная часть
    int intBeforeDot = 0; // кол-во цифр до десятичной точки
    int intAfterDot = 0; // кол-во цифр после десятичной точки
    double intNumBoofer = 0.0; // целая часть double
    double doubleNumBoofer = 0.0; // дробная часть double

    while ((c = strDouble[carentChar]) != '\0' && IsSpaceChar(c)) { // пропукаем возможные ведущие пробелы
        carentChar++;
    }

    bool isExp = false;
    char powtype;
    int Exp;
    while ((c = strDouble[carentChar]) != '\0') {
        if(c == 'e') {
            int sizeStr = strlen(strDouble);
            char *powstr = Substr(strDouble, carentChar+2, sizeStr-1);

            powtype = strDouble[carentChar+1];
            Exp = ParserDoubleFromStr(powstr);

            isExp = true;

            strDouble = Substr(strDouble, 0, carentChar-1);
            break;
        }


        carentChar++;
    }

    carentChar = 0;

    bool powNum = false;
    while ((c = strDouble[carentChar]) != '\0' && (IsNumSign(c) || (isExp && (c == '-' || c == '+') ))) {
        if (c >= '0' && c <= '9') {
            numBooferFlag = 1; // найдено число

            if (numPart == 0) { // вычисление целой части
                intNumBoofer = intNumBoofer + (((double)c - 48) * PowTen(++intBeforeDot * -1));
            }
            else { // вычисление дробной части
                doubleNumBoofer = doubleNumBoofer + (((double)c - 48) * PowTen(++intAfterDot * -1));
            }
        }
        else if (c == '.') {
            numBooferFlag = 1; // найдено число
            numPart = 1;
        }
        else if (c == ',') { // исправляем запятую на точку
            strDouble[carentChar] = '.';
            carentChar--;
        }

        carentChar++;
    }

    if (numBooferFlag == 1) { // окончательное оформление числа
        if (intNumBoofer < 1) { // переводим промежуточное дробное число в целое
            intNumBoofer = intNumBoofer * PowTen(intBeforeDot);
        }

        doubleNumBoofer = intNumBoofer + doubleNumBoofer;
    }

    if(isExp) {
        if(powtype == '-')
            doubleNumBoofer /= PowTen(Exp);
        else
            doubleNumBoofer *= PowTen(Exp);
    }

    //std::cout << "doubleNumBoofer: " << doubleNumBoofer << "\n";

    return doubleNumBoofer;
}





/**
 * Стартовая функция анализа мат. выражения
 *
 * Стартовая функция используемая как для анализа изначально передаваемого (из вне) выражения,
 * так и для (рекурсивного) анализа отдельных кусков (подвыражений) уже переданного выражения (например,
 * таких как выражения в скобках, значений передаваемых в тригонометрические функции,
 * а так же выражений (либо вещественных чисел) указываемых для инициализации переменных).
 *
 * @param char			* expression - математическое выражение, передаваемое на анализ
 * @param VariablesList	* varList - список переменных (по умолчанию - NULL)
 *
 * @return double
 */
double ParserMathExpression(char *expression, VariablesList *varList) {
    int varDeletedFlag = 0;
    ExpressionList *mathList = CreateMathList();

    if (!varList) { // Если это внешний (первый) вызов функции
        varList = CreateVariablesList();
        varDeletedFlag = 1;
    }

    // Запускаем инициализатор переменных
    VariableInitializer(expression, varList); // Проблема здесь

    // Переводим строку в список-репрезентацию мат. выражения
    ParserStrMathExpression(mathList, varList, expression);

    // Вычисляем значение выражения по списку-репрезентации мат. выражения
    double result = CalculateMathExpression(mathList, varList);

    // Удаляем список-репрезентацию мат. выражения
    DeleteMathList(mathList);
    if (varDeletedFlag) { // Удаляем список переменных если это внешний (первый) вызов функции
        DeleteVarList(varList);
    }
    return result;
}





/**
 * Анализ символов выражения
 *
 * Анализирует символы выражения и либо передаёт сложные участки выражения
 * на обработку другим специализированным (на обработке таких участков) функциям,
 * либо обрабатывает некоторые лёгкие участки выражения (например, математические элементы) внутри себя.
 * На основании полученного математического выражения строит/заполняет список-репрезентацию математического выражения.
 *
 * @param ExpressionList  * list - список-репрезентация математического выражения
 * @param VariablesList	  * var_list - список переменных
 * @param char			  * expression - математическое выражение
 * @param int			  * level - список переменных
 *
 * @return void
 */
void ParserStrMathExpression(ExpressionList *list, VariablesList *var_list, char *expression) {
    MathAtom *math_atom = NULL;
    Variable *var = NULL;

    char c;
    int carentChar = 0;
    int exp_length = strlen(expression);
    while ((c = expression[carentChar]) != '\0' && carentChar < exp_length) {
        if (c == '(') { // обработка скобок
            expression[carentChar] = ' '; // затираем первую открывающую скобку ("(")

            int nested_from = ++carentChar;
            int nested_to = NULL;
            int nested_level = 1;//уровень вложенности
            while (nested_level > 0 && (c = expression[carentChar]) != '\0') {
                if (c == '(') {
                    nested_level++;
                }
                else if(c == ')' && nested_level-- == 1) {
                    nested_to = carentChar - 1;
                    char *sub_exp = Substr(expression, nested_from, nested_to);

                    // вызваем стартовую ф-ию для анализа и рассчета найденного подвыражения
                    double calculateVal = ParserMathExpression(sub_exp, var_list);

                    expression[carentChar] = ' '; // затираем соответствующую закрывающую скобку (")")
                                                    // добавляем мат. атом со значеним полученным от рассчёта найденного подвыражения
                    PushEndAtom(list, CreateMathAtom(calculateVal, NULL));
                    break;
                }
                carentChar++;
            }
            if (nested_level != 0) { // Генерим ошибку
                std::cout << "\nОшибка: Отсутствует закрывающая скобка.";
                getchar(); // ожидание нажатия клавиши
                exit(3); // завершаем программу и передаём код завершения
            }
        }
        else { // обработка мат. элементов (атомов)
            switch (c) {
            case '+':
                PushEndAtom(list, CreateMathAtom(NULL, "+"));
                break;
            case '-':
                PushEndAtom(list, CreateMathAtom(NULL, "-"));
                break;
            case '*':
                PushEndAtom(list, CreateMathAtom(NULL, "*"));
                break;
            case '/':
                PushEndAtom(list, CreateMathAtom(NULL, "/"));
                break;
            default:
                if (IsNumSign(c)) { // если число
                    PushEndAtom(list, NumberParser(expression, &carentChar));

                }
                else if (c == '$' && (var = VariableParser(var_list, expression, &carentChar))) { // если переменная
                    PushEndAtom(list, CreateMathAtom(var->value, NULL));
                }
                else if (IsLetter(c) && (math_atom = TrigonometryParser(var_list, expression, &carentChar))) { // если тригонометрическая ф-ия
                    PushEndAtom(list, math_atom);
                }
            }
        }
        carentChar++;
    }
}





/**
 * Степень числа 10
 *
 * Возводит число 10 в указанную степень.
 *
 * @param int num - степень числа 10
 *
 * @return double
 */
double PowTen(int num) {
    if (num == 0) {
        return 1;
    }
    else if (num > 0) {
        return 10 * PowTen(--num);
    }
    else {
        return 0.1 * PowTen(++num);
    }
}





/**
 * Заталкивание мат. атома в конец списка
 *
 * Заталкивание математического атома в конец списка математических атомов
 * составляющих репрезентацию разбираемого выражения
 *
 * @param ExpressionList  * list - список математических атомов
 * @param MathAtom		  * atom - заталкиваемый математический атом
 *
 * @return void
 */
void PushEndAtom(ExpressionList *list, MathAtom *atom) {
    if (atom) {
        if (EmptyMathList(list)) {
            atom->next = NULL;
            atom->prev = NULL;
            list->head = atom;
        }
        else {
            MathAtom *carrentTail = GetTailMathList(list);
            atom->next = NULL;
            atom->prev = GetTailMathList(list);
            carrentTail->next = atom;
        }

        list->tail = atom;
    }
    else { //atom == NULL
        std::cout << "\nОшибка в ф-ии pushEndAtom. Вместо MathAtom был передан NULL.";
        getchar(); // ожидание нажатия клавиши
        exit(3); // завершаем программу и передаём код завершения
    }
}





/**
 * Возвращает значение тригонометрической функции
 *
 * Возвращает мат. атом со значением передаваемым в тригонометрическую функцию
 *
 * @param VariablesList	* var_list - список переменных
 * @param char			* expression - анализируемое выражение
 * @param int			* cChar - текущий анализируемый символ выражения
 * @param char			* tfunc - имя тригонометрической функции
 *
 * @return MathAtom *
 */
MathAtom *ReturnTrigonometryValue(VariablesList *var_list, char *expression, int *cChar, char *tfunc) {
    int carentChar = *cChar;
    int value_from = -1;
    int value_to = -1;
    int nested_level = 0;
    char c;

    while ((c = expression[carentChar]) != '\0') {
        if (c == '(' && ++nested_level == 1) {
            value_from = carentChar + 1;
            expression[carentChar] = ' '; // Затираем скобку
        }
        else if (c == ')' && nested_level-- == 1) {
            value_to = carentChar - 1;
            expression[carentChar] = ' '; // Затираем скобку
            break;
        }

        if (IsNumSign(c) && value_from == -1) {
            std::cout << "\nОшибка. После названия тригонометрической ф-ии '" << tfunc << "' (" << carentChar << ")[" << expression[carentChar] << "] ожидались скобки со значением!";
            getchar(); // ожидание нажатия клавиши
            exit(3); // завершаем программу и передаём код завершения
        }

        carentChar++;
    }

    char *value_tfunc = Substr(expression, value_from, value_to);

    // вызваем стартовую ф-ию для анализа и рассчета найденного подвыражения
    double calculateVal = ParserMathExpression(value_tfunc, var_list);

    *cChar = carentChar; // переводим позицию анализируемого символа

    return CreateMathAtom(calculateVal, tfunc);
}





/**
 * Извлечение части строки
 *
 * Извлекает подстроку определяемую  @param "from" и @param "to" из строки @param * string
 *
 * @param char    * string - исходная строка
 * @param int		from - от какого по счёту (с 0) символа (включительно) начинается извлечение строки
 * @param int		to -  до какого по счёту (с 0) символа (включительно) продолжается извлечение строки
 *
 * @return char *
 */
char *Substr(char *string, int from, int to) {
    int strLength = strlen(string);

    if (strLength < to) { // корректировка длины копируемой строки
        to = strLength - 1;
    }

    int sub_strLength = to - from + 1; // '+ 1' т.к. от from (включительно) до to (включительно)

    char *sub_str = (char*)malloc(sizeof(char) * (sub_strLength + 1)); // '+ 1' т.к. добавляем терминальный символ
    sub_str[sub_strLength] = '\0';

    int carentChar = from;
    while (carentChar <= to) { // копирование подстроки с затиранием соответствующего пространства в основной строке
        sub_str[carentChar - from] = string[carentChar];
        string[carentChar] = ' ';
        carentChar++;
    }

    return sub_str;
}





/**
 * Анализатор тригонометрических функций
 *
 * Возвращает мат. атом со значением получаемым в результате нахождения тригонометрической функции.
 *
 * @param VariablesList	* var_list - список переменных
 * @param char			* expression - анализируемое выражение
 * @param int			* cChar - текущий анализируемый символ выражения
 *
 * @return MathAtom *
 */
MathAtom *TrigonometryParser(VariablesList *var_list, char *expression, int *cChar) {
    MathAtom *returnAtom = NULL;

    int carentChar = *cChar;
    int initial_carentChar = carentChar++;
    int letters_from = initial_carentChar;
    int letters_to = letters_from;
    char c;

    // определяем имя тригонометрической ф-ии
    while ((c = expression[carentChar]) != '\0' && IsLetter(c)) {
        letters_to = carentChar++;
    }

    char *tfunc = Substr(expression, letters_from, letters_to);
    *cChar = carentChar;

    if (!strcmp("cos", tfunc)) {
        returnAtom = ReturnTrigonometryValue(var_list, expression, cChar, "cos"); // CreateTrigonometryAtom
        returnAtom->value = cos(returnAtom->value);
    }
    else if (!strcmp("ctan", tfunc)) { // ctg = cos/sin
        returnAtom = ReturnTrigonometryValue(var_list, expression, cChar, "ctan");
        returnAtom->value = cos(returnAtom->value) / sin(returnAtom->value);
    }
    else if (!strcmp("sin", tfunc)) {
        returnAtom = ReturnTrigonometryValue(var_list, expression, cChar, "sin");
        returnAtom->value = sin(returnAtom->value);
    }
    else if (!strcmp("tan", tfunc)) { // tg = sin/cos
        returnAtom = ReturnTrigonometryValue(var_list, expression, cChar, "tan");
        returnAtom->value = sin(returnAtom->value) / cos(returnAtom->value);
    }
    else {
        // Если не соответствует ни одной тригонометрической ф-ии отматываем текущий рассматриваемый символ *carentChar обратно
        *cChar = initial_carentChar;
    }

    return returnAtom;
}





/**
 * Инициализация переменных
 *
 * Инициализация переменных, встречаемых (инициализируемых) в выражении
 *
 * @param char			* expression - анализируемое выражение
 * @param VariablesList	* varList - список переменных
 *
 * @return void
 */
void VariableInitializer(char *expression, VariablesList *var_list) {
    char c;
    int carentChar = 0;
    int initialization;
    int expLength = strlen(expression);
    while ((c = expression[carentChar]) != '\0' && carentChar < expLength) {
        if (c == '$' && IsVarChar(expression[carentChar + 1], 0)) {
            int varName_from = ++carentChar;
            int varName_to = varName_from;

            // определение имени переменной
            while ((c = expression[carentChar]) != '\0' && (IsVarChar(c, 1)) && carentChar < expLength) {
                varName_to = carentChar;
                carentChar++;
            }

            initialization = 0;
            int exp_from = 0;
            int exp_to = 0;
            int nested_level = 0;
            while ((c = expression[carentChar]) != '\0' && carentChar < expLength) {
                if (initialization == 0 && c == '=') {
                    initialization = 1;
                    exp_from = carentChar + 1;
                    expression[carentChar] = ' '; // Затираем '='
                    expression[varName_from - 1] = ' '; // Затираем '$'
                }
                else if (initialization == 1 && c == '(') {
                    nested_level++;
                }
                else if (initialization == 1 && c == ')') {
                    nested_level--;
                }
                else if (initialization == 1 && nested_level == 0 && c == ';') {
                    initialization = 2;
                    carentChar++;
                    break;
                }
                carentChar++;
            }

            exp_to = carentChar - 1;

            if (initialization == 2) {
                char *var_name = Substr(expression, varName_from, varName_to);
                char *var_exp = Substr(expression, exp_from, exp_to);

                // Вызывает стартовую ф-ию рассчета выражения
                double var_value = ParserMathExpression(var_exp, var_list);

                // Добавляем переменную
                AddVariable(var_list, var_value, var_name);
            }
            else if (initialization == 1) {
                std::cout << "Ошибка имени переменной\n";
                // генерим ошибку
                char *var_name = Substr(expression, varName_from, varName_to);
                char *place_err = Substr(expression, varName_from, exp_to);

                std::cout << "\nОшибка. Неправильно инициализирована переменная: '" << var_name << "'!: \"" << place_err << "\".\n";
                if (place_err[strlen(place_err) - 1] != ';') {
                    std::cout << "Отсутствует завершающий символ ';'.\n";
                }
                getchar(); // ожидание нажатия клавиши
                exit(3); // завершаем программу и передаём код завершения
            }
        }
        else if (c == '$' && !IsVarChar(expression[carentChar + 1], 0)) {
            int var_name_from = carentChar;

            while ((c = expression[carentChar]) != '\0' && !IsSpaceChar(c) && carentChar < expLength) {
                carentChar++;
            }

            std::cout << "\nОшибка. '" << Substr(expression, var_name_from, carentChar - 1) << "' некорректное имя переменной" << "'" << expression << "'";
            getchar(); // ожидание нажатия клавиши
            exit(3); // завершаем программу и передаём код завершения
        }
        carentChar++;
    }
}





/**
 * Возвращает переменную встречаемую в выражении
 *
 * Возвращает переменную встречаемую в анализируемом выражении
 *
 * @param VariablesList	* list - список переменных
 * @param char			* expression - анализируемое выражение
 * @param int			* carentChar - текущий анализируемый символ выражения
 *
 * @return Variable *
 */
Variable *VariableParser(VariablesList *list, char *expression, int *carentChar) {
    if (IsVarChar(expression[*carentChar + 1], 0)) {
        int name_from = ++(*carentChar);
        int name_to = name_from;
        char c;

        // определяем имя переменной
        while (((c = expression[++(*carentChar)]) != '\0') && IsVarChar(c, 1)) {
            name_to = *carentChar;
        }

        char *vname = Substr(expression, name_from, name_to); // извлекаем имя пеменной
        return FindVariable(list, vname); // ищем переменную
    }
    else {
        std::cout << "\nОшибка. После символа переменной '$' (" << *carentChar << ") ожидалось имя переменной!";
        getchar(); // ожидание нажатия клавиши
        exit(3); // завершаем программу и передаём код завершения
    }
}


