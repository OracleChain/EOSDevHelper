#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>
#include <QSettings>

extern QString base_url;
extern QString url_port;
extern bool skip_signature;

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
            base_url = settings->value("Network/url", "159.89.197.162").toString();
            url_port = settings->value("Network/port", "8888").toString();
            skip_signature = settings->value("Common/skip-signature", 1).toBool();
        }
    }

    void saveSettings()
    {
        if (settings) {
            settings->setValue("Network/url", base_url);
            settings->setValue("Network/port", url_port);
            settings->setValue("Common/skip-signature", skip_signature);
        }
    }

private:
    QString iniFile()
    {
        QDir cur = QDir::current();
        QString strDataPath = cur.absolutePath() + "/data";
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
