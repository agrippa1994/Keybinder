#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SAMP/SAMP.hpp"
#include "OverlayAPI/TextOverlay.h"
#include "OverlayAPI/BoxOverlay.h"

#include "QKeyHook.h"
#include "QLineReader.h"
#include "InfoDialog.h"

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
    bool addChatMessage(QString str);
    QString convertToHTMLColorCodes(const QString& text);

private slots:
    void onGlobalKeyPressed(KBDLLHOOKSTRUCT *key, bool& block);
    void onChatlog(const QString& line);
    void onHealthOverlayTimer();
    void onStatsOverlayTimer();

private:
    Ui::MainWindow *ui;

    InfoDialog m_infoDialog;

    SAMP::SAMP m_SAMP;

    QLineReader m_chatlogReader;

    QTimer m_healthOverlayTimer;
    QTimer m_statsOverlayTimer;

    TextOverlay m_healthOverlay;
    TextOverlay m_statsOverlay;
    BoxOverlay m_statsBoxOverlay;

    bool m_updateStats = true;

    QTimer m_lastSpamWarning;
};

#endif // MAINWINDOW_H
