#include "coordplane.h"
#include <iostream>
#include <math.h>

/**
 * @brief CoordPlane::CoordPlane
 * Конструктор класса
 * @param ui     -- ui (user interface) объект Qt
 * @param scene  -- сцена для canvas
 * @param x      -- координата 'x' верхней левой точки
 * @param y      -- координата 'y' верхней левой точки
 * @param scale  -- масштаб отображения координатной плоскости
 */
CoordPlane::CoordPlane(Ui::MainWindow *ui, QGraphicsScene *scene, double x, double y, double scale) {
    this->SetX(x);
    this->SetY(y);
    this->SetScale(scale);
    this->ui     = ui;
    this->scene  = scene;
    this->debug  = this->ui->DebugFlag->isChecked();
    this->canvas = this->ui->graphicsView->viewport();
}



/**
 * @brief CoordPlane::~CoordPlane
 * Деструктор класса
 */
CoordPlane::~CoordPlane() {
    // Нечего удалять, т.к. мы внутри класса ни чего не создаем
}



/**
 * @brief CoordPlane::GetDebug
 * Возвращает значение флага отладки
 * @return bool -- значение флага отладки
 */
bool CoordPlane::GetDebug() {
    return this->debug;
}



/**
 * @brief CoordPlane::SetDebug
 * Устанавливает/спускает флаг отладки
 * @param bool debug -- значение флага
 */
void CoordPlane::SetDebug(bool debug) {
    this->debug = debug;
}



/**
 * @brief CoordPlane::GetX
 * Возвращает крайне-левое значение по шкале 'X' координатной плоскости
 * @return double -- крайне-левое значение по шкале 'X'
 */
double CoordPlane::GetX() {
    return this->X;
}



/**
 * @brief CoordPlane::SetX
 * Устанавливает крайне-левое значение по шкале 'X' координатной плоскости
 * @param x -- крайне-левое значение по шкале 'X'
 */
void CoordPlane::SetX(double x) {
    this->X = x;
}



/**
 * @brief CoordPlane::GetXe
 * Возвращает крайне-правое значение по шкале 'X' координатной плоскости
 * @return double -- значение координаты 'Xe'
 */
double CoordPlane::GetXe() {
    double Xe = this->Xe;
    if(Xe == 0) {
        Xe = this->GetX() + this->GetScale();
        if(this->debug) { std::cout << "Xe = " << this->GetX() << " + " << this->GetScale() << " = " << Xe << ";\n"; }
        this->SetXe(Xe);
    }
    return Xe;
}



/**
 * @brief CoordPlane::SetXe
 * Устанавливает крайне-правое значение по шкале 'X' координатной плоскости
 * @param Xe -- значение координаты 'Xe'
 */
void CoordPlane::SetXe(double Xe) {
    this->Xe = Xe;
}



/**
 * @brief CoordPlane::GetY
 * Возвращает верхнее значение по шкале 'Y' координатной плоскости
 * @return double -- значение координаты 'Y'
 */
double CoordPlane::GetY() {
    return this->Y;
}



/**
 * @brief CoordPlane::SetY
 * Устанавливает верхнее значение по шкале 'Y' координатной плоскости
 * @param y -- значение координаты 'Y'
 */
void CoordPlane::SetY(double y) {
    this->Y = y;
}



/**
 * @brief CoordPlane::GetYe
 * Возвращает нижнее значение по шкале 'Y' координатной плоскости
 * @return double -- значение координаты 'Ye'
 */
double CoordPlane::GetYe() {
    double Ye = this->Ye;
    if(Ye == 0) {
        Ye = this->GetY() - this->GetScale();
        if(this->debug) std::cout << "Ye = " << Ye << ";\n";
        this->SetYe(Ye);
    }

    return Ye;
}



/**
 * @brief CoordPlane::SetYe
 * Устанавливает нижнее значение по шкале 'Y' координатной плоскости
 * @param Ye -- значение координаты 'Ye'
 */
void CoordPlane::SetYe(double Ye) {
    this->Ye = Ye;
}



/**
 * @brief CoordPlane::GetScale
 * Возвращает значение масштаба
 * @return double -- значение масштаба
 */
double CoordPlane::GetScale() {
    return this->scale;
}



/**
 * @brief CoordPlane::SetScale
 * Устанавливает значение масштаба
 * @param scale -- значение масштаба
 */
