#include "graphics.h"
#include "../../libs/calc_in_c/calc_in_c.h"
#include <iostream>
#include <math.h>

/**
 * @brief Graphics::Graphics
 * Конструктор класса отвечающего за отображение графиков математических функций
 * @param ui    -- ui (user interface) объект Qt
 * @param cp    -- класс отвечающий за отображение координатной плоскости
 * @param scene -- сцена для canvas
 */
Graphics::Graphics(Ui::MainWindow *ui, CoordPlane *cp, QGraphicsScene *scene) {
    this->ui = ui;
    this->cp = cp;
    this->scene  = scene;
    this->canvas = this->ui->graphicsView->viewport();
    this->debug  = this->ui->DebugFlag->isChecked();
    this->ToDefaultCoordArrays();

    this->SetGraphicString( 0, ui->gFunc0->text() );
    this->SetGraphicString( 1, ui->gFunc1->text() );
    this->SetGraphicString( 2, ui->gFunc2->text() );

    this->SetGraphicColor( 0, ui->color_graph0->currentIndex() );
    this->SetGraphicColor( 1, ui->color_graph1->currentIndex() );
    this->SetGraphicColor( 2, ui->color_graph2->currentIndex() );
}



/**
 * @brief Graphics::~Graphics
 * Деструктор класса
 */
Graphics::~Graphics() {
    delete [] arrayXCoords;
    delete [] arrayYCoords;
    delete [] arrayCanvasXCoords;
    delete [] arrayCanvasYCoords;

    for (int i = 0; i < this->usedColorsNums; i++) {
        delete usedColors[i];
    }
}



/**
 * @brief Graphics::GetDebug
 * Возвращает значение флага отладки
 * @return -- значение флага отладки
 */
bool Graphics::GetDebug() {
    return this->debug;
}



/**
 * @brief Graphics::SetDebug
 * Устанавливает/спускает флаг отладки
 * @param debug -- значение флага
 */
void Graphics::SetDebug(bool debug) {
    this->debug = debug;
}



/**
 * @brief Graphics::GetGraphicString
 * Возвращает строку с математическим выражением для построения графика
 * @param num -- номер поля с математическим выражением
 * @return std::string -- математическое выражение
 */
std::string Graphics::GetGraphicString(int num) {
    std::string result = "";
    if( num >= 0 && num <= this->graphics_numbers )
        result = this->arrayGraphics[num];

    return result;
}



/**
 * @brief Graphics::SetGraphicString
 * Устанавливает найденное значение строки с математическим выражением для построения графика
 * @param num -- номер поля с математическим выражением
 * @param exp -- математическое выражение
 */
void Graphics::SetGraphicString(int num, QString exp) {
    if( num >= 0 && num <= this->graphics_numbers )
        this->arrayGraphics[num] = exp.toUtf8().constData();
}



/**
 * @brief Graphics::GetGraphicColor
 * Возвращает цвет линий для построения графика
 * @param num -- номер поля с математическим выражением
 * @return QColor -- цвет линий
 */
QColor *Graphics::GetGraphicColor(int num) {
    QColor * color;// = new QColor(0,0,0);
    if( num >= 0 && num <= this->graphics_numbers )
        color = this->usedColors[ this->arrayGraphicsColors[num] ];
    else
        color = this->usedColors[0];
    return color;
}



/**
 * @brief Graphics::SetGraphicColor
 * Устанавливает цвет линий для построения графика
 * @param num -- номер поля с математическим выражением
 * @param color -- номер цвета, доступного в наборе цветов (this->arrayGraphicsColors)
 */
void Graphics::SetGraphicColor(int num, int color) {
    if( num >= 0 && num <= this->graphics_numbers )
        this->arrayGraphicsColors[num] = color;
}



/**
 * @brief Graphics::CalculateYValue
 * Возвращает результат рассчета математического выражения
 * @param exp -- математическое выражение
 * @param X   -- значение 'X'
 * @return double -- результат рассчета математического выражения
 */
double Graphics::CalculateYValue(std::string exp, double X) {
    double result = 0;

    if( this->CheckExpression(exp) ) {
        std::string expression = "$x = " + std::to_string(X) + "; " + exp;

        char * charexpression = new char [expression.length()+1];
        strcpy (charexpression, expression.c_str());

        result = ParserMathExpression(charexpression);
    }

    return result;
}



