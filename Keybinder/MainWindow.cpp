#include "StatsAPI/StatsAPI.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QStandardPaths>
#include <QRegularExpression>
#include <QTextStream>

#include <string>
#include <functional>

#define SEND(VK, text) case VK: if(!m_SAMP.isInForeground()) break; if(m_SAMP.isInChat()) break; block = true; m_SAMP.sendChat(text); break;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_infoDialog(this),
    m_chatlogReader(QStandardPaths::locate(QStandardPaths::DocumentsLocation, "\\GTA San Andreas User Files\\SAMP\\chatlog.txt")),
    m_healthOverlayTimer(this), m_statsOverlayTimer(this),
    m_lastSpamWarning(this)
{
    ui->setupUi(this);


    QObject::connect(ui->actionInfo, SIGNAL(triggered()), &m_infoDialog, SLOT(show()));

    QObject::connect(QKeyHook::instance(), SIGNAL(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)), SLOT(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)), Qt::DirectConnection);

    QObject::connect(&m_chatlogReader, SIGNAL(onLine(QString)), SLOT(onChatlog(QString)));
    QObject::connect(&m_chatlogReader, SIGNAL(onClear()), ui->chatTextBrowser, SLOT(clear()));

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

bool MainWindow::addChatMessage(QString str)
{
    str.insert(0, "  ~  {ffffff}[{33ffff}Keybinder{ffffff}] ");

    ui->outputTextBrowser->append(convertToHTMLColorCodes(str));
    return m_SAMP.addChatMessage(str.toStdString().c_str());
}

QString MainWindow::convertToHTMLColorCodes(const QString& text)
{
    std::function<void(QString&, QString)> convert = [&convert](QString& ret, QString str)
    {
        QRegularExpression rx("\\{(.*?)\\}(.*)", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
        auto globalMatch = rx.globalMatch(str);

        if(ret.length() == 0)
            ret += str.section('{', 0, 0);

        if(globalMatch.hasNext())
        {
            auto match = globalMatch.next();
            if(match.hasMatch())
            {
                QString captured = match.captured(2);
                ret += "<font color=#" + match.captured(1) + ">" + captured.section('{', 0, 0) +"</font>";

                convert(ret, captured);
            }
            else
            {
                ret += str;
            }
        }
    };

    QString ret;
    convert(ret, text);

    return ret;
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
        /* 6 */ SEND(0x36, "/accept refill");
        /* 7 */ SEND(0x37, "/accept repair");
        /* 8 */ SEND(0x38, "/accept heal");
        /* 9 */ SEND(0x39, "/accept hotdog");
        /* 0 */ SEND(0x30, "/accept sex");

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
                    addChatMessage("Stats-Updates: {00ff00}Aktiviert");
                else
                    addChatMessage("Stats-Updates: {ff0000}Deaktiviert");

                break;
            }
        }
    }
}