void CoordPlane::SetScale(double scale) {
    this->scale = scale;
}



/**
 * @brief CoordPlane::GetSteps
 * Возвращает примерное (рассчитанное в лоб) количество шагов
 * @return int -- примерное количество шагов
 */
int  CoordPlane::GetSteps() {
    // Кол-во шагов определяем динамически
    int steps = this->steps;

    if(steps == 0) {
        int stepSize;
        stepSize = ceil(this->GetScale() / 10);
        steps = this->GetScale() / stepSize;
        this->maxLines = steps + 1;
        if(this->debug) std::cout << "steps = " << steps << ";\n";
        this->SetSteps(steps);
    }

    return steps;
}



/**
 * @brief CoordPlane::SetSteps
 * Устанавливает значение примерного (рассчитанного в лоб) количества шагов
 * @param steps -- количество шагов
 */
void CoordPlane::SetSteps(int steps) {
    this->steps = steps;
}



/**
 * @brief CoordPlane::GetXLines
 * Возвращает кол-во линий отрисовываемых по оси X
 * @return int -- кол-во линий
 */
int CoordPlane::GetXLines() {
    int lines = this->X_lines;

    if(lines == 0) {
        int stepLength = this->GetStep();
        double fromX = this->GetFromX();
        double toX = this->GetXe();
        double currentStep = fromX;

        if(this->debug) std::cout  << "XLines: " << currentStep << ", ";

        while(currentStep + stepLength <= toX) {
            lines++;
            currentStep += stepLength;
            if(this->debug) std::cout << currentStep << ", ";
        }
        this->SetXLines(++lines);
       if(this->debug) std::cout << "; steps = " << lines << ";\n";
    }

    return lines;
}



/**
 * @brief CoordPlane::SetXLines
 * Устанавливает значение кол-ва линий отрисовываемых по оси X
 * @param lines -- кол-во линий
 */
void CoordPlane::SetXLines(int lines) {
    this->X_lines = lines;
}



/**
 * @brief CoordPlane::GetYLines
 * Возвращает кол-во линий отрисовываемых по оси Y
 * @return int -- кол-во линий
 */
int CoordPlane::GetYLines() {
    int lines = this->Y_lines;

    if(lines == 0) {
        int stepLength = this->GetStep();
        double toY = this->GetYe();
        double fromY = this->GetFromY();
        double currentStep = fromY;
        if(this->debug) {
            std::cout << "Y: " << fromY << " -> " << toY << "\n";
            std::cout  << "YLines: " << currentStep << ", ";
        }
        while(currentStep - stepLength >= toY) {
            lines++;
            currentStep -= stepLength;
            if(this->debug) std::cout << currentStep << ", ";
        }
        this->SetYLines(++lines);
        if(this->debug) std::cout << "; steps = " << lines << ";\n";

    }

    return lines;
}



/**
 * @brief CoordPlane::SetYLines
 * Устанавливает значение кол-ва линий отрисовываемых по оси Y
 * @param lines -- кол-во линий
 */
void CoordPlane::SetYLines(int lines) {
    this->Y_lines = lines;
}



/**
 * @brief CoordPlane::GetFromX
 * Возвращает значение 'X' первой линии по оси X
 * @return double -- значение 'X' первой линии по оси X
 */
double CoordPlane::GetFromX() {
    double fromX = this->fromX;

    if(fromX == 0) {
        int correctX = this->GetX()*100;
        int stepLength = this->GetStep();
        int remainder = correctX % (stepLength * 100);
        remainder = correctX >= 0 ? -remainder : remainder;
        fromX = (correctX - remainder ) / 100;
        if(this->debug) {
            std::cout << "fromX = (" << correctX << " - (" << (correctX >= 0 ? "-" : "") << correctX << " % (" << stepLength << " * " << 100 << ") ) ) / " << 100 << "    =    ";
            std::cout <<  "(" << correctX << " - (" << (correctX >= 0 ? "-" : "") << correctX << " % (" << stepLength * 100 << ") ) ) / " << 100 << "    =    ";
            std::cout <<  "(" << correctX << " - (" << (correctX >= 0 ? "-" : "") << (correctX % (stepLength * 100)) << ") ) / " << 100 << "    =    ";
            std::cout <<  "(" << (correctX + (correctX % (stepLength * 100))) << ") / " << 100 << "     =     ";
            std::cout <<  fromX << "\n";
        }

        this->SetFromX(fromX);
    }

    return fromX;
}