/**
 * @brief Graphics::ToDefaultCoordArrays
 * Обнуляет значения массивов значений координат
 */
void Graphics::ToDefaultCoordArrays() {
    int dots = this->GetDots();
    for(int i=0; i < this->graphics_numbers; i++) {
        for(int j=0; j < dots; j++) {
            this->arrayXCoords[i][j] = 0;
            this->arrayYCoords[i][j] = 0;
            this->arrayCanvasXCoords[i][j] = 0;
            this->arrayCanvasYCoords[i][j] = 0;
        }
    }
}



/**
 * @brief Graphics::GetDots
 * Возвращает кол-во точек, через которые будут проходить линии
 * @return int -- кол-во точек, для построения графика
 */
int Graphics::GetDots() {
    return this->dots;
}



/**
 * @brief Graphics::CheckExpression
 * Проверяет выражение на корректность
 * @param exp -- проверяемое выражение
 * @return bool -- результат проверки выражения
 */
bool Graphics::CheckExpression(std::string exp) {
    if( exp.length() == 0 )
        return false;

    int openParenthesis  = 0;
    int closeParenthesis = 0;
    int exp_length = exp.length();
    for(int i=0; i < exp_length; i++) { // Проверяем скобки
        switch (exp[i]) {
            case '(':
                openParenthesis++;
                break;
            case ')':
                closeParenthesis++;
                break;
        }
    }

    if(openParenthesis != 0 && openParenthesis != closeParenthesis)
        return false;

    if(closeParenthesis != 0 && openParenthesis != closeParenthesis)
        return false;

    return true;
}



/**
 * @brief Graphics::RefillCoordArrays
 * Заполняет значения массивов значений координат
 */
void Graphics::RefillCoordArrays() {
    double X  = this->cp->GetX();
    double Y  = this->cp->GetY();
    double Xe = this->cp->GetXe();
    double Ye = this->cp->GetYe();

    if(this->debug) std::cout << "X: " << X << " -> " << Xe << "\n";
    int    dots = this->GetDots();
    double step = (Xe - X) / (dots - 1);

    double X_PsiP = this->cp->GetX_PsiP();
    double Y_PsiP = this->cp->GetY_PsiP();
    for(int i=0; i < this->graphics_numbers; i++) {
        std::string exp = GetGraphicString(i);
        if( !this->CheckExpression(exp) )
            continue;

        if(this->debug) std::cout << "X:\n";
        for(int j=0; j < dots; j++) {
            double x1 = X + j * step;
            double y1 = this->CalculateYValue(exp, x1);

            double cnv_x1 = (x1  - X) / X_PsiP;
            double cnv_y1 = (-y1 + Y) / Y_PsiP;

            if(j) { // Проверка
                double x0 = this->arrayXCoords[i][j-1];
                double y0 = this->arrayYCoords[i][j-1];

                bool condition_x0 = (x0 >= X && x0 <= Xe);
                bool condition_x1 = (x1 >= X && x1 <= Xe);
                bool condition_y0 = (y0 <= Y && y0 >= Ye);
                bool condition_y1 = (y1 <= Y && y1 >= Ye);

                if(0 && this->debug) {
                    std::cout << "\n";
                    std::cout << "condition_x0 = ("<<x0<<" >= "<<X<<" && "<<x0<<" <= "<<Xe<<") = "<< (condition_x0 ? "true" : "false") <<";   ";
                    std::cout << "condition_x1 = ("<<x1<<" >= "<<X<<" && "<<x1<<" <= "<<Xe<<") = "<< (condition_x1 ? "true" : "false") <<";\n";
                    std::cout << "condition_y0 = ("<<y0<<" <= "<<Y<<" && "<<y0<<" >= "<<Ye<<") = "<< (condition_y0 ? "true" : "false") <<";   ";
                    std::cout << "condition_y1 = ("<<y1<<" <= "<<Y<<" && "<<y1<<" >= "<<Ye<<") = "<< (condition_y1 ? "true" : "false") <<";\n";
                }

                int action = 0;
                action += condition_x0 ? 1000 : 0;
                action += condition_y0 ?  100 : 0;
                action += condition_x1 ?   10 : 0;
                action += condition_y1 ?    1 : 0;
                double *correctDot;

                switch (action) { // Корректировка
                    case 1011: // Y0 не попал в диапазон
                        if(this->debug) std::cout << "Пограничная ситуация (Y0) this->scene->addLine("<<x0<<", "<<y0<<",  "<<x1<<", "<<y1<<",  pen);\n";
                        correctDot = this->CorrectY0(exp, x0, y0, x1, y1);
                        this->arrayXCoords[i][j-1] = correctDot[0];
                        this->arrayYCoords[i][j-1] = correctDot[1];
                        this->arrayCanvasXCoords[i][j-1] = ( correctDot[0] - X)  / X_PsiP;
                        this->arrayCanvasYCoords[i][j-1] = (-correctDot[1] + Y)  / Y_PsiP;
                        break;
                    case 1110: // Y1 не попал в диапазон
                        if(this->debug) std::cout << "Пограничная ситуация (Y1) this->scene->addLine("<<x0<<", "<<y0<<",  "<<x1<<", "<<y1<<",  pen);\n";
                        correctDot = this->CorrectY1(exp, x0, y0, x1, y1);
                        if(correctDot[0] != correctDot[2] && correctDot[1] != correctDot[3]) {
                            x1     = correctDot[2];
                            y1     = correctDot[3];
                            cnv_x1 = ( correctDot[2] - X) / X_PsiP;
                            cnv_y1 = (-correctDot[3] + Y) / Y_PsiP;
                        }
                        break;
                }
            }

            this->arrayXCoords[i][j] = x1;
            this->arrayYCoords[i][j] = y1;

            this->arrayCanvasXCoords[i][j] = cnv_x1;
            this->arrayCanvasYCoords[i][j] = cnv_y1;
        }
        if(this->debug) std::cout << "\n";
    }
}



