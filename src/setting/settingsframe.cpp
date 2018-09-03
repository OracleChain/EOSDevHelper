#include "settingsframe.h"
#include "ui_settingsframe.h"
#include "misc/config.h"

#include "codebase/utility/httpclient.h"

#include <QJsonDocument>
#include <QRegExpValidator>

extern QString base_url;
extern QString url_port;

SettingsFrame::SettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SettingsFrame),
    httpc (std::make_shared<HttpClient>(nullptr))
{
    ui->setupUi(this);
    ui->lineEditPort->setValidator(new QRegExpValidator(QRegExp("(\\d{1,5})"), this));

    ui->lineEditUrl->setText(base_url);
    ui->lineEditPort->setText(url_port);
}

SettingsFrame::~SettingsFrame()
{
    delete ui;
}

void SettingsFrame::on_pushButtonConnect_clicked()
{
    base_url = ui->lineEditUrl->text();
    url_port = ui->lineEditPort->text();

    Config cfg;
    cfg.saveSettings();

    connect(httpc.get(), &HttpClient::responseData, [=](const QByteArray& data){
        auto formatData = QJsonDocument::fromJson(data).toJson(QJsonDocument::Indented);
        ui->textEditOutput->setText(QString::fromStdString(formatData.toStdString()));
    });
    httpc->request(FunctionID::get_info);
}
