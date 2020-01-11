#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "classes/coordPlane/coordplane.h"
#include "classes/graphics/graphics.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ShowGraph();

private slots:
    void on_renderButton_clicked();

    void on_scalePlane_valueChanged(double arg1);

    void on_to_upButton_clicked();

    void on_to_rightButton_clicked();

    void on_to_downButton_clicked();

    void on_to_leftButton_clicked();

    void on_to_leftUpButton_clicked();

    void on_to_rightUpButton_clicked();

    void on_to_rightBottomButton_clicked();

    void on_to_leftDownButton_clicked();

    void on_DebugFlag_stateChanged(int arg1);

    void on_gFunc0_textChanged(const QString &arg1);

    void on_color_graph0_currentIndexChanged(int index);

    void on_gFunc1_textChanged(const QString &arg1);

    void on_color_graph1_currentIndexChanged(int index);

    void on_gFunc2_textChanged(const QString &arg1);

    void on_color_graph2_currentIndexChanged(int index);

private:
    Ui::MainWindow    *ui;
    QGraphicsScene *scene;
    CoordPlane    *cplane;
    Graphics    *graphics;

    bool debug;
    bool GetDebug();
    void SetDebug(bool debug);
};

#endif // MAINWINDOW_H
