#ifndef KEYSUNLOCKEDDIALOG_H
#define KEYSUNLOCKEDDIALOG_H

#include <QDialog>

namespace Ui {
class KeysUnlockedDialog;
}

class KeysUnlockedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeysUnlockedDialog(QWidget *parent = 0);
    ~KeysUnlockedDialog();

    void showUnlockedKeys();

private slots:
    void on_pushButton_clicked();

private:
    Ui::KeysUnlockedDialog *ui;
};

#endif // KEYSUNLOCKEDDIALOG_H
