#ifndef QKEYHOOK_H
#define QKEYHOOK_H

#include "windows.h"

#include <QObject>

class QKeyHook : public QObject
{
    Q_OBJECT

    friend LRESULT WINAPI ll_proc(int nCode, WPARAM wParam, LPARAM lParam);

public:
    static QKeyHook *instance();
    static bool drop();

signals:
    void onGlobalKeyPressed(KBDLLHOOKSTRUCT *key, bool& block);

protected:
    virtual LRESULT keyProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static QKeyHook *m_pInstance;

    HHOOK m_hHook;

    QKeyHook();
    ~QKeyHook();

    QKeyHook(const QKeyHook&) = delete;
    QKeyHook(QKeyHook&&) = delete;
    QKeyHook& operator=(const QKeyHook&) = delete;
};

#endif
