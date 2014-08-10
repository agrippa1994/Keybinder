#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QStandardPaths>

#define SEND(VK, text) case VK: if(GetForegroundWindow() != FindWindowA(0, "GTA:SA:MP")) break; if(m_SAMP.isInChat()) break; block = true; m_SAMP.sendChat(text); break;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_chatlogReader(QStandardPaths::locate(QStandardPaths::DocumentsLocation, "\\GTA San Andreas User Files\\SAMP\\chatlog.txt")),
    m_healthOverlayTimer(this)
{
    ui->setupUi(this);

    QObject::connect(QKeyHook::instance(), SIGNAL(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)), SLOT(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)));
    QObject::connect(&m_chatlogReader, SIGNAL(onLine(QString)), SLOT(onChatlog(QString)));
    QObject::connect(&m_healthOverlayTimer, SIGNAL(timeout()), SLOT(onHealthOverlayTimer()));

    m_healthOverlayTimer.setSingleShot(false);
    m_healthOverlayTimer.start(50);
}

MainWindow::~MainWindow()
{
    QKeyHook::drop();
    delete ui;
}

void MainWindow::onGlobalKeyPressed(KBDLLHOOKSTRUCT *key, bool& block)
{
    if((key->flags >> 7) & 1) // keyUp
        return;

    if(!(key->flags & 1)) // kein Numpad
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
        }
    }
    else // Numpad
    {

    }
}

void MainWindow::onChatlog(const QString &s)
{
    ui->textBrowser->append(s);
}

void MainWindow::onHealthOverlayTimer()
{
    if(m_healthOverlay == -1)
        m_healthOverlay.init("Trebuchet MS", 6, false, false, 720, 90, 0xFFFFFFFF, "100", true, true);

    if(m_healthOverlay == -1)
        return;

    m_healthOverlay.setString(QString().sprintf("%.0f", m_SAMP.getPlayerHealth()).toStdString().c_str());
}