/**
 * @brief Graphics::ShowLines
 * Отрисовывает линии графиков по двум рассчитанным точкам
 */
void Graphics::ShowLines() {
    int dots  = this->GetDots();
    double X  = this->cp->GetX();
    double Y  = this->cp->GetY();
    double Xe = this->cp->GetXe();
    double Ye = this->cp->GetYe();

    for(int i=0; i < this->graphics_numbers; i++) {
        QPen pen(*this->GetGraphicColor(i));
        std::string exp = GetGraphicString(i);
        if( !this->CheckExpression(exp) )
            continue;

        for(int j=0; j < dots; j++) {
            if(j) {
                double x0 = this->arrayXCoords[i][j-1];
                double y0 = this->arrayYCoords[i][j-1];
                double x1 = this->arrayXCoords[i][j];
                double y1 = this->arrayYCoords[i][j];

                double cnv_x0 = this->arrayCanvasXCoords[i][j-1];
                double cnv_y0 = this->arrayCanvasYCoords[i][j-1];
                double cnv_x1 = this->arrayCanvasXCoords[i][j];
                double cnv_y1 = this->arrayCanvasYCoords[i][j];

                bool condition_x0 = (x0 >= X && x0 <= Xe) ? true : false;
                bool condition_x1 = (x1 >= X && x1 <= Xe) ? true : false;
                bool condition_y0 = (y0 <= Y && y0 >= Ye);
                bool condition_y1 = (y1 <= Y && y1 >= Ye);

                if(condition_x0 && condition_y0 && condition_x1 && condition_y1) {
                    this->scene->addLine(cnv_x0, cnv_y0,  cnv_x1, cnv_y1,  pen);
                    if(this->debug) std::cout << "Добавляем this->scene->addLine("<<x0<<", "<<y0<<", "<<x1<<", "<<y1<<", pen);\n";
                }
                else {
                    if(this->debug) std::cout << "Пропускаем this->scene->addLine("<<x0<<", "<<y0<<", "<<x1<<", "<<y1<<", pen);\n";
                }
            }
        }
    }
}



/**
 * @brief Graphics::CorrectY0
 * Корректировка значения 'Y' первой точки, выходящего за пределы отображаемой части координатной плоскости
 * @param exp -- математическое выражение
 * @param X0  -- значение X первой точки
 * @param Y0  -- значение Y первой точки
 * @param X1  -- значение X второй точки
 * @param Y1  -- значение Y второй точки
 * @return double -- откорректированное значение
 */
