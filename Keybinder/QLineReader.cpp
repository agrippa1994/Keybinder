#include "QLineReader.h"

QLineReader::QLineReader(const QString& filePath, QObject *parent) :
    QObject(parent), m_file(filePath)
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
        m_file.seek(0);
        for(int current = 0; !m_file.atEnd(); current ++)
        {
            QString line = QString(m_file.readLine()).remove("\r\n");

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

    int count = 0;
    for(m_file.seek(0); !m_file.atEnd(); m_file.readLine(), count ++);

    return count;
}
