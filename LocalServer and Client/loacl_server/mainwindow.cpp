#include "mainwindow.h"
#include "ui_mainwindow.h"
int divide(int a, int b)
{
    Q_ASSERT_X(b != 0, "divide-local", "division by zero");
    return a / b;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    divide(4,1);
    m_ProcessGCServer = new CTLProcessCommunicationServer(this);
    m_ProcessGCServer->setDiscribe("local-server");
    m_ProcessGCServer->bind("test-local");
//    m_ProcessGCServer->setTarget("cloudboard-GC");
    connect(m_ProcessGCServer,&CTLProcessCommunicationServer::sigClientComing,
            this,&MainWindow::slotGCConnected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotGCConnected(QString name)
{
    qDebug()<<__FUNCTION__<<name;

}