/**
 * @brief CoordPlane::SetFromX
 * Устанавливает значение 'X' первой линии по оси X
 * @param fromX double -- значение 'X' первой линии по оси X
 */
void   CoordPlane::SetFromX(double fromX) {
    this->fromX = fromX;
}



/**
 * @brief CoordPlane::GetToX
 * Возвращает значение 'X' последней линии по оси X
 * @return double -- значение 'X' последней линии по оси X
 */
double CoordPlane::GetToX() {
    double toX = this->toX;

    if(toX == 0) {
        double fromX = this->GetFromX();
        double step  = this->GetStep();
        int    lines = this->GetXLines() - 1;
        toX = fromX + (step * lines);
        if(this->debug) std::cout << "toX = " << fromX << " + (" << step << " * " << lines << ") = " << toX << ";\n";
        this->SetToX(toX);
    }

    return toX;
}



/**
 * @brief CoordPlane::SetToX
 * Устанавливает значение 'X' последней линии по оси X
 * @param toX -- значение 'X' последней линии по оси X
 */
void   CoordPlane::SetToX(double toX) {
    this->toX = toX;
}



/**
 * @brief CoordPlane::GetFromY
 * Возвращает значение 'Y' первой линии по оси Y
 * @return double -- значение 'Y' первой линии по оси Y
 */
double CoordPlane::GetFromY() {
    double fromY = this->fromY;

    if(fromY == 0) {
        int correctY = this->GetY()*100;
        int stepLength = this->GetStep();
        fromY = (correctY - abs(correctY % (stepLength * 100) ) ) / 100;
        if(this->debug) std::cout << "fromY = (" << correctY << " - (" << correctY << " % (" << stepLength << " * " << 100 << ") ) ) / " << 100 << " = " << fromY << ";\n";
        this->SetFromY(fromY);
    }

    return fromY;
}



/**
 * @brief CoordPlane::SetFromY
 * Устанавливает значение 'Y' первой линии по оси Y
 * @param fromY -- значение 'Y' первой линии по оси Y
 */
void   CoordPlane::SetFromY(double fromY) {
    this->fromY = fromY;
}



/**
 * @brief CoordPlane::GetToY
 * Возвращает значение 'Y' последней линии по оси Y
 * @return double -- значение 'Y' последней линии по оси Y
 */
double CoordPlane::GetToY() {
    double toY = this->toY;

    if(toY == 0) {
        double fromY = this->GetFromY();
        double step  = this->GetStep();
        int    lines = this->GetYLines() - 1 ;
        toY = fromY - (step * lines);
        if(this->debug) std::cout << "toY = " << fromY << " - (" << step << " * " << lines << ") = " << toY << ";\n";
        this->SetToY(toY);
    }
    return toY;
}



/**
 * @brief CoordPlane::SetToY
 * Устанавливает значение 'Y' последней линии по оси Y
 * @param toY -- значение 'Y' последней линии по оси Y
 */
void   CoordPlane::SetToY(double toY) {
    this->toY = toY;
}



/**
 * @brief CoordPlane::GetStep
 * Возвращает величину шага на графике
 * @return double -- величина шага
 */
double CoordPlane::GetStep() {
    double step = this->step;

    if(step == 0) {
        step = ceil( this->GetScale() / this->GetSteps() );
        this->SetStep(step);
        if(this->debug) std::cout << "step = " << step << ";\n";
    }

    return step;
}



/**
 * @brief CoordPlane::SetStep
 * Устанавливает размер одного шага (между значениями соседних линий координатной прямой)
 * @param step -- назначаемое значение шага
 */
void CoordPlane::SetStep(double step) {
    this->step = step;
}



/**
 * @brief CoordPlane::GetStepCanvasWidth
 * Возвращает величину шага в canvas при построении горизонтальных линий
 * @return double -- величина горизонтального шага в canvas
 */
double CoordPlane::GetStepCanvasWidth() {
    double stepCanvasWidth = this->stepCanvasWidth;

    if(stepCanvasWidth == 0) {
        // -1 т.к. первая в нулевой точке.
        int lines = this->GetXLines() - 1;

        double margin = this->GetProcentMarginLeft() + this->GetProcentMarginRight();
        stepCanvasWidth = (this->GetUsedCanvasWidth() * (1 - margin)) / lines; // / this->GetSteps()
        if(this->debug) std::cout << "stepCanvasHeight = " << (this->GetUsedCanvasWidth() * (1 - margin)) << " / " << lines << " = " << stepCanvasWidth << ";\n";
        this->SetStepCanvasWidth( stepCanvasWidth );
    }

    return stepCanvasWidth;
}



