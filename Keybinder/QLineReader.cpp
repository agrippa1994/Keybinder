#include "QLineReader.h"
#include <QTextStream>
#include <QDebug>
QLineReader::QLineReader(const QString& filePath, const QString& delimiter, QObject *parent) :
    QObject(parent), m_file(filePath), m_delimiter(delimiter)
{
    QObject::connect(&m_timer, SIGNAL(timeout()), SLOT(onTimer()));

    m_timer.setSingleShot(false);
    m_timer.start(20);
}

QLineReader::~QLineReader()
{
    m_timer.stop();
}

QLineReader::operator bool()
{
   return m_file.isOpen() && m_file.isReadable();
}

void QLineReader::onTimer()
{
    if(!m_file.isOpen() || !m_file.isReadable())
        if(!m_file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    if(m_lineCount > lineCount())
    {
        m_lineCount = 0;
        emit onClear();
    }

    {
        QTextStream stream(&m_file);
        stream.setAutoDetectUnicode(true);
        stream.seek(0);

        auto lines = stream.readAll().split(m_delimiter, QString::SkipEmptyParts);
        int current = 0;

        for(auto& line : lines)
        {
            current ++;

            if(current > m_lineCount)
            {
                m_lineCount = current;

                emit onLine(line);
            }   
        }
    }
}

int QLineReader::lineCount()
{
    if(!m_file.isOpen() || !m_file.isReadable())
        return 0;

    QTextStream stream(&m_file);
    stream.seek(0);

    return stream.readAll().split(m_delimiter, QString::SkipEmptyParts).count();
}
