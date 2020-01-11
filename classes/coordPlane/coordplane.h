#ifndef COORDPLANE_H
#define COORDPLANE_H

#include "ui_mainwindow.h"
#include <QGraphicsTextItem>

class CoordPlane {
protected:
    bool   debug = false;
    double defaultDouble = -10000; // std::numeric_limits<double>::is_iec559;

    double X     = 0.0;
    double Xe    = 0.0;
    double Y     = 0.0;
    double Ye    = 0.0;
    double scale = 0.0;

    int    steps = 0;
    double step  = 0.0;
    double stepCanvasWidth  = 0.0;
    double stepCanvasHeight = 0.0;

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QWidget        *canvas;

    int    realWidthCanvas  = 0;
    double GetUsedCanvasWidth();
    void   SetUsedCanvasWidth(double width);

    int    realHeightCanvas = 0;
    double GetUsedCanvasHeight();
    void   SetUsedCanvasHeight(double height);

    int    maxLines = 11;
    int    displayedLines = maxLines;

    double EndCoordLineX = 0.0;
    double coordLinesX[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // вертикальные
    double coordLinesY[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // горизонтальные

    double canvasLeftMargin = defaultDouble;
    double GetCanvasLeftMargin();
    void   SetCanvasLeftMargin(double canvasLeftMargin);

    double canvasTopMargin = defaultDouble; // горизонтальный отступ
    double GetCanvasTopMargin();
    void   SetCanvasTopMargin(double canvasTopMargin);

    double procentMarginLeft = defaultDouble;
    double GetProcentMarginLeft();
    void   SetProcentMarginLeft(double pml);

    double procentMarginRight = defaultDouble;
    double GetProcentMarginRight();
    void   SetProcentMarginRight(double pmr);

    double procentMarginTop = defaultDouble;
    double GetProcentMarginTop();
    void   SetProcentMarginTop(double pmt);

    double procentMarginBottom = defaultDouble;
    double GetProcentMarginBottom();
    void   SetProcentMarginBottom(double pmb);

    double fromX = 0;
    double GetFromX();
    void   SetFromX(double fromX);

    double toX = 0;
    double GetToX();
    void   SetToX(double toX);

    double fromY = 0;
    double GetFromY();
    void   SetFromY(double fromY);

    double toY = 0;
    double GetToY();
    void   SetToY(double toY);

    int    X_lines = 0;
    int    GetXLines();
    void   SetXLines(int lines);

    int    Y_lines = 0;
    int    GetYLines();
    void   SetYLines(int lines);

    void   RefillCoordLinesArray();
    void   ShowCoordLines();
    void   ShowCoordLineNums();

    void   SetStep(double step);
    void   SetSteps(int steps);
    double GetStepCanvasWidth();
    void   SetStepCanvasWidth(double width);
    double GetStepCanvasHeight();
    void   SetStepCanvasHeight(double height);

    std::string ToHandleNumber(std::string str);

    void   ResetWorkProperties();
    void   ClearScene();

    double X_PsiP = 0.0;
    double Y_PsiP = 0.0;
    void   SetX_PsiP(double coeff);
    void   SetY_PsiP(double coeff);
    ~CoordPlane();


public:
    CoordPlane(Ui::MainWindow *ui, QGraphicsScene *scene, double x = -5, double y = 5, double scale = 10);

    bool   GetDebug();
    void   SetDebug(bool debug);

    double GetX();
    void   SetX(double x);

    double GetXe(); // X end
    void   SetXe(double x);

    double GetY();
    void   SetY(double y);

    double GetYe(); // Y end
    void   SetYe(double y);

    double GetScale();
    void   SetScale(double sc);

    double GetStep();
    int    GetSteps();

    double GetX_PsiP();
    double GetY_PsiP();

    void   Show();
};

#endif // COORDPLANE_H