/**
 * @brief CoordPlane::SetStepCanvasWidth
 * Устанавливает рассчитанное значение горизонтальной величины шага
 * @param width -- величина горизонтального шага
 */
void CoordPlane::SetStepCanvasWidth(double width) {
    this->stepCanvasWidth = width;
}



/**
 * @brief CoordPlane::GetStepCanvasHeight
 * Возвращает величину шага в canvas при построении вертикальных линий
 * @return double -- величина вертикального шага в canvas
 */
double CoordPlane::GetStepCanvasHeight() {
    double stepCanvasHeight = this->stepCanvasHeight;

    if( stepCanvasHeight == 0 ) {
        // -1 т.к. первая в нулевой точке.
        int lines = this->GetYLines() - 1;
        double margin = this->GetProcentMarginTop() + this->GetProcentMarginBottom();
        stepCanvasHeight = (this->GetUsedCanvasHeight() * (1 - margin)) / lines; // / this->GetSteps()
        if(this->debug) std::cout << "stepCanvasHeight = " << (this->GetUsedCanvasHeight() * (1 - margin)) << " / " << lines << " = " << stepCanvasHeight << ";\n";
        this->SetStepCanvasHeight( stepCanvasHeight );
    }

    return stepCanvasHeight;
}



/**
 * @brief CoordPlane::SetStepCanvasHeight
 * Устанавливает рассчитанное значение вертикальной величины шага
 * @param height -- величина вертикального шага
 */
void CoordPlane::SetStepCanvasHeight(double height) {
    this->stepCanvasHeight = height;
}



/**
 * @brief CoordPlane::GetUsedCanvasWidth
 * Возвращает рассчитанное значение ширины холста
 * @return double -- значение ширины холста
 */
double CoordPlane::GetUsedCanvasWidth() {
    double realWidthCanvas = this->realWidthCanvas;

    if(realWidthCanvas == 0) {
        realWidthCanvas = this->canvas->width() - 50;
        this->realWidthCanvas = realWidthCanvas;
        if(this->debug) std::cout << "realWidthCanvas = " << realWidthCanvas << "\n";
    }

    return realWidthCanvas;
}



/**
 * @brief CoordPlane::SetUsedCanvasWidth
 * Устанавливает рассчитанное значение ширины холста
 * @param width -- значение ширины холста
 */
void CoordPlane::SetUsedCanvasWidth(double width) {
    this->realWidthCanvas = width;
}



/**
 * @brief CoordPlane::GetUsedCanvasHeight
 * Возвращает рассчитанное значение высоты холста
 * @return double -- значение высоты холста
 */
double CoordPlane::GetUsedCanvasHeight() {
    double realHeightCanvas = this->realHeightCanvas;

    if(realHeightCanvas == 0) {
        realHeightCanvas = this->canvas->height() - 50;
        this->realHeightCanvas = realHeightCanvas;
        if(this->debug) std::cout << "realHeightCanvas = " << realHeightCanvas << "\n";
    }

    return realHeightCanvas;
}



/**
 * @brief CoordPlane::SetUsedCanvasHeight
 * Устанавливает рассчитанное значение высоты холста
 * @param height -- значение высоты холста
 */
void CoordPlane::SetUsedCanvasHeight(double height) {
    this->realHeightCanvas = height;
}



/**
 * @brief CoordPlane::GetX_PsiP
 * Возвращает кол-во точек холста на одну точку координатной прямой по X
 * @return double -- коэффициент соотношения точек по X
 */
double CoordPlane::GetX_PsiP() { // Points in Point
    double X_PsiP = this->X_PsiP;

    if(X_PsiP == 0) {
        X_PsiP = ( this->GetScale() / this->GetUsedCanvasWidth() );
        if(this->debug) std::cout << "X_PsiP = " << X_PsiP << "\n";
    }

    return X_PsiP;
}



/**
 * @brief CoordPlane::SetX_PsiP
 * Устанавливает рассчитанное значение кол-ва точек холста на одну точку координатной прямой по X
 * @param coeff -- рассчитанное значение коэффициента соотношения точек по X
 */
