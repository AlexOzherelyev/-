#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
     MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent*)override;
private slots:
    void on_comboBox_activated(int index);
    void on_pushButton_clicked();
    void on_comboBox_2_activated(int index);
    void on_comboBox_3_activated(int index);
    void on_action_triggered();
    void on_action_2_triggered();
    void on_pushButton_2_clicked();
private:
    Ui::MainWindow *ui;
    int q=2;
    int z;
    bool needRedraw = true;
    bool graph=false;
    int x1;
    int y3;
    QString AAA;
};
#endif // MAINWINDOW_H