void MainWindow::onChatlog(const QString &s)
{
    ui->chatTextBrowser->append(convertToHTMLColorCodes(s));

    if(s.contains("Antiflood:", Qt::CaseInsensitive))
    {
        QRegExp rx("(\\d+) Sekunde", Qt::CaseInsensitive);
        if(rx.indexIn(s) != -1)
        {
            int seconds = rx.cap(1).toInt();
            m_lastSpamWarning.start(seconds * 1000 + 500);
        }
    }

    if(s.contains("* KFZ-Mechaniker", Qt::CaseInsensitive))
    {
        QRegularExpression rx("(\\d+) Litern für \\$(\\d+)", QRegularExpression::CaseInsensitiveOption);
        auto match = rx.match(s);

        if(!match.hasMatch())
            return;

        QString str;
        QTextStream ts(&str);

        ts << "Ein Mechaniker bietet dir fuer ";
        ts << match.captured(2) << "$ " << match.captured(1) << " Liter an: ";
        ts << (float)(match.captured(2).toFloat() / match.captured(1).toFloat()) << "$/Liter";

        addChatMessage(str);
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
    const int X = 620, Y = 160;
    if(!m_SAMP.isInForeground())
        return;

    if(m_statsBoxOverlay == -1)
    {
        m_statsBoxOverlay.init(X, Y, 140, 400, 0x80000000, true);
        m_statsBoxOverlay.setBorder(3, true);
        m_statsBoxOverlay.setBorderColor(0xFF886A08);
    }

    if(m_statsBoxOverlay == -1)
        return;

    if(m_statsOverlay == -1)
        m_statsOverlay.init("Trebuchet MS", 7, false, false, X + 10, Y + 5, 0xFFFFFFFF, "Statistik wird geladen und initialisiert...", true, true);

    if(m_statsOverlay == -1)
        return;

    if(isSpamWarningActive())
        return;

    QString stats = QString(readStats().c_str());
    if(stats.length() == 0)
        return;

    ui->statsLabel->setText(stats);
    auto formatOverlayString = [&](QString stats, QString entry, QString display) -> QString
    {
        if(display.length() == 0)
            display = entry;

        auto keyForEntry = [&](QString text, QString entry) -> QString
        {
            QRegularExpression rx(entry + ":\\[(.*?)\\]", QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
            auto i = rx.globalMatch(text);
            if(i.hasNext())
                return i.next().captured(1);

            return {};
        };

        return QString().sprintf("{ffffff}%s: {ff0000}%s\r\n", display.toStdString().c_str(), keyForEntry(stats, entry).toStdString().c_str());
    };


    QString overlay_str = "Updates: ";
    if(m_updateStats)
        overlay_str += "{00ff00} Aktiviert\r\n";
    else
        overlay_str += "{ff0000} Deaktiviert\r\n";

    overlay_str += "\r\n{33ffff} ~ Allgemeines ~\r\n";
    overlay_str += formatOverlayString(stats, "Level", "");
    overlay_str += formatOverlayString(stats, "Respekt", "");
    overlay_str += formatOverlayString(stats, "Spielzeit", "");
    overlay_str += formatOverlayString(stats, "Tel.", "Nummer");
    overlay_str += formatOverlayString(stats, "WantedLevel", "Wanteds");
    overlay_str += formatOverlayString(stats, "Minuten seit Payday", "");

    overlay_str += "\r\n{33ffff} ~ Materialien ~\r\n";
    overlay_str += formatOverlayString(stats, "Materialien", "Mats auf der Hand");
    overlay_str += formatOverlayString(stats, "SafeMats", "Mats in der Savebox");

    overlay_str += "\r\n{33ffff} ~ Drogen ~\r\n";
    overlay_str += formatOverlayString(stats, "Drogen", "Drogen auf der Hand");
    overlay_str += formatOverlayString(stats, "SafeDrugs", "Drogen in der Savebox");

    overlay_str += "\r\n{33ffff} ~ Hacking ~\r\n";
    overlay_str += formatOverlayString(stats, "HackingCodes", "Codes");

    overlay_str += "\r\n{33ffff} ~ Eigentum ~\r\n";
    overlay_str += formatOverlayString(stats, "Kondome", "");

    overlay_str += "\r\n{33ffff} ~ Vermoegen ~\r\n";
    overlay_str += formatOverlayString(stats, "Geld", "Geld auf der Hand");
    overlay_str += formatOverlayString(stats, "Bank", "");

    overlay_str += "\r\n{33ffff} ~ Deathmatch ~\r\n";
    overlay_str += formatOverlayString(stats, "Kills\\(DM\\)", "Kills davon SDM");
    overlay_str += formatOverlayString(stats, "Tode", "");
    overlay_str += formatOverlayString(stats, "DM-Rate", "");

    if(!m_statsOverlay.setString(overlay_str.toStdString().c_str()))
    {
        m_statsOverlay.destroy();
        m_statsBoxOverlay.destroy();
    }
}