void CoordPlane::SetX_PsiP(double coeff) {
    this->X_PsiP = coeff;
}



/**
 * @brief CoordPlane::GetY_PsiP
 * Возвращает кол-во точек холста на одну точку координатной прямой по Y
 * @return double -- коэффициент соотношения точек по Y
 */
double CoordPlane::GetY_PsiP() { // Points in Point
    double Y_PsiP = this->Y_PsiP;

    if(Y_PsiP == 0) {
        Y_PsiP = ( this->GetScale() / this->GetUsedCanvasHeight() );
        if(this->debug) std::cout << "Y_PsiP = " << Y_PsiP << "\n";
    }
    return Y_PsiP;
}



/**
 * @brief CoordPlane::SetY_PsiP
 * Устанавливает рассчитанное значение кол-ва точек холста на одну точку координатной прямой по Y
 * @param coeff -- рассчитанное значение коэффициента соотношения точек по Y
 */
void CoordPlane::SetY_PsiP(double coeff) {
    this->Y_PsiP = coeff;
}



/**
 * @brief CoordPlane::GetProcentMarginLeft
 * Возвращает величину отступа (в процентах от величины горизонтального шага)
 * от левого края координатной плоскости до первой линии по оси X
 * @return double -- величина левого отступа в процентном отношении
 */
double CoordPlane::GetProcentMarginLeft() {
    double marginLeft = this->procentMarginLeft;

    if(marginLeft == this->defaultDouble) {
        double correctX     = this->GetX() * 100;
        double correctFromX = this->GetFromX() * 100;
        double correctScale = this->GetScale() * 100;
        marginLeft = abs(correctX - correctFromX) / correctScale;
        if(this->debug) {
            std::cout << "marginLeft = " << "abs(" << correctX << " - " << correctFromX << ") / " << correctScale << " = ";
            std::cout << "abs(" << correctX - correctFromX << ") / " << correctScale << " = ";
            std::cout << abs(correctX - correctFromX) << " / " << correctScale << " = ";
            std::cout << marginLeft << "\n";
        }
        this->SetProcentMarginLeft( marginLeft );
    }

    return marginLeft;
}



/**
 * @brief CoordPlane::SetProcentMarginLeft
 * Устанавливает рассчитанное значение величины отступа (в процентах от величины горизонтального шага)
 * от левого края координатной плоскости до первой линии по оси X
 * @param pml -- величина левого отступа в процентном отношении
 */
void CoordPlane::SetProcentMarginLeft(double pml) {
    this->procentMarginLeft = pml;
}



/**
 * @brief CoordPlane::GetProcentMarginRight
 * Возвращает величину отступа (в процентах от величины горизонтального шага)
 * от правого края координатной плоскости до последней линии по оси X
 * @return double -- величина правого отступа в процентном отношении
 */
double CoordPlane::GetProcentMarginRight() {
    double marginRight = this->procentMarginRight;

    if(marginRight == this->defaultDouble) {
        double correctXe = this->GetXe() * 100;
        double correctToX  = this->GetToX() * 100;
        double correctScale = this->GetScale() * 100;
        marginRight = abs(correctXe - correctToX) / correctScale;
        if(this->debug) std::cout << "marginRight = " <<  "(" << correctXe <<" - " << correctToX << ") / " << correctScale << " = " << marginRight << "\n";
        this->SetProcentMarginRight( marginRight );
    }

    return marginRight;
}



/**
 * @brief CoordPlane::SetProcentMarginRight
 * Устанавливает рассчитанное значение величины отступа (в процентах от величины горизонтального шага)
 * от правого края координатной плоскости до последней линии по оси X
 * @param pmr -- величина правого отступа в процентном отношении
 */
void CoordPlane::SetProcentMarginRight(double pmr) {
    this->procentMarginRight = pmr;
}



/**
 * @brief CoordPlane::GetProcentMarginTop
 * Возвращает величину отступа (в процентах от величины вертикального шага)
 * от верхнего края координатной плоскости до первой линии по оси Y
 * @return double -- величина верхнего отступа в процентном отношении
 */
