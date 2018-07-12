#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>

extern QString base_url;
extern QString url_port;

class Config
{
public:
    Config()
    {
        settings = new QSettings(iniFile(), QSettings::IniFormat);
    }

    ~Config()
    {
        if (settings) {
            delete settings;
        }
    }

    void loadSetting()
    {
        if (settings) {
            base_url = settings->value("Network/url", "").toString();
            url_port = settings->value("Network/port", "").toString();
        }
    }

    void saveSettings()
    {
        if (settings) {
            settings->setValue("Network/url", base_url);
            settings->setValue("Network/port", url_port);
        }
    }

private:
    QString iniFile()
    {
        QString curDir = QCoreApplication::applicationDirPath();
        QDir cur(curDir);
        QString strDataPath = curDir + "/data";
        QDir data(strDataPath);
        if (!data.exists()) {
            cur.mkdir("data");
        }

        return strDataPath + "/config.ini";
    }

private:
    QSettings *settings;
};

#endif // CONFIG_H
