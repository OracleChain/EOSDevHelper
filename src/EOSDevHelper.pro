#-------------------------------------------------
#
# Project created by QtCreator 2018-04-03T17:44:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EOSDevHelper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    wallet/walletframe.cpp \
    push/pushframe.cpp \
    table/tableframe.cpp \
    account/accountframe.cpp \
    output/outputframe.cpp \
    Crypto/base58.c \
    Crypto/rmd160.c \
    Crypto/sha2.c \
    Crypto/sha3.c \
    Crypto/uECC.c \
    utility/httpclient.cpp \
    ec/eos_key_encode.cpp \
    ec/typechainid.cpp \
    checker/checkerframe.cpp \
    wallet/walletpassworddialog.cpp \
    wallet/eoswallet.cpp \
    wallet/eoswalletmanager.cpp \
    wallet/walletitemframe.cpp \
    wallet/importkeydialog.cpp \
    wallet/keysunlockeddialog.cpp \
    utility/packer.cpp \
    wallet/unlockwalletdialog.cpp \
    Crypto/aes.c \
    chain/eosbytewriter.cpp \
    chain/typename.cpp \
    chain/action.cpp \
    chain/transactionheader.cpp \
    chain/transaction.cpp \
    chain/signedtransaction.cpp \
    chain/eosnewaccount.cpp \
    chain/typeauthority.cpp \
    chain/typeaccountpermissionweight.cpp \
    chain/typepermissionlevel.cpp \
    chain/typekeypermissionweight.cpp \
    misc/customtabstyle.cpp \
    account/createaccountframe.cpp \
    setting/settingsframe.cpp \
    misc/aboutframe.cpp \
    chain/chainmanager.cpp \
    chain/packedtransaction.cpp \
    utility/utils.cpp \
    ec/sha512.cpp

HEADERS  += mainwindow.h \
    wallet/walletframe.h \
    push/pushframe.h \
    table/tableframe.h \
    account/accountframe.h \
    output/outputframe.h \
    Crypto/libbase58.h \
    Crypto/macros.h \
    Crypto/options.h \
    Crypto/rmd160.h \
    Crypto/sha2.h \
    Crypto/sha3.h \
    Crypto/uECC.h \
    utility/httpclient.h \
    ec/eos_key_encode.h \
    checker/checkerframe.h \
    wallet/walletpassworddialog.h \
    wallet/eoswallet.h \
    wallet/eoswalletmanager.h \
    wallet/walletitemframe.h \
    wallet/importkeydialog.h \
    wallet/keysunlockeddialog.h \
    misc/config.h \
    utility/packer.h \
    wallet/unlockwalletdialog.h \
    Crypto/aes.h \
    Crypto/aes.hpp \
    chain/chainbase.h \
    chain/eosbytewriter.h \
    ec/typechainid.h \
    chain/typename.h \
    chain/action.h \
    chain/transactionheader.h \
    chain/transaction.h \
    chain/signedtransaction.h \
    chain/eosnewaccount.h \
    chain/typeauthority.h \
    chain/typeaccountpermissionweight.h \
    chain/typepermissionlevel.h \
    chain/typekeypermissionweight.h \
    misc/customtabstyle.h \
    account/createaccountframe.h \
    setting/settingsframe.h \
    misc/aboutframe.h \
    chain/chainmanager.h \
    chain/packedtransaction.h \
    utility/utils.h \
    ec/sha512.h

FORMS    += mainwindow.ui \
    wallet/walletframe.ui \
    push/pushframe.ui \
    table/tableframe.ui \
    account/accountframe.ui \
    output/outputframe.ui \
    checker/checkerframe.ui \
    wallet/walletpassworddialog.ui \
    wallet/walletitemframe.ui \
    wallet/importkeydialog.ui \
    wallet/keysunlockeddialog.ui \
    wallet/unlockwalletdialog.ui \
    account/createaccountframe.ui \
    setting/settingsframe.ui \
    misc/aboutframe.ui

INCLUDEPATH += Crypto \
    utility

DISTFILES +=

RESOURCES += \
    resource.qrc