double CoordPlane::GetProcentMarginTop() {
    double marginTop = this->procentMarginTop;

    if(marginTop == this->defaultDouble) {
        marginTop = (this->GetY() - this->GetFromY()) / this->GetScale();
        if(this->debug) std::cout << "marginTop = " <<  "(" << this->GetY() <<" - " << this->GetFromY() << ") / " << this->GetScale() << " = " << marginTop << "\n";
        this->SetProcentMarginTop( marginTop );
    }

    return marginTop;
}



/**
 * @brief CoordPlane::SetProcentMarginTop
 * Устанавливает рассчитанное значение величины отступа (в процентах от величины вертикального шага)
 * от верхнего края координатной плоскости до первой линии по оси Y
 * @param pmt -- величина верхнего отступа в процентном отношении
 */
void CoordPlane::SetProcentMarginTop(double pmt) {
    this->procentMarginTop = pmt;
}



/**
 * @brief CoordPlane::GetProcentMarginBottom
 * Возвращает величину отступа (в процентах от величины вертикального шага)
 * от нижнего края координатной плоскости до последней линии по оси Y
 * @return -- величина нижнего отступа в процентном отношении
 */
double CoordPlane::GetProcentMarginBottom() {
    double marginBottom = this->procentMarginBottom;

    if(marginBottom == this->defaultDouble) {
        double correctYe = this->GetYe() * 100;
        double correctToY  = this->GetToY() * 100;
        double correctScale = this->GetScale() * 100;
        marginBottom = abs(correctYe - correctToY) / correctScale;
        if(this->debug) {
            std::cout << "marginBottom = " <<  "(" << correctYe <<" - " << correctToY << ") / " << correctScale << " = ";
            std::cout << "(" << correctYe - correctToY << ") / " << correctScale << " = " << marginBottom << "\n";
        }
        this->SetProcentMarginBottom( marginBottom );
    }

    return marginBottom;
}



/**
 * @brief CoordPlane::SetProcentMarginBottom
 * Устанавливает рассчитанное значение величины отступа (в процентах от величины вертикального шага)
 * от нижнего края координатной плоскости до последней линии по оси Y
 * @param pmb -- величина нижнего отступа в процентном отношении
 */
void CoordPlane::SetProcentMarginBottom(double pmb) {
    this->procentMarginBottom = pmb;
}



/**
 * @brief CoordPlane::GetCanvasLeftMargin
 * Возвращает величину отступа на холсте от левого края координатной плоскости до первой линии по оси X
 * @return double -- величина левого отступа на холсте
 */
double CoordPlane::GetCanvasLeftMargin() {
    double canvasLeftMargin = this->canvasLeftMargin;

    if(canvasLeftMargin == this->defaultDouble) {
        canvasLeftMargin = this->GetProcentMarginLeft() * this->GetUsedCanvasWidth();
       if(this->debug) std::cout << "canvasLeftMargin = " << this->GetProcentMarginLeft() << " * " << this->GetUsedCanvasWidth() << " = " << canvasLeftMargin << "\n";
        this->SetCanvasLeftMargin(canvasLeftMargin);
    }

    return canvasLeftMargin;
}



/**
 * @brief CoordPlane::SetCanvasLeftMargin
 * Устанавливает значение рассчитанной величины отступа на холсте от левого края координатной плоскости до первой линии по оси X
 * @param canvasLeftMargin -- величина левого отступа на холсте
 */
void CoordPlane::SetCanvasLeftMargin(double canvasLeftMargin) {
    this->canvasLeftMargin = canvasLeftMargin;
}



/**
 * @brief CoordPlane::GetCanvasTopMargin
 * Возвращает величину отступа на холсте от верхнего края координатной плоскости до первой линии по оси Y
 * @return double -- величина верхнего отступа на холсте
 */
double CoordPlane::GetCanvasTopMargin() {
    double canvasTopMargin = this->canvasTopMargin;

    if(canvasTopMargin == this->defaultDouble) {
        canvasTopMargin = this->GetProcentMarginTop() * this->GetUsedCanvasHeight();
        if(this->debug) std::cout << "canvasTopMargin = " << this->GetProcentMarginTop() << " * " << this->GetUsedCanvasHeight() << " = " << canvasTopMargin << "\n";
        this->SetCanvasTopMargin(canvasTopMargin);
    }

    return canvasTopMargin;
}



/**
 * @brief CoordPlane::SetCanvasTopMargin
 * Устанавливает значение рассчитанной величины отступа на холсте от верхнего края координатной плоскости до первой линии по оси Y
 * @param canvasTopMargin -- величина верхнего отступа на холсте
 */
