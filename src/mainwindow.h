#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "output/outputframe.h"
#include "account/accountframe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    AccountFrame *accountFrame() { return acntFrame; }
    OutputFrame *pushOutputFrame() { return pushOutput; }

private slots:
    void on_tabWidget_tabBarClicked(int index);

private:
    void InitUI();

private:
    Ui::MainWindow *ui;

    AccountFrame *acntFrame;
    OutputFrame *pushOutput;
};

#endif // MAINWINDOW_H
