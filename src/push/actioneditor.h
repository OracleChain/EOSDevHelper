#ifndef ACTIONEDITOR_H
#define ACTIONEDITOR_H

#include <QDialog>
#include <QMap>
#include <QVariant>

namespace Ui {
class ActionEditor;
}

class ActionEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ActionEditor(const QString& actionName, const QByteArray& actionFields, QWidget *parent = nullptr);
    ~ActionEditor();

signals:
    void ActionFinish(const QByteArray& ba);

private:
    void updateFieldsUI(const QByteArray& fields);
    bool isArrayType(const QString& type);

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::ActionEditor *ui;

    QString                 actionName;
    QByteArray              action;
    QMap<QString, QVariant> firstMap;
};

#endif // ACTIONEDITOR_H
