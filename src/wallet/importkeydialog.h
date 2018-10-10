#ifndef IMPORTKEYDIALOG_H
#define IMPORTKEYDIALOG_H

#include <QDialog>

namespace Ui {
class ImportKeyDialog;
}

class ImportKeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportKeyDialog(QWidget *parent = nullptr);
    ~ImportKeyDialog();

    void setWalletName(const QString& name);

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::ImportKeyDialog *ui;

    QString walletName;
};

#endif // IMPORTKEYDIALOG_H
