#ifndef CHECKERFRAME_H
#define CHECKERFRAME_H

#include <QFrame>

namespace Ui {
class CheckerFrame;
}

class CheckerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CheckerFrame(QWidget *parent = nullptr);
    ~CheckerFrame();

private slots:
    void on_pushButtonCheck_clicked();

private:
    Ui::CheckerFrame *ui;
};

#endif // CHECKERFRAME_H
