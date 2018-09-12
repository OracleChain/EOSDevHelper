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
    checker/checkerframe.cpp \
    wallet/walletpassworddialog.cpp \
    wallet/eoswallet.cpp \
    wallet/eoswalletmanager.cpp \
    wallet/walletitemframe.cpp \
    wallet/importkeydialog.cpp \
    wallet/keysunlockeddialog.cpp \
    wallet/unlockwalletdialog.cpp \
    misc/customtabstyle.cpp \
    account/createaccountframe.cpp \
    setting/settingsframe.cpp \
    misc/aboutframe.cpp \
    wallet/packer.cpp \
    codebase/chain/action.cpp \
    codebase/chain/chainmanager.cpp \
    codebase/chain/eosbytewriter.cpp \
    codebase/chain/eosnewaccount.cpp \
    codebase/chain/packedtransaction.cpp \
    codebase/chain/signedtransaction.cpp \
    codebase/chain/transaction.cpp \
    codebase/chain/transactionheader.cpp \
    codebase/chain/typeaccountpermissionweight.cpp \
    codebase/chain/typeauthority.cpp \
    codebase/chain/typekeypermissionweight.cpp \
    codebase/chain/typename.cpp \
    codebase/chain/typepermissionlevel.cpp \
    codebase/Crypto/aes.c \
    codebase/Crypto/base58.c \
    codebase/Crypto/rmd160.c \
    codebase/Crypto/sha2.c \
    codebase/Crypto/sha3.c \
    codebase/Crypto/uECC.c \
    codebase/ec/eos_key_encode.cpp \
    codebase/ec/sha512.cpp \
    codebase/ec/typechainid.cpp \
    codebase/utility/httpclient.cpp \
    codebase/utility/utils.cpp \
    codebase/chain/transactionextension.cpp \
    codebase/chain/typewaitweight.cpp \
    codebase/ec/sha256.cpp \
    push/actioneditor.cpp

HEADERS  += mainwindow.h \
    wallet/walletframe.h \
    push/pushframe.h \
    table/tableframe.h \
    account/accountframe.h \
    output/outputframe.h \
    checker/checkerframe.h \
    wallet/walletpassworddialog.h \
    wallet/eoswallet.h \
    wallet/eoswalletmanager.h \
    wallet/walletitemframe.h \
    wallet/importkeydialog.h \
    wallet/keysunlockeddialog.h \
    misc/config.h \
    wallet/unlockwalletdialog.h \
    misc/customtabstyle.h \
    account/createaccountframe.h \
    setting/settingsframe.h \
    misc/aboutframe.h \
    wallet/packer.h \
    codebase/chain/action.h \
    codebase/chain/chainbase.h \
    codebase/chain/chainmanager.h \
    codebase/chain/eosbytewriter.h \
    codebase/chain/eosnewaccount.h \
    codebase/chain/packedtransaction.h \
    codebase/chain/signedtransaction.h \
    codebase/chain/transaction.h \
    codebase/chain/transactionheader.h \
    codebase/chain/typeaccountpermissionweight.h \
    codebase/chain/typeauthority.h \
    codebase/chain/typekeypermissionweight.h \
    codebase/chain/typename.h \
    codebase/chain/typepermissionlevel.h \
    codebase/Crypto/aes.h \
    codebase/Crypto/aes.hpp \
    codebase/Crypto/libbase58.h \
    codebase/Crypto/macros.h \
    codebase/Crypto/options.h \
    codebase/Crypto/rmd160.h \
    codebase/Crypto/sha2.h \
    codebase/Crypto/sha3.h \
    codebase/Crypto/uECC.h \
    codebase/ec/eos_key_encode.h \
    codebase/ec/sha512.h \
    codebase/ec/typechainid.h \
    codebase/utility/httpclient.h \
    codebase/utility/utils.h \
    codebase/chain/transactionextension.h \
    codebase/chain/typewaitweight.h \
    codebase/ec/sha256.h \
    push/actioneditor.h

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
    misc/aboutframe.ui \
    push/actioneditor.ui

DISTFILES +=

RESOURCES += \
    resource.qrc

CONFIG += c++11

RC_ICONS = resource/logo.ico

ICON = resource/logo.icns
