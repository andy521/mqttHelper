#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextCodec>
#include <QByteArray>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client=new QMQTT::Client;
    ui->outputConsole->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QObject::connect(ui->connectButton,SIGNAL(clicked(bool)),this,SLOT(connect()));
    QObject::connect(ui->disconnectButton,SIGNAL(clicked(bool)),this,SLOT(disconnect()));
    QObject::connect(ui->subButton,SIGNAL(clicked(bool)),this,SLOT(subscribe()));
    QObject::connect(ui->cancelSubButton,SIGNAL(clicked(bool)),this,SLOT(unsubscribe()));
    QObject::connect(client,SIGNAL(error(QMQTT::ClientError)),this,SLOT(error(QMQTT::ClientError)));
    QObject::connect(client,SIGNAL(connected()),this,SLOT(connected()));
    QObject::connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::onError
 * @param error
 *
    SocketProxyAuthenticationRequiredError,
    SocketSslHandshakeFailedError,
    SocketProxyConnectionRefusedError,
    SocketProxyConnectionClosedError,
    SocketProxyConnectionTimeoutError,
    SocketProxyNotFoundError,
    SocketProxyProtocolError,
    SocketOperationError,
    SocketSslInternalError,
    SocketSslInvalidUserDataError,
    SocketTemporaryError
 */
void MainWindow::error(QMQTT::ClientError error){
    qDebug()<<error;
    switch (error) {

    case 0: log(QString::fromLocal8Bit("连接失败!错误码:UnknownError 0"));
        break;
    case 1: log(QString::fromLocal8Bit("连接失败!错误码:SocketConnectionRefusedError 1"));
        break;

    case 2: log(QString::fromLocal8Bit("连接失败!错误码:SocketRemoteHostClosedError 2"));

        break;

    case 3: log(QString::fromLocal8Bit("连接失败!错误码:SocketRemoteHostClosedError 3"));

        break;
    case 4: log(QString::fromLocal8Bit("连接失败!错误码:SocketAccessError 4"));
        break;

    case 5: log(QString::fromLocal8Bit("连接失败!错误码:SocketResourceError 5"));
        break;

    case 6: log(QString::fromLocal8Bit("连接失败!错误码:SocketTimeoutError 6"));
        break;

    case 7: log(QString::fromLocal8Bit("连接失败!错误码:SocketDatagramTooLargeError 7"));
        break;

    case 8: log(QString::fromLocal8Bit("连接失败!错误码:SocketNetworkError 8"));
        break;

    case 9: log(QString::fromLocal8Bit("连接失败!错误码:SocketAddressInUseError 9"));
        break;

    case 10: log(QString::fromLocal8Bit("连接失败!错误码:SocketAddressNotAvailableError 10"));
        break;

    case 11: log(QString::fromLocal8Bit("连接失败!错误码:SocketUnsupportedSocketOperationError 11"));
        break;

    default: log(QString::fromLocal8Bit("连接失败!错误码:SocketConnectionRefusedError 12"));
        break;
    }
}
void MainWindow:: log(QString str)
{
    ui->outputConsole->append("log>>"+ str);
}
void MainWindow::connected(){
    log(QString::fromLocal8Bit("连接完成!"));
}

void MainWindow:: disconnected(){
    log(QString::fromLocal8Bit("设备离线!"));
}

void MainWindow:: subscribed(const QString& topic, const quint8 qos){
    log(QString::fromLocal8Bit("Topic: %s 订阅成功!").arg(topic));
}
void MainWindow:: unsubscribed(const QString& topic){
    log(QString::fromLocal8Bit("Topic: %s  退订成功!").arg(topic));
}
void MainWindow:: published(const QMQTT::Message& message, quint16 msgid ){
    log(QString::fromLocal8Bit("发布成功! 消息内容:%s" ).arg(QString(message.payload())));
}
void MainWindow:: received(const QMQTT::Message& message){
    log(QString::fromLocal8Bit("收到消息:"+message.payload()));

}
void MainWindow:: pingresp(){

}
void MainWindow::disconnect(){
    log(QString::fromLocal8Bit("离线成功!"));
    client->disconnect();
}

void MainWindow:: subscribe(){
    client->subscribe(ui->topicEdit->text());
    log(QString::fromLocal8Bit("订阅成功!"));

}
void MainWindow:: unsubscribe(){
    log(QString::fromLocal8Bit("退订成功!"));
    client->unsubscribe(ui->topicEdit->text());

}

void MainWindow:: publish(){

    QMQTT::Message *message = new QMQTT::Message();
    message->setTopic(ui->topicEdit->text());
    message->setPayload(QByteArray(ui->pubContentEdit->text().toLatin1()) );
    client->publish(*message);
    log(QString::fromLocal8Bit("发布成功!"));
}
void MainWindow::connect(){

    host=new QHostAddress(ui->hostAddressEdit->text());
    username=ui->usernameEdit->text();
    password=ui->passwordEdit->text();
    port=ui->portEdit->text().toInt();
    client->setPort(port);
    client->setUsername(username);
    client->setPassword(password.toLatin1());
    client->setClientId(ui->clientIdEdit->text());
    if(client->isConnectedToHost()){
        log(QString::fromLocal8Bit("已经连接成功!"));
    }else {
        client->connectToHost();
    }


}


