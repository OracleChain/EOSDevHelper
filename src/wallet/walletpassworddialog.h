#ifndef WALLETPASSWORDDIALOG_H
#define WALLETPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class WalletPasswordDialog;
}

class WalletPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WalletPasswordDialog(QWidget *parent = nullptr);
    ~WalletPasswordDialog();

    void setWalletName(const QString& name);
    void showPassword(const QString& passwd);

private slots:
    void on_pushButtonOk_clicked();

private:
    Ui::WalletPasswordDialog *ui;

    QString walletName;
    QString password;
};

#endif // WALLETPASSWORDDIALOG_H
