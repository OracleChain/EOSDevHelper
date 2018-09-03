#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTabBar>

#include "wallet/walletframe.h"
#include "account/accountframe.h"
#include "push/pushframe.h"
#include "table/tableframe.h"
#include "output/outputframe.h"
#include "checker/checkerframe.h"
#include "misc/customtabstyle.h"
#include "setting/settingsframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    updateAbout();
}

void MainWindow::InitUI()
{
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->tabWidget->tabBar()->setStyle(std::move(new CustomTabStyle));

    auto walletHLayout = new QHBoxLayout(ui->tabWallet);
    walletHLayout->addWidget(std::move(new WalletFrame(ui->tabWallet)));

    acntFrame = new AccountFrame(ui->tabAccount);
    auto accountVLayout = new QVBoxLayout(ui->tabAccount);
    accountVLayout->addWidget(acntFrame);

    pushOutput = new OutputFrame(ui->tabPush);

    auto pushHLayout = new QHBoxLayout(ui->tabPush);
    pushHLayout->addWidget(std::move(new PushFrame(ui->tabPush)));
    pushHLayout->addWidget(pushOutput);
    pushHLayout->setStretch(0, 1);
    pushHLayout->setStretch(1, 4);

    auto tableHLayout = new QHBoxLayout(ui->tabGetTable);
    tableHLayout->addWidget(std::move(new TableFrame(ui->tabGetTable)));

    auto checkerLayout = new QHBoxLayout(ui->tabChecker);
    checkerLayout->addWidget(std::move(new CheckerFrame(ui->tabChecker)));

    auto settingLayout = new QHBoxLayout(ui->tabSettings);
    settingLayout->addWidget(std::move(new SettingsFrame(ui->tabSettings)));

    about = new AboutFrame(this);
    updateAbout();
}

void MainWindow::updateAbout()
{
    if (about) {
        QPoint p(this->rect().bottomLeft().x(), this->rect().bottomLeft().y());
        auto rect = about->geometry();
        p.setY(p.y() - rect.height());
        about->move(p);
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if (index == 1) {
        if (acntFrame) {
            acntFrame->refreshCreateAccountFrame();
        }
    }
}
