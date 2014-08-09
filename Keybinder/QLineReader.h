#ifndef QLINEREADER_H
#define QLINEREADER_H

#include <QObject>
#include <QTimer>
#include <QFile>

class QLineReader : public QObject
{
    Q_OBJECT

public:
    explicit QLineReader(const QString& filePath, QObject *parent = 0);
    ~QLineReader();

    operator bool();

signals:
    void onLine(const QString&);
    void onClear();

private slots:
    void onTimer();

private:
    QFile m_file;
    QTimer m_timer;
    int m_lineCount = 0;

    int lineCount();
};

#endif // QLINEREADER_H