double *Graphics::CorrectY0(std::string exp, double X0, double Y0, double X1, double Y1) {
    if(this->debug) std::cout << "Переопределение координат с { "<<X0<<", "<<Y0<<", "<<X1<<",  "<<Y1<<"} ";
    double  cpY  = this->cp->GetY();
    double  cpYe = this->cp->GetYe();
    double *newCoord = new double[4];

    double differenceY = Y0 - Y1;
    double differenceX = X1 - X0;
    int    differenceXDots  = 10;
    double deviation  = differenceY / differenceXDots; // рассчитываем допустимое отклонение от края координатной плоскости
    deviation = deviation * (deviation > 0 ? 1 : -1);
    double cpY_dev  = cpY  - (deviation * 1.2); // Допустимая точка отклонения по верхнему краю
    double cpYe_dev = cpYe + (deviation * 1.2); // Допустимая точка отклонения по нижнему  краю

    double differenceXStep  = differenceX / differenceXDots;

    double currX0 = 0;
    double currY0 = 0;
    bool successfullyCorrect = false;

    for(int i=0; i <= differenceXDots; i++) {
        currX0 = X0 + i * differenceXStep;
        currY0 = this->CalculateYValue(exp, currX0);

        if(
            (cpYe <= currY0 && currY0 <= cpYe_dev) || // по нижнему пределу
            (cpY  >= currY0 && currY0 >= cpY_dev)     // по верхнему пределу
          ) {
            successfullyCorrect = true;
            break;
        }
    }

    if(successfullyCorrect) {
        X0 = currX0;
        Y0 = currY0;
    }

    newCoord[0] = X0;
    newCoord[1] = Y0;
    newCoord[2] = X1;
    newCoord[3] = Y1;

    if(this->debug)  std::cout << "на { "<<X0<<", "<<Y0<<", "<<X1<<",  "<<Y1<<"};\n";

    return newCoord;
}



/**
 * @brief Graphics::CorrectY1
 * Корректировка значения 'Y' второй точки, выходящего за пределы отображаемой части координатной плоскости
 * @param exp -- математическое выражение
 * @param X0  -- значение X первой точки
 * @param Y0  -- значение Y первой точки
 * @param X1  -- значение X второй точки
 * @param Y1  -- значение Y второй точки
 * @return double -- откорректированное значение
 */
double *Graphics::CorrectY1(std::string exp, double X0, double Y0, double X1, double Y1) {
    if(this->debug) std::cout << "Переопределение координат с { "<<X0<<", "<<Y0<<", "<<X1<<",  "<<Y1<<"} ";
    double cpY  = this->cp->GetY();
    double cpYe = this->cp->GetYe();
    double* newCoord = new double[4];

    double differenceY = Y0 - Y1;
    double differenceX = X1 - X0;
    int    differenceXDots  = 10;
    double deviation  = differenceY / differenceXDots; // рассчитываем допустимое отклонение от края координатной плоскости
    deviation = deviation * (deviation > 0 ? 1 : -1);
    double cpYe_dev = cpYe + (deviation * 1.2); // Допустимая точка отклонения по верхнему краю
    double cpY_dev  = cpY  - (deviation * 1.2); // Допустимая точка отклонения по нижнему  краю

    double differenceXStep  = differenceX / differenceXDots;

    double currX1 = 0;
    double currY1 = 0;
    bool successfullyCorrect = false;

    for(int i=0; i <= differenceXDots; i++) {
        currX1 = X1 - i * differenceXStep;
        currY1 = this->CalculateYValue(exp, currX1);

        if(
            (cpYe <= currY1 && currY1 <= cpYe_dev) || // по нижнему пределу
            (cpY  >= currY1 && currY1 >= cpY_dev)     // по верхнему пределу
          ) {
            successfullyCorrect = true;
            break;
        }
    }

    if(successfullyCorrect) {
        X1 = currX1;
        Y1 = currY1;
    }

    newCoord[0] = X0;
    newCoord[1] = Y0;
    newCoord[2] = X1;
    newCoord[3] = Y1;

    if(this->debug)  std::cout << "на { "<<X0<<", "<<Y0<<", "<<X1<<",  "<<Y1<<"};\n";

    return newCoord;
}



/**
 * @brief Graphics::Show
 * Функция выводящая готовую сцену на canvas
 */
void Graphics::Show() {
    this->RefillCoordArrays();
    this->ShowLines();
    ui->graphicsView->setScene( this->scene );
}
