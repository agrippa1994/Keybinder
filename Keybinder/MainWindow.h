#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SAMP/SAMP.hpp"
#include "OverlayAPI/TextOverlay.h"

#include "QKeyHook.h"
#include "QLineReader.h"

#include <QMainWindow>
#include <QTimer>

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
    void onHealthOverlayTimer();

private:
    Ui::MainWindow *ui;
    SAMP::SAMP m_SAMP;
    QLineReader m_chatlogReader;
    QTimer m_healthOverlayTimer;
    TextOverlay m_healthOverlay;

};

#endif // MAINWINDOW_H
