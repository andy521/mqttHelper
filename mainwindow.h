#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mqtt/qmqtt.h"
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString  username,password;//默认是null
    QHostAddress *host;//默认是localhost
    qint16 port;//默认是1883
    QMQTT::Client * client;
    ~MainWindow();
public slots:
    void log(QString str);
    void connected();
    void disconnected();
    void error(const QMQTT::ClientError error);

    void subscribed(const QString& topic, const quint8 qos = 0);
    void unsubscribed(const QString& topic);
    void published(const QMQTT::Message& message, quint16 msgid = 0);
    void received(const QMQTT::Message& message);
    void pingresp();


    void connect();
    void disconnect();
    void subscribe();
    void unsubscribe();
    void publish();



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H


