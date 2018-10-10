#ifndef OUTPUTFRAME_H
#define OUTPUTFRAME_H

#include <QFrame>

namespace Ui {
class OutputFrame;
}

class OutputFrame : public QFrame
{
    Q_OBJECT

public:
    explicit OutputFrame(QWidget *parent = nullptr);
    ~OutputFrame();

    void setRequestOutput(int index, const QString& title, const QByteArray& data);
    void setResponseOutput(int index, const QByteArray& data);
    void clearOutput();

private:
    Ui::OutputFrame *ui;
};

#endif // OUTPUTFRAME_H
