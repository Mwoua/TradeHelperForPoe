#pragma once

#include "Defines.hpp"

#include <QObject>

class QThread;

/// Manage a thread to monitor both client text file
class ClientMonitor : public QObject
{
    Q_OBJECT;

  public:
    ClientMonitor();
    ~ClientMonitor() override;

  private:
    QThread *mMonitor1                = nullptr;
    std::streampos mLastReadPosition1 = 0, mLastReadPosition2 = 0;

    void MonitorTextFile();
    static void ReadChangedFileContents( const QString &aFile, std::streampos &aLastReadPosition, PoeVersion aVersion );
};
