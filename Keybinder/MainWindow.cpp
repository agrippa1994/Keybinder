#include "StatsAPI/StatsAPI.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QStandardPaths>
#include <QRegularExpressionMatch>
#include <string>

#define SEND(VK, text) case VK: if(!m_SAMP.isInForeground()) break; if(m_SAMP.isInChat()) break; block = true; m_SAMP.sendChat(text); break;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_chatlogReader(QStandardPaths::locate(QStandardPaths::DocumentsLocation, "\\GTA San Andreas User Files\\SAMP\\chatlog.txt")),
    m_healthOverlayTimer(this), m_statsOverlayTimer(this),
    m_lastSpamWarning(this)
{
    ui->setupUi(this);

    QObject::connect(QKeyHook::instance(), SIGNAL(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)), SLOT(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)));
    QObject::connect(&m_chatlogReader, SIGNAL(onLine(QString)), SLOT(onChatlog(QString)));
    QObject::connect(&m_healthOverlayTimer, SIGNAL(timeout()), SLOT(onHealthOverlayTimer()));
    QObject::connect(&m_statsOverlayTimer, SIGNAL(timeout()), SLOT(onStatsOverlayTimer()));

    m_lastSpamWarning.setSingleShot(true);

    m_healthOverlayTimer.setSingleShot(false);
    m_statsOverlayTimer.setSingleShot(false);

    m_healthOverlayTimer.start(50);
    m_statsOverlayTimer.start(3000);
}

MainWindow::~MainWindow()
{
    QKeyHook::drop();
    delete ui;
}

bool MainWindow::isSpamWarningActive() const
{
    return m_lastSpamWarning.isActive();
}

void MainWindow::onGlobalKeyPressed(KBDLLHOOKSTRUCT *key, bool& block)
{
    if((key->flags >> 7) & 1) // keyUp
        return;

    if(!(key->flags & 1))
    {
        switch(key->vkCode)
        {
        /* 1 */ SEND(0x31, "/stats");
        /* 2 */ SEND(0x32, "/carlock");
        /* 3 */ SEND(0x33, "/lock");
        /* 4 */ SEND(0x34, "/engine");
        /* 5 */ SEND(0x35, "/lights");

        /* X */ SEND(0x58, "/exit");
        /* Y */ SEND(0x59, "/enter");

        /* 1 */ SEND(VK_NUMPAD1, "/gettrailer waren");
        /* 2 */ SEND(VK_NUMPAD2, "/gettrailer benzin");
        /* 3 */ SEND(VK_NUMPAD3, "/gettrailer destroy");
        /* 4 */ SEND(VK_NUMPAD4, "/fill 1000");
        /* 5 */ SEND(VK_NUMPAD5, "/sellfuel");
        /* 6 */ SEND(VK_NUMPAD6, "/sellfuel 1000");
        /* 7 */ SEND(VK_NUMPAD7, "/loadimport 1000");
        /* 8 */ SEND(VK_NUMPAD8, "/sellimport 1000");

        case VK_F2:
            {
                if(!m_SAMP.isInForeground())
                    break;

                block = true;
                m_updateStats = !m_updateStats;

                if(m_updateStats)
                    m_SAMP.addChatMessage("{ffffff} Stats-Updates: {00ff00}Aktiviert");
                else
                    m_SAMP.addChatMessage("{ffffff} Stats-Updates: {ff0000}Deaktiviert");

                break;
            }
        }
    }
}

void MainWindow::onChatlog(const QString &s)
{
    if(s.contains("Antiflood:", Qt::CaseInsensitive))
    {
        QRegExp rx("(\\d+) Sekunde", Qt::CaseInsensitive);
        if(rx.indexIn(s) != -1)
        {
            int seconds = rx.cap(1).toInt();
            m_lastSpamWarning.start(seconds * 1000 + 500);
        }
    }
}

void MainWindow::onHealthOverlayTimer()
{
    if(!m_SAMP.isInForeground())
        return;

    if(m_healthOverlay == -1)
        m_healthOverlay.init("Trebuchet MS", 6, false, false, 720, 90, 0xFFFFFFFF, "100", true, true);

    if(m_healthOverlay == -1)
        return;

    if(!m_healthOverlay.setString(QString().sprintf("%.0f", m_SAMP.getPlayerHealth()).toStdString().c_str()))
        m_healthOverlay.destroy();
}

void MainWindow::onStatsOverlayTimer()
{
    if(!m_SAMP.isInForeground())
        return;

    if(m_statsOverlay == -1)
        m_statsOverlay.init("Trebuchet MS", 8, false, false, 660, 160, 0xFFFFFFFF, "", true, true);

    if(m_statsOverlay == -1)
        return;

    if(isSpamWarningActive())
        return;

    QString stats = QString(readStats().c_str());
    if(stats.length() == 0)
        return;

    auto formatOverlayString = [&](QString stats, QString entry) -> QString
    {
        auto keyForEntry = [&](QString text, QString entry) -> QString
        {
            QRegularExpression rx(entry + ":\\[(.*?)\\]", QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
            auto i = rx.globalMatch(text);
            if(i.hasNext())
                return i.next().captured(1);

            return {};
        };

        return QString().sprintf("{ffffff}%s: {00ff00}%s\r\n", entry.toStdString().c_str(), keyForEntry(stats, entry).toStdString().c_str());
    };


    QString overlay_str = "Updates: ";
    if(m_updateStats)
        overlay_str += "{00ff00} Aktiviert\r\n";
    else
        overlay_str += "{ff0000} Deaktiviert\r\n";

    overlay_str += formatOverlayString(stats, "Materialien");
    overlay_str += formatOverlayString(stats, "SafeMats");

    overlay_str += formatOverlayString(stats, "Drogen");
    overlay_str += formatOverlayString(stats, "SafeDrugs");

    overlay_str += formatOverlayString(stats, "HackingCodes");

    overlay_str += formatOverlayString(stats, "Kondome");

    overlay_str += formatOverlayString(stats, "Respekt");
    overlay_str += formatOverlayString(stats, "Geld");
    overlay_str += formatOverlayString(stats, "Bank");
    overlay_str += formatOverlayString(stats, "Tode");
    overlay_str += formatOverlayString(stats, "WantedLevel");
    overlay_str += formatOverlayString(stats, "Minuten seit Payday");

    if(!m_statsOverlay.setString(overlay_str.toStdString().c_str()))
        m_statsOverlay.destroy();
}
