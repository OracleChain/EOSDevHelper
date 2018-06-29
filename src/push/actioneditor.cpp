#include "actioneditor.h"
#include "ui_actioneditor.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

const QString lineEditStyle = "QLineEdit {"
                        "background-color: rgb(255, 255, 255);"
        "font: 14pt \"Arial\";"
        "border-color: rgb(0, 0, 0);"
        "border-width: 2px;"
        "color: rgb(0, 0, 0);"
        "}";

const QString labelStyle = "QLabel {"
                     "background-color: rgb(255, 255, 255);"
        "font: 14pt \"Arial\";"
        "color: rgb(0, 0, 0);"
        "}";

ActionEditor::ActionEditor(const QString &actionName, const QByteArray &actionFields, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActionEditor)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    this->setWindowTitle("Editing action " + actionName);

    updateFieldsUI(actionFields);
}

ActionEditor::~ActionEditor()
{
    delete ui;
}

void ActionEditor::updateFieldsUI(const QByteArray &fields)
{
    QJsonArray array = QJsonDocument::fromJson(fields).array();
    if (array.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "empty fields in action struct.");
        close();
    }

    firstMap.clear();

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array.at(i).toObject();
        if (obj.isEmpty()) {
            continue;
        }

        QString name = obj.value("name").toString();
        QString type = obj.value("type").toString();

        QObject *object = nullptr;

        if (isArrayType(type)) {
            object = new QVBoxLayout(nullptr);
            QList<QVariant> secondList;
            for (int j = 0; j < 10; ++j) {
                QLineEdit *lineEdit = new QLineEdit;
                secondList.append(QVariant(reinterpret_cast<int>(lineEdit)));
                dynamic_cast<QVBoxLayout*>(object)->addWidget(lineEdit);
            }

            firstMap.insert(name, secondList);
        } else {
            object = new QLineEdit(this);
            dynamic_cast<QLineEdit*>(object)->setStyleSheet(lineEditStyle);
            firstMap.insert(name, reinterpret_cast<int>(object));
        }

        if (!object) {
            continue;
        }

        QLabel *label = new QLabel(this);
        label->setStyleSheet(labelStyle);
        label->setText(name+" ("+type+"):");

        QHBoxLayout *layout = new QHBoxLayout(nullptr);
        layout->addWidget(label);
        if (QLineEdit* line = dynamic_cast<QLineEdit*>(object)) {
            layout->addWidget(line);
        } else {
            layout->addLayout(dynamic_cast<QVBoxLayout*>(object));
        }
        ui->verticalLayout->addLayout(layout);
    }
}

bool ActionEditor::isArrayType(const QString &type)
{
    return type.endsWith("[]");
}

void ActionEditor::on_pushButtonOk_clicked()
{
    QJsonObject obj;
    for (auto itr = firstMap.cbegin(); itr != firstMap.end(); ++itr) {
        bool ok = false;
        int in = itr.value().toInt(&ok);
        if (ok) {
            QLineEdit *lineEdit = reinterpret_cast<QLineEdit*>(in);
            if (lineEdit) {
                obj.insert(itr.key(), QJsonValue(lineEdit->text()));
            }
        } else {
            QList<QVariant> list = itr.value().toList();
            if (!list.isEmpty()) {
                QJsonArray array;
                for (int i = 0; i < list.size(); ++i) {
                    bool ok2 = false;
                    int in2 = list.at(i).toInt(&ok2);
                    if (ok2) {
                        QLineEdit *lineEdit = reinterpret_cast<QLineEdit*>(in2);
                        if (lineEdit) {
                            QString text = lineEdit->text();
                            if (text.isEmpty()) continue;
                            array.append(QJsonValue(text));
                        }
                    }
                }
                obj.insert(itr.key(), array);
            }
        }
    }

    emit ActionFinish(QJsonDocument(obj).toJson());
    close();
}

void ActionEditor::on_pushButtonCancel_clicked()
{
    close();
}
