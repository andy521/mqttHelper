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

    case 0: log(QString::fromLocal8Bit("����ʧ��!������:UnknownError 0"));
        break;
    case 1: log(QString::fromLocal8Bit("����ʧ��!������:SocketConnectionRefusedError 1"));
        break;

    case 2: log(QString::fromLocal8Bit("����ʧ��!������:SocketRemoteHostClosedError 2"));

        break;

    case 3: log(QString::fromLocal8Bit("����ʧ��!������:SocketRemoteHostClosedError 3"));

        break;
    case 4: log(QString::fromLocal8Bit("����ʧ��!������:SocketAccessError 4"));
        break;

    case 5: log(QString::fromLocal8Bit("����ʧ��!������:SocketResourceError 5"));
        break;

    case 6: log(QString::fromLocal8Bit("����ʧ��!������:SocketTimeoutError 6"));
        break;

    case 7: log(QString::fromLocal8Bit("����ʧ��!������:SocketDatagramTooLargeError 7"));
        break;

    case 8: log(QString::fromLocal8Bit("����ʧ��!������:SocketNetworkError 8"));
        break;

    case 9: log(QString::fromLocal8Bit("����ʧ��!������:SocketAddressInUseError 9"));
        break;

    case 10: log(QString::fromLocal8Bit("����ʧ��!������:SocketAddressNotAvailableError 10"));
        break;

    case 11: log(QString::fromLocal8Bit("����ʧ��!������:SocketUnsupportedSocketOperationError 11"));
        break;

    default: log(QString::fromLocal8Bit("����ʧ��!������:SocketConnectionRefusedError 12"));
        break;
    }
}
void MainWindow:: log(QString str)
{
    ui->outputConsole->append("log>>"+ str);
}
void MainWindow::connected(){
    log(QString::fromLocal8Bit("�������!"));
}

void MainWindow:: disconnected(){
    log(QString::fromLocal8Bit("�豸����!"));
}

void MainWindow:: subscribed(const QString& topic, const quint8 qos){
    log(QString::fromLocal8Bit("Topic: %s ���ĳɹ�!").arg(topic));
}
void MainWindow:: unsubscribed(const QString& topic){
    log(QString::fromLocal8Bit("Topic: %s  �˶��ɹ�!").arg(topic));
}
void MainWindow:: published(const QMQTT::Message& message, quint16 msgid ){
    log(QString::fromLocal8Bit("�����ɹ�! ��Ϣ����:%s" ).arg(QString(message.payload())));
}
void MainWindow:: received(const QMQTT::Message& message){
    log(QString::fromLocal8Bit("�յ���Ϣ:"+message.payload()));

}
void MainWindow:: pingresp(){

}
void MainWindow::disconnect(){
    log(QString::fromLocal8Bit("���߳ɹ�!"));
    client->disconnect();
}

void MainWindow:: subscribe(){
    client->subscribe(ui->topicEdit->text());
    log(QString::fromLocal8Bit("���ĳɹ�!"));

}
void MainWindow:: unsubscribe(){
    log(QString::fromLocal8Bit("�˶��ɹ�!"));
    client->unsubscribe(ui->topicEdit->text());

}

void MainWindow:: publish(){

    QMQTT::Message *message = new QMQTT::Message();
    message->setTopic(ui->topicEdit->text());
    message->setPayload(QByteArray(ui->pubContentEdit->text().toLatin1()) );
    client->publish(*message);
    log(QString::fromLocal8Bit("�����ɹ�!"));
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
        log(QString::fromLocal8Bit("�Ѿ����ӳɹ�!"));
    }else {
        client->connectToHost();
    }


}


