#include "QKeyHook.h"

#include <QMutex>
#include <QMutexLocker>

QKeyHook *QKeyHook::m_pInstance;

LRESULT WINAPI ll_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return QKeyHook::instance()->keyProc(nCode, wParam, lParam);
}

QKeyHook *QKeyHook::instance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    if(!m_pInstance)
        m_pInstance = new QKeyHook;

    return m_pInstance;
}

bool QKeyHook::drop()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    if(!m_pInstance)
        return false;

    delete m_pInstance;
    m_pInstance = 0;

    return m_pInstance == 0;
}

LRESULT QKeyHook::keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    bool bBlock = false;
    emit onGlobalKeyPressed((KBDLLHOOKSTRUCT *)lParam, bBlock);

    return bBlock ? 1 : CallNextHookEx(m_hHook, nCode, wParam, lParam);
}

QKeyHook::QKeyHook() : QObject(0), m_hHook(SetWindowsHookExA(WH_KEYBOARD_LL, ll_proc, GetModuleHandle(NULL), 0))
{

}

QKeyHook::~QKeyHook()
{
    UnhookWindowsHookEx(m_hHook);
    m_hHook = 0;
}
