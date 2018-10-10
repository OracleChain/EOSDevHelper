#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "output/outputframe.h"
#include "account/accountframe.h"
#include "misc/aboutframe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    AccountFrame *accountFrame() { return acntFrame; }
    OutputFrame *pushOutputFrame() { return pushOutput; }

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_tabWidget_tabBarClicked(int index);

private:
    void InitUI();
    void updateAbout();

private:
    Ui::MainWindow *ui;

    AccountFrame *acntFrame;
    OutputFrame  *pushOutput;
    AboutFrame   *about;
};

#endif // MAINWINDOW_H
