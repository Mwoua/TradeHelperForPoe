#pragma once

#include "Defines.hpp"

#include <QObject>

class QThread;
class TradeWidgetDisplayer;

/// Manage a thread to monitor both client text file
class ClientMonitor : public QObject
{
    Q_OBJECT

  public:
    ClientMonitor( TradeWidgetDisplayer &aTradeWidgetDisplayer );
    ~ClientMonitor() override;

  private:
    QThread *mMonitor1                = nullptr;
    std::streampos mLastReadPosition1 = 0, mLastReadPosition2 = 0;
    TradeWidgetDisplayer &mTradeWidgetDisplayer;

    void MonitorTextFile();
    void ReadChangedFileContents( const QString &aFile, std::streampos &aLastReadPosition, PoeVersion aVersion );
};
