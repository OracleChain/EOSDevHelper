#include "actioneditor.h"
#include "ui_actioneditor.h"

#include "codebase/chain/abiparser.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
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
    //secondVec.clear();

    ABIParser parser;
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array.at(i).toObject();
        if (obj.isEmpty()) {
            continue;
        }

        QString name = obj.value("name").toString();
        QString type = obj.value("type").toString();

        QObject *object = nullptr;

        std::string t = type.toStdString();
        if (parser.is_array(t)) {
            object = new QVBoxLayout(this);
            QComboBox *combo = new QComboBox(nullptr);
            int i = 1;
            QStringList numList;
            while(i <= 30) {
                numList.append(QString::number(i));
                ++i;
            }
            combo->addItems(numList);
            dynamic_cast<QVBoxLayout*>(object)->addWidget(combo);
            connect(combo, QComboBox::currentTextChanged, [&](const QString& str){
                //combo->hide();
                QList<QVariant> secondList;
                int num = str.toInt();
                for (int j = 0; j < num; ++j) {
                    QLineEdit *lineEdit = new QLineEdit;
                    secondList.append(QVariant((int)lineEdit));
                    dynamic_cast<QVBoxLayout*>(object)->addWidget(lineEdit);
                }

                firstMap.insert(name, secondList);
            });

        } else {
            object = new QLineEdit(this);
            dynamic_cast<QLineEdit*>(object)->setStyleSheet(lineEditStyle);
            firstMap.insert(name, int(object));
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

void ActionEditor::on_pushButtonOk_clicked()
{

    close();
}

void ActionEditor::on_pushButtonCancel_clicked()
{
    close();
}
