#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

#include "wallet/walletframe.h"
#include "account/accountframe.h"
#include "push/pushframe.h"
#include "table/tableframe.h"
#include "output/outputframe.h"
#include "checker/checkerframe.h"
#include "misc/customtabstyle.h"
#include "setting/settingsframe.h"
#include "misc/aboutframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->tabWidget->tabBar()->setStyle(std::move(new CustomTabStyle));

    InitUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUI()
{
    QHBoxLayout *walletHLayout = new QHBoxLayout(ui->tabWallet);
    walletHLayout->addWidget(std::move(new WalletFrame(ui->tabWallet)));

    acntFrame = new AccountFrame(ui->tabAccount);
    QVBoxLayout *accountVLayout = new QVBoxLayout(ui->tabAccount);
    accountVLayout->addWidget(acntFrame);

    pushOutput = new OutputFrame(ui->tabPush);

    QHBoxLayout *pushHLayout = new QHBoxLayout(ui->tabPush);
    pushHLayout->addWidget(std::move(new PushFrame(ui->tabPush)));
    pushHLayout->addWidget(pushOutput);
    pushHLayout->setStretch(0, 1);
    pushHLayout->setStretch(1, 4);

    QHBoxLayout *tableHLayout = new QHBoxLayout(ui->tabGetTable);
    tableHLayout->addWidget(std::move(new TableFrame(ui->tabGetTable)));

    QHBoxLayout *checkerLayout = new QHBoxLayout(ui->tabChecker);
    checkerLayout->addWidget(std::move(new CheckerFrame(ui->tabChecker)));

    QHBoxLayout *settingLayout = new QHBoxLayout(ui->tabSettings);
    settingLayout->addWidget(std::move(new SettingsFrame(ui->tabSettings)));

    AboutFrame *about = new AboutFrame(this);
    about->move(this->rect().bottomLeft());
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if (index == 1) {
        if (acntFrame) {
            acntFrame->refreshCreateAccountFrame();
        }
    }
}
