#include "aboutframe.h"
#include "ui_aboutframe.h"

#include <QDesktopServices>
#include <QUrl>

AboutFrame::AboutFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AboutFrame)
{
    ui->setupUi(this);
}

AboutFrame::~AboutFrame()
{
    delete ui;
}

void AboutFrame::on_label_2_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void AboutFrame::on_labelTwitter_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void AboutFrame::on_label_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void AboutFrame::on_label_3_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void AboutFrame::on_labelCompany_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}
