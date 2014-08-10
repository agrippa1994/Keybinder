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

    bool isSpamWarningActive() const;

private slots:
    void onGlobalKeyPressed(KBDLLHOOKSTRUCT *key, bool& block);
    void onChatlog(const QString& line);
    void onHealthOverlayTimer();
    void onStatsOverlayTimer();

private:
    Ui::MainWindow *ui;

    SAMP::SAMP m_SAMP;

    QLineReader m_chatlogReader;

    QTimer m_healthOverlayTimer;
    QTimer m_statsOverlayTimer;

    TextOverlay m_healthOverlay;
    TextOverlay m_statsOverlay;

    bool m_updateStats = true;

    QTimer m_lastSpamWarning;
};

#endif // MAINWINDOW_H
