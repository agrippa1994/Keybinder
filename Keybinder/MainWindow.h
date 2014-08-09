#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SAMP/SAMP.hpp"

#include "QKeyHook.h"
#include "QLineReader.h"

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
    void onChatlog(const QString& line);

private:
    Ui::MainWindow *ui;
    SAMP::SAMP m_SAMP;
    QLineReader m_chatlogReader;
};

#endif // MAINWINDOW_H
