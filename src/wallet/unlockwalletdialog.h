#ifndef UNLOCKWALLETDIALOG_H
#define UNLOCKWALLETDIALOG_H

#include <QDialog>

namespace Ui {
class UnlockWalletDialog;
}

class UnlockWalletDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnlockWalletDialog(QWidget *parent = nullptr);
    ~UnlockWalletDialog();

    void setWalletName(const QString& name);

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::UnlockWalletDialog *ui;

    QString name;
};

#endif // UNLOCKWALLETDIALOG_H
