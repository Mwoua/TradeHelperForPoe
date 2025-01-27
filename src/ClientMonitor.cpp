#include "ClientMonitor.h"

#include "Settings.h"

#include <QFile>
#include <QFileSystemWatcher>
#include <QThread>

#include <fstream>

ClientMonitor::ClientMonitor()
    : mMonitor1( new QThread( this ) )
{
    connect( mMonitor1, &QThread::started, this, [this]() { MonitorTextFile(); } );
    mMonitor1->start();
}

ClientMonitor::~ClientMonitor()
{
    mMonitor1->requestInterruption();
    mMonitor1->quit();
    mMonitor1->wait();
}

void ClientMonitor::MonitorTextFile()
{
    if( QFile( Settings::GetPoe1Client() ).exists() )
    {
        auto *lWatcher1 = new QFileSystemWatcher( { Settings::GetPoe1Client() }, QThread::currentThread() );
        connect( lWatcher1, &QFileSystemWatcher::fileChanged, this, [this]( const QString &aFile ) { ReadChangedFileContents( aFile, mLastReadPosition1 ); } );
    }
    if( QFile( Settings::GetPoe2Client() ).exists() )
    {
        auto *lWatcher2 = new QFileSystemWatcher( { Settings::GetPoe2Client() }, QThread::currentThread() );
        connect( lWatcher2, &QFileSystemWatcher::fileChanged, this, [this]( const QString &aFile ) { ReadChangedFileContents( aFile, mLastReadPosition2 ); } );
    }
}

void ClientMonitor::ReadChangedFileContents( const QString &aFile, std::streampos &aLastReadPosition )
{
    std::ifstream lFile( aFile.toStdString(), std::ios_base::in );
    if( !lFile.is_open() )
    {
        return;
    }
    if( aLastReadPosition > 0 )
    {
        lFile.seekg( aLastReadPosition );
    }

    std::string lLastLine;
    while( std::getline( lFile, lLastLine ) )
    {
        // TODO: check line if its a trade
    }
    aLastReadPosition = lFile.tellg();
}
