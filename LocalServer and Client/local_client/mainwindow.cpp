#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ProcessGCServer = new CTLProcessCommunicationClient(this);
    m_ProcessGCServer->setDiscribe("local-client");
    m_ProcessGCServer->bind("test-local");
    LocalMessage msg;
    msg.data = "i am client";
    msg.head.type = 0;
    msg.head.size = CTLProcessHeadSize + msg.data.size();
//    m_ProcessGCServer->sendMessage(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
}
