#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "local/ctlprocesscommunicationserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void slotGCConnected(QString name);
private:
    Ui::MainWindow *ui;
    CTLProcessCommunicationServer *m_ProcessGCServer = Q_NULLPTR;
};

#endif // MAINWINDOW_H
