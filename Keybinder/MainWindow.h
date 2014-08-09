#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SAMP/SAMP.hpp"

#include "QKeyHook.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onGlobalKeyPressed(KBDLLHOOKSTRUCT *key, bool& block);

private:
    Ui::MainWindow *ui;
    SAMP::SAMP m_SAMP;
};

#endif // MAINWINDOW_H
