#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <cmath>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <iostream>
#include <QPixmap>
#include <QDateTime>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
double m=0;
double mas=1;
MainWindow::~MainWindow()
{
    delete ui;  
}
void MainWindow::on_action_triggered()
{
    QMessageBox::information(this,"Руководство пользователя","Для использования программы нужно нажать кнопку Choose the file.\nС помощью подсписков можно масштабировать график и менять дискретизацию, а также посмотреть как работает программа.");
}
void MainWindow::on_action_2_triggered()
{
    QMessageBox::information(this,"Справка","Курсовой проект выполнил студент группы ИВТ-223 - Ожерельев Алексей Игоревич.");
}
QString readTextFromFile(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        return in.readAll();
    } else {
        qDebug() << "Не удалось открыть файл";
        return 0;
    }
}
void MainWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Text files(*.txt);;All files(*.*)");
    if (!filePath.isEmpty()) {
        QString fileContent = readTextFromFile(filePath);
        if (!fileContent.isEmpty()) {
            AAA = fileContent;
            needRedraw = false;
            z = 4;
            graph=true;
            update();
        }
        else {
            qDebug() << "Не удалось прочесть содержимое файла";
        }
    }
    else {
        qDebug() << "Файл не выбран";
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    QPixmap snapshot(this->size());
    this->render(&snapshot);
    QString fileName = "screenshot_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".png";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), fileName, tr("Images (*.png)"));
    if (!filePath.isEmpty()) {
        snapshot.save(filePath);
    }
}
void MainWindow::on_comboBox_activated(int index){
    if(index==0){
    z=index;
        graph=true;
    }
    if(index==1){
    z=index;
        graph=true;
    }
    if(index==2){
    z=index;
        graph=true;
    }
    if(index==3){
    z=index;
        graph=true;
    }
    update();
}
void MainWindow::on_comboBox_2_activated(int index)
{
    if(index==0){
        q=2;
        graph=true;
    }
    if(index==1){
        q=3;
        graph=true;
    }
    if(index==2){
        q=5;
        graph=true;
    }
    update();
}
void MainWindow::on_comboBox_3_activated(int index)
{
    if(index==0){
        mas=1;
        graph=true;
    }
    if(index==1){
        mas=2;
        graph=true;
    }
    if(index==2){
        mas=0.5;
        graph=true;
    }
    update();
}
double Func(int x,int z){
    double y=0;
    if(z==0){
    y=400-(sin((2*x*3.14)/180)+sin((4*x*3.14)/180))*50*mas;
    }
    if(z==1){
    y=400-(sin((x*3.14)/180)*50)*mas;
    }
    if(z==2){
    y=400-(cos((x*3.14)/180)*50)*mas;
    }
    if(z==3){
    y=400-(2*1*cos((1.14*(x*3.14/180)-50))*exp(-0.008*x))*100*mas;
    }
    return y;
}
double Func2(int x,int z){
    double y=0;
    if(z==0){
    y=sin((2*x*3.14)/180)+sin((4*x*3.14)/180);
    }
    if(z==1){
    y=sin((x*3.14)/180);
    }
    if(z==2){
    y=cos((x*3.14)/180);
    }
    if(z==3){
    y=2*1*cos((1.14*(x*3.14/180)-50))*exp(-0.008*x);
    }
    return y;
}
double MAX(int x,double max1,double*C) {
    int x2=0;
    double max2=max1*0.95;
    for (int i=x+1 ; i < 2000; i++) {
    if (C[i] > max2){
        if ((C[i-1]<C[i]) and (C[i]>C[i+1])){
            max2 = C[i];
            x2 = i;
            i = 2000;
        }
    }
    }
    return x2;
}
double MIN(int y,double min1,double*C) {
    int y2=0;
    double min2=min1*0.95;
    for (int i=y+1 ; i < 2000; i++) {
    if (C[i] < min2){
        if ((C[i-1]>C[i]) and (C[i]<C[i+1])){
            min2 = C[i];
            y2 = i;
            i = 2000;
        }
    }
    }
    return y2;
}
double MAX2(int x1,double max1,double*C){
    int i = 0;
    int g = MAX(x1, max1,C);
    int max2=x1;
    double T;
    int x3;
    while (i < 2000){
    T =  g - x1;
    x3 = MAX(max2,max1,C) + T;
    double c1 = C[x3];
    if (abs(c1 - max1)<0.1) {
        T = g - x1;
        i = 2000;
    }
    else {      
        max2=g;
        g=MAX(max2,max1,C);
        i ++;
    }
    }
    return T;
}
double MIN2(int y1,double min1,double*C){
    int i = 0;
    int g = MIN(y1, min1,C);
    double min2=y1;
    double T;
    int y3;
    while (i < 2000){
    T =  g - y1;
    y3= MIN(min2,min1,C) + T;
    double c1 = C[y3];
    if (abs(c1 - min1)<0.1) {
        T = g - y1;
        i = 2000;
    }
    else {
        min2=g;
        g=MIN(min2,min1,C);
        i ++;
    }
    }
    return T;
}
void MainWindow::paintEvent(QPaintEvent*)
{
    if (graph){
    int i=0;
    QPainter painter;
    int*A=new int[10000];
    double*B=new double[10000];
    double*C=new double[10000];
    painter.begin(this);
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(100,100,100,400);
    painter.drawLine(100,100,103,110);
    painter.drawLine(100,100,97,110);
    painter.drawLine(100,400,2000,400);
    painter.drawLine(2000,400,1990,397);
    painter.drawLine(2000,400,1990,403);
    int k=0;
    for(int i=0;i<10000;i++){
        A[i]=k;
        k+=1;
    }
    if(z<4){
    for(int i=0;i<2000;i++){
        pen.setWidth(1);
        painter.setPen(pen);
        B[i]=Func(A[i],z);
        C[i]=Func2(A[i],z);
        if(A[i]%q==0){
        painter.drawLine(100+A[i],400,100+A[i],Func(A[i],z));
        }
    }
    }
    if((z==4)and(needRedraw==false)){   
    i=0;
    QStringList numbersList = AAA.split(" ");
    for (const QString& number : numbersList) {
        bool ok;
        double inumber = number.toDouble(&ok);
        if (ok) {
            C[i]=inumber;
            B[i]=400-C[i]*30*mas;
            i++;
        }
    }
    }
    for(int i=0;i<2000;i++){
    if(A[i]%q==0){
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(100+A[i],400,100+A[i],B[i]);
    }
    }
    i=0;
    double max1 = 0;
    double min1=0;
    while (i < 2000) {
        if (C[i] > max1){
            if ((C[i-1]<C[i]) and (C[i]>C[i+1])) {
                max1 = C[i];
                x1 = i;
                i = 2000;
            }
        }
        i++;
    }
    i=0;
    while (i < 2000) {
        if (C[i] < min1){
            if((C[i-1]>C[i])and(C[i]<C[i+1])) {
                min1 = C[i];
                y3 = i;
                i = 2000;
            }
        }
        i++;
    }
    if(z<4){
    int x2=x1+MAX2(x1,max1,C);
    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    m=0;
    for(int l=x1;l<x2;l++){
        if(A[l]%q==0){
        painter.drawLine(100+A[l],400,100+A[l],Func(A[l],z));
            if(B[l]>m){
                m=B[l];
            }
        }
    }
    if(x1!=x2){
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(100+A[x1],400,100+A[x1],m+30);
    painter.drawLine(100+A[x2],400,100+A[x2],m+30);
    painter.drawLine(100+A[x1],m+30,100+A[x2],m+30);
    int p0=(A[x2]-A[x1]);
    QString p=QString::number(p0);
    QString g="T ="+ p;
    painter.drawText(150+(A[x2]-A[x1])/2,m+20,g);
    }
    }
    if(z==4){
    if(abs(MAX2(x1,max1,C)-MIN2(y3,min1,C)<2)and(MAX2(x1,max1,C)!=0)and(MIN2(y3,min1,C)!=0)){
    int x2=x1+MAX2(x1,max1,C);
    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    m=0;
    for(int l=x1;l<x2;l++){
    if(A[l]%q==0){
            painter.drawLine(100+A[l],400,100+A[l],B[l]);
            if(B[l]>m){
                m=B[l];
            }
    }
    }
    if(x1!=x2){
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(100+A[x1],400,100+A[x1],m+30);
    painter.drawLine(100+A[x2],400,100+A[x2],m+30);
    painter.drawLine(100+A[x1],m+30,100+A[x2],m+30);
    int p0=(A[x2]-A[x1]);
    QString p=QString::number(p0);
    QString g="T ="+ p;
    painter.drawText(150+(A[x2]-A[x1])/2,m+20,g);
    }
    }
    }
    painter.end();
    delete[] A;
    delete[] B;
    delete[] C;
    }

}