void CoordPlane::SetCanvasTopMargin(double canvasTopMargin) {
    this->canvasTopMargin = canvasTopMargin;
}



/**
 * @brief RefillCoordLinesArray
 * Обновление значений линий координатной плоскости
 */
void CoordPlane::RefillCoordLinesArray() {
    int stepLength = this->GetStep();

    // Вертикальные линии
    int x_lines = this->GetXLines();
    double fromX = this->GetFromX();
    if(this->debug) {
        std::cout << "fromX = " << fromX << "; (" << this->GetX() << " -> " << this->GetXe() << ")\n";
        std::cout << "X: ";
    }

    for(int i=0; i < this->maxLines; i++) {
        if(i < x_lines) {
            this->coordLinesX[i] = fromX + (i * stepLength);
            if(this->debug) std::cout << this->coordLinesX[i] << ", ";
        }
        else {
            this->coordLinesX[i] = 0;
        }
    }
    if(this->debug) std::cout << "\n";

    // Горизонтальные линии
    int y_lines = this->GetYLines();
    // Определяем начальную току шага
    double fromY = this->GetFromY();
    if(this->debug) std::cout << "fromY = " << fromY << "; (" << this->GetY() << " -> " << this->GetYe() << ")\n";

    for(int i=0; i < this->maxLines; i++) {
        if(i < y_lines)
            this->coordLinesY[i] = fromY - (i * stepLength);
        else
            this->coordLinesY[i] = 0;
    }
}



/**
 * @brief CoordPlane::ShowCoordLines
 * Функция отвечающая за вывод вертикальных (по оси X) и горизонтальных линий (по оси Y)
 */
void CoordPlane::ShowCoordLines() {
    QPen pen(QColor(225, 225, 225));
    QPen penAxis(QColor(225, 225, 225), 3);
    int x_lines = this->GetXLines();
    double currX;
    double canvasLeftMargin = this->GetCanvasLeftMargin();
    for(int i=0; i < x_lines; i++) {
        currX = (i * this->GetStepCanvasWidth()) + canvasLeftMargin;
        if(this->coordLinesX[i])
            scene->addLine(currX, 0,  currX, this->GetUsedCanvasHeight(), pen);
        else
            scene->addLine(currX, 0,  currX, this->GetUsedCanvasHeight(), penAxis);
    }

    int y_lines = this->GetYLines();
    double currY;
    double canvasTopMargin = this->GetCanvasTopMargin();
    for(int i=0; i < y_lines; i++) {
        currY = (i * this->GetStepCanvasHeight()) + canvasTopMargin;
        if(this->coordLinesY[i])
            scene->addLine(0, currY, this->GetUsedCanvasWidth(), currY, pen);
        else
            scene->addLine(0, currY, this->GetUsedCanvasWidth(), currY, penAxis);
    }
}



/**
 * @brief CoordPlane::ShowCoordNums
 * Функция отвечающая за вывод числовых обозначений вертикальных (по оси X) и горизонтальных (по оси Y) линий
 */
