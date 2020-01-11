#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "ui_mainwindow.h"
#include "../coordPlane/coordplane.h"
#include "../../libs/calc_in_c/calc_in_c.h"


class Graphics {
protected:
    bool debug = false;

    Ui::MainWindow    *ui;
    CoordPlane        *cp;
    QGraphicsScene *scene;
    QWidget       *canvas;

    static const int  graphics_numbers = 3;
    static const int dots = 1001;

    double (*arrayXCoords)[dots] = new double[graphics_numbers][dots];
    double (*arrayYCoords)[dots] = new double[graphics_numbers][dots];
    double (*arrayCanvasXCoords)[dots] = new double[graphics_numbers][dots];
    double (*arrayCanvasYCoords)[dots] = new double[graphics_numbers][dots];

    std::string arrayGraphics[graphics_numbers];
    int         arrayGraphicsColors[graphics_numbers];

    static const int  usedColorsNums   = 8;
    QColor *usedColors[usedColorsNums] = {
        new QColor(  0,    0,    0), // black
        new QColor(  0,    0,  255), // blue
        new QColor(255,    0,  255), // fuchsia
        new QColor(255,  215,    0), // gold
        new QColor(  0,  128,    0), // green
        new QColor(173,  255,   47), // greenyellow
        new QColor(128,  128,  128), // grey
        new QColor(255,    0,    0), // red
    };

    bool    CheckExpression(std::string exp);
    void    ToDefaultCoordArrays();
    void    RefillCoordArrays();
    double *CorrectY0(std::string exp, double X0, double Y0, double X1, double Y1);
    double *CorrectY1(std::string exp, double X0, double Y0, double X1, double Y1);
    ~Graphics();



public:
    Graphics(Ui::MainWindow *ui, CoordPlane *cp, QGraphicsScene *scene);

    bool    GetDebug();
    void    SetDebug(bool debug);

    std::string GetGraphicString(int num);
    void        SetGraphicString(int num, QString exp);
    QColor     *GetGraphicColor(int num);
    void        SetGraphicColor(int num, int color);

    double  CalculateYValue(std::string exp, double X);
    int     GetDots();
    void    ShowLines();
    void    Show();
};
#endif // GRAPHICS_H
