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
    auto array = QJsonDocument::fromJson(fields).array();
    if (array.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "empty fields in action struct.");
        close();
    }

    firstMap.clear();

    for (auto i = 0; i < array.size(); ++i) {
        auto obj = array.at(i).toObject();
        if (obj.isEmpty()) {
            continue;
        }

        auto name = obj.value("name").toString();
        auto type = obj.value("type").toString();

        QObject *object = nullptr;  // this eventually add to ui, so just use QT way: new, add to ui and leave it alone.

        if (isArrayType(type)) {
            object = new QVBoxLayout(nullptr);
            QList<QVariant> secondList;
            for (int j = 0; j < 10; ++j) {
                auto lineEdit = new QLineEdit;
                secondList.append(QVariant(reinterpret_cast<qintptr>(lineEdit)));
                dynamic_cast<QVBoxLayout*>(object)->addWidget(lineEdit);
            }

            firstMap.insert(name, secondList);
        } else {
            object = new QLineEdit(this);
            dynamic_cast<QLineEdit*>(object)->setStyleSheet(lineEditStyle);
            firstMap.insert(name, reinterpret_cast<qintptr>(object));
        }

        if (!object) {
            continue;
        }

        auto label = new QLabel(this);
        label->setStyleSheet(labelStyle);
        label->setText(name+" ("+type+"):");

        auto layout = new QHBoxLayout(nullptr);
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
        auto ok = false;
        auto in = itr.value().toLongLong(&ok);  // clang make qint64 to qint32, don't know why.
        if (ok) {
            if (QLineEdit *lineEdit = reinterpret_cast<QLineEdit*>(in)) {
                obj.insert(itr.key(), QJsonValue(lineEdit->text()));
            }
        } else {
            auto list = itr.value().toList();
            if (!list.isEmpty()) {
                QJsonArray array;
                for (auto i = 0; i < list.size(); ++i) {
                    auto ok2 = false;
                    auto in2 = list.at(i).toLongLong(&ok2);
                    if (ok2) {
                        if (QLineEdit *lineEdit = reinterpret_cast<QLineEdit*>(in2)) {
                            auto text = lineEdit->text();
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
