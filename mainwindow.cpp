#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "classes/coordPlane/coordplane.h"
#include "classes/graphics/graphics.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->scene    = new QGraphicsScene(ui->graphicsView);
    this->cplane   = new CoordPlane(this->ui, this->scene);
    this->graphics = new Graphics(this->ui, this->cplane, this->scene);
    this->debug    = this->ui->DebugFlag->isChecked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::GetDebug() {
    return this->debug;
}

void MainWindow::SetDebug(bool debug) {
    this->debug = debug;
}

void MainWindow::ShowGraph() {
    this->scene->clear();
    this->cplane->Show();
    this->graphics->Show();

    if( this->GetDebug() )
        std::cout << "-------------------------------------------\n\n\n";
}

void MainWindow::on_renderButton_clicked() { // Нажатие на кнопку "Отрис."
    this->ShowGraph();
}

/*
*********************************************************************
******  Кнопки изменения видимой части координатной плоскости   *****
*********************************************************************
*/
void MainWindow::on_scalePlane_valueChanged(double arg1) {
    this->cplane->SetScale(arg1);
    if( this->GetDebug() ) std::cout << "SetScale(" << arg1 << ")\n";
    this->ShowGraph();
}

void MainWindow::on_to_upButton_clicked() {
    double newY = this->cplane->GetY() + ui->to_upStep->value();
    //std::cout << "newY = " << this->cplane->GetY() << " + " << ui->to_upStep->value() << "\n";
    this->cplane->SetY(newY);
    if( this->GetDebug() ) std::cout << "SetY(" << newY << ")\n";
    this->ShowGraph();
}

void MainWindow::on_to_rightButton_clicked() {
    double newX = this->cplane->GetX() + ui->to_rightStep->value();
    this->cplane->SetX(newX);
    if( this->GetDebug() ) std::cout << "SetX+(" << newX << ")\n";
    this->ShowGraph();
}

void MainWindow::on_to_downButton_clicked() {
    double newY = this->cplane->GetY() - ui->to_downStep->value();
    this->cplane->SetY(newY);
    if( this->GetDebug() ) std::cout << "SetY+(" << newY << ")\n";
    this->ShowGraph();
}


void MainWindow::on_to_leftButton_clicked() {
    double newX = this->cplane->GetX() - ui->to_leftStep->value();
    this->cplane->SetX(newX);
    if( this->GetDebug() ) std::cout << "SetX-(" << newX << ")\n";
    this->ShowGraph();
}

void MainWindow::on_to_leftUpButton_clicked() {
    this->on_to_leftButton_clicked();
    this->on_to_upButton_clicked();
}

void MainWindow::on_to_rightUpButton_clicked() {
    this->on_to_rightButton_clicked();
    this->on_to_upButton_clicked();
}

void MainWindow::on_to_rightBottomButton_clicked() {
    this->on_to_rightButton_clicked();
    this->on_to_downButton_clicked();
}

void MainWindow::on_to_leftDownButton_clicked() {
    this->on_to_leftButton_clicked();
    this->on_to_downButton_clicked();
}

void MainWindow::on_DebugFlag_stateChanged(int arg1) {
    bool b = (arg1 ? true : false);
    this->SetDebug(b);
    this->cplane->SetDebug(b);
    this->graphics->SetDebug(b);
}

void MainWindow::on_gFunc0_textChanged(const QString &arg1) {
    this->graphics->SetGraphicString(0, arg1);
}

void MainWindow::on_color_graph0_currentIndexChanged(int index) {
    this->graphics->SetGraphicColor(0, index);
}

void MainWindow::on_gFunc1_textChanged(const QString &arg1) {
    this->graphics->SetGraphicString(1, arg1);
}

void MainWindow::on_color_graph1_currentIndexChanged(int index){
    this->graphics->SetGraphicColor(1, index);
}

void MainWindow::on_gFunc2_textChanged(const QString &arg1) {
    this->graphics->SetGraphicString(2, arg1);
}

void MainWindow::on_color_graph2_currentIndexChanged(int index) {
    this->graphics->SetGraphicColor(2, index);
}
