#ifndef WALLETITEMFRAME_H
#define WALLETITEMFRAME_H

#include <QFrame>

namespace Ui {
class WalletItemFrame;
}

class WalletItemFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WalletItemFrame(QWidget *parent = nullptr);
    ~WalletItemFrame();

    void setWalletName(const QString& name);
    void setLockState(bool locked);

private slots:
    void on_pushButtonImportKey_clicked();

    void on_pushButtonLock_clicked();

private:
    Ui::WalletItemFrame *ui;

    bool locked;
};

#endif // WALLETITEMFRAME_H
