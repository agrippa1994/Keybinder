#include "MainWindow.h"
#include "ui_MainWindow.h"

#define SEND(VK, text) case VK: if(m_SAMP.isInChat()) break; block = true; m_SAMP.sendChat(text); break;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(QKeyHook::instance(), SIGNAL(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)), SLOT(onGlobalKeyPressed(KBDLLHOOKSTRUCT*,bool&)));
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
        }
    }
    else // Numpad
    {

    }
}
