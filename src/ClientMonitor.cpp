#include "ClientMonitor.hpp"

#include "Settings.hpp"
#include "StringParser.hpp"

#include <QFile>
#include <QFileSystemWatcher>
#include <QThread>
#include <QTimer>

#include <fstream>

ClientMonitor::ClientMonitor( )
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
        std::ifstream lFile( Settings::GetPoe1Client().toStdString(), std::ios_base::in );
        if( lFile.is_open() )
        {
            lFile.seekg( 0, std::ios::end );
            mLastReadPosition1 = lFile.tellg();
            auto *lWatcher1    = new QFileSystemWatcher( { Settings::GetPoe1Client() }, QThread::currentThread() );
            connect(
                lWatcher1, &QFileSystemWatcher::fileChanged, this, [this]( const QString &aFile ) { ReadChangedFileContents( aFile, mLastReadPosition1, PoeVersion::Poe1 ); } );
        }
    }
    if( QFile( Settings::GetPoe2Client() ).exists() )
    {
        std::ifstream lFile( Settings::GetPoe2Client().toStdString(), std::ios_base::in );
        if( lFile.is_open() )
        {
            lFile.seekg( 0, std::ios::end );
            mLastReadPosition2 = lFile.tellg();
            auto *lWatcher2    = new QFileSystemWatcher( { Settings::GetPoe2Client() }, QThread::currentThread() );
            connect(
                lWatcher2, &QFileSystemWatcher::fileChanged, this, [this]( const QString &aFile ) { ReadChangedFileContents( aFile, mLastReadPosition2, PoeVersion::Poe2 ); } );
        }
    }
    // QFileSystemWatcher does not detect changes made by Poe directly (I guess windows does not always refresh the status
    // so poll it regularly to actually trigger the signals
    auto *lForceUpdateTimer = new QTimer( this );
    connect( lForceUpdateTimer,
             &QTimer::timeout,
             this,
             []()
             {
                 QFile().exists( Settings::GetPoe1Client() );
                 QFile().exists( Settings::GetPoe2Client() );
             } );
    lForceUpdateTimer->start( 500 );
}

void ClientMonitor::ReadChangedFileContents( const QString &aFile, std::streampos &aLastReadPosition, PoeVersion aVersion )
{
    std::ifstream lFile( aFile.toStdString(), std::ios_base::in );
    if( !lFile.is_open() )
    {
        return;
    }
    if( aLastReadPosition > 0 )
    {
        lFile.seekg( aLastReadPosition );
        if( lFile.peek() == EOF )
        {
            // we are in a wrong position, reset to the end of the file
            lFile.seekg( 0, std::ios::end );
            aLastReadPosition = lFile.tellg();
        }
    }

    std::string lLastLine;
    while( std::getline( lFile, lLastLine ) )
    {
        StringParser( mTradeWidgetDisplayer, std::move( lLastLine ), aVersion );
    }
    lFile.clear();
    aLastReadPosition = lFile.tellg();
}