void CoordPlane::ShowCoordLineNums() {
    int numsXLines = this->GetXLines();
    double canvasLeftMargin = this->GetCanvasLeftMargin();
    double currX, canvasX;
    double realHeightCanvas = this->GetUsedCanvasHeight() - 5;
    for(int i=0; i < numsXLines; i++) {
        canvasX = ( i * this->GetStepCanvasWidth() ) + canvasLeftMargin; // Вывод координат canvas
        currX = coordLinesX[i]; // Вывод реальных координат
        std::string currXString = this->ToHandleNumber( std::to_string(currX) );
        QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::fromUtf8(currXString.c_str()));
        textItem->setPos(canvasX-10, realHeightCanvas+35);
        textItem->setRotation(-90);
        scene->addItem(textItem);
    }

    int numsYLines = this->GetYLines();
    double canvasTopMargin = this->GetCanvasTopMargin();
    double currY, canvasY;
    for(int i=0; i < numsYLines; i++) {
        canvasY = ( i * this->GetStepCanvasHeight() ) + canvasTopMargin; // Вывод координат canvas
        currY = coordLinesY[i]; // Вывод реальных координат
        std::string currYString = this->ToHandleNumber( std::to_string(currY) );
        QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::fromUtf8(currYString.c_str()));
        textItem->setPos(-35, canvasY-10);
        scene->addItem(textItem);
    }


    if( this->debug ) {
        QColor debugTextColor = QColor(225, 0, 0);
        double usedCanvasHeight = this->GetUsedCanvasHeight();
        double usedCanvasWidth = this->GetUsedCanvasWidth();
        // По X
        std::string currXString = this->ToHandleNumber( std::to_string(this->GetX()) );
        QGraphicsTextItem *textX = new QGraphicsTextItem( QString::fromUtf8(currXString.c_str()) );
        textX->setDefaultTextColor( debugTextColor );
        textX->setPos(20, usedCanvasHeight-20);
        scene->addItem(textX);

        std::string currXeString = this->ToHandleNumber( std::to_string(this->GetXe()) );
        QGraphicsTextItem *textXe = new QGraphicsTextItem( QString::fromUtf8(currXeString.c_str()) );
        textXe->setDefaultTextColor( debugTextColor );
        textXe->setPos(usedCanvasWidth-50, usedCanvasHeight-20);
        scene->addItem(textXe);

        // По Y
        std::string currYString = this->ToHandleNumber( std::to_string(this->GetY()) );
        QGraphicsTextItem *textY = new QGraphicsTextItem( QString::fromUtf8(currYString.c_str()) );
        textY->setDefaultTextColor( debugTextColor );
        textY->setPos(0, 0);
        scene->addItem(textY);

        std::string currYeString = this->ToHandleNumber( std::to_string(this->GetYe()) );
        QGraphicsTextItem *textYe = new QGraphicsTextItem( QString::fromUtf8(currYeString.c_str()) );
        textYe->setDefaultTextColor( debugTextColor );
        textYe->setPos(0, usedCanvasHeight-40);
        scene->addItem(textYe);
    }
}



/**
 * @brief CoordPlane::ToHandleNumber
 * Удаляет лишние нули в дробной части числа
 * @param  std::string str -- строка с лишними нулями
 * @return std::string     -- строка без лишних нулей
 */
std::string CoordPlane::ToHandleNumber(std::string str) {
    int comma = -1;
    int lastNotNull = -1;
    int strLength = str.length();
    for(int i=0; i < strLength; i++) {
        if( comma >= 0 ) {
            // Находим в дробной части последний не нуль
            if( str[i] != '0' ) {
                lastNotNull = i;
            }
        }
        else if( str[i] == ',' ) { // Находим запятую
            comma = i;
        }
    }

    if( lastNotNull == -1) {
        lastNotNull = comma - 1;
    }

    // Стираем ненужные символы
    for(int i=(lastNotNull + 1); i < strLength; i++) {
        str[i] = ' ';
    }

    return str;
}



/**
 * @brief CoordPlane::ClearScene
 * Очистка canvas
 */
void CoordPlane::ClearScene() {
    this->scene->clear();
}



/**
 * @brief CoordPlane::Show
 * Функция отображения координатной плоскости
 */
void CoordPlane::Show() {
    this->ResetWorkProperties();
    this->ClearScene();
    this->RefillCoordLinesArray();
    this->ShowCoordLines();    // Выводим линии
    this->ShowCoordLineNums(); // Выводим значения линий

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
}



/**
 * @brief CoordPlane::ResetWorkProperties
 * Очистка рассчитываемых свойств класса
 */
void CoordPlane::ResetWorkProperties() {
    this->SetXe(0.0);
    this->SetYe(0.0);
    this->SetSteps(0);
    this->SetStep(0);
    this->SetStepCanvasWidth(0);
    this->SetStepCanvasHeight(0);
    this->SetXLines(0);
    this->SetYLines(0);
    this->SetFromX(0);
    this->SetFromY(0);
    this->SetCanvasLeftMargin(this->defaultDouble);
    this->SetCanvasTopMargin(this->defaultDouble);
    this->SetUsedCanvasWidth(0);
    this->SetUsedCanvasHeight(0);
    this->SetToX(0);
    this->SetToY(0);
    this->SetProcentMarginLeft(this->defaultDouble);
    this->SetProcentMarginRight(this->defaultDouble);
    this->SetProcentMarginTop(this->defaultDouble);
    this->SetProcentMarginBottom(this->defaultDouble);
    this->SetX_PsiP(0.0);
    this->SetY_PsiP(0.0);
}
