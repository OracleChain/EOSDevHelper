#ifndef ABOUTFRAME_H
#define ABOUTFRAME_H

#include <QFrame>

namespace Ui {
class AboutFrame;
}

class AboutFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AboutFrame(QWidget *parent = 0);
    ~AboutFrame();

private slots:
    void on_label_2_linkActivated(const QString &link);

    void on_labelTwitter_linkActivated(const QString &link);

    void on_label_linkActivated(const QString &link);

    void on_label_3_linkActivated(const QString &link);

    void on_labelCompany_linkActivated(const QString &link);

private:
    Ui::AboutFrame *ui;
};

#endif // ABOUTFRAME_H
