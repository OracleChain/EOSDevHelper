#ifndef WALLETFRAME_H
#define WALLETFRAME_H

#include <QFrame>

namespace Ui {
class WalletFrame;
}

class WalletFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WalletFrame(QWidget *parent = nullptr);
    ~WalletFrame();

private:
    void addWalletToUIList(const QString& name);
    void initListUI();

private slots:
    void on_pushButtonCreateAccount_clicked();

    void on_pushButtonViewUnlockedKeys_clicked();

private:
    Ui::WalletFrame *ui;
};

#endif // WALLETFRAME_H
