#ifndef SETTINGSFRAME_H
#define SETTINGSFRAME_H

#include <QFrame>
#include <memory>

namespace Ui {
class SettingsFrame;
}

class HttpClient;

class SettingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SettingsFrame(QWidget *parent = nullptr);
    ~SettingsFrame();

private slots:
    void on_pushButtonConnect_clicked();

private:
    Ui::SettingsFrame           *ui;
    std::shared_ptr<HttpClient> httpc;
};

#endif // SETTINGSFRAME_H
