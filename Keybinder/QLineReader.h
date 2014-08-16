#ifndef QLINEREADER_H
#define QLINEREADER_H

#include <QObject>
#include <QTimer>
#include <QFile>

class QLineReader : public QObject
{
    Q_OBJECT

public:
    explicit QLineReader(const QString& filePath, const QString& delimiter = "\r\n", QObject *parent = 0);
    ~QLineReader();

    operator bool();

signals:
    void onLine(const QString&);
    void onClear();

private slots:
    void onTimer();

private:
    QFile m_file;
    QString m_delimiter;
    QTimer m_timer;
    int m_lineCount = 0;

    int lineCount();
};

#endif // QLINEREADER_H
