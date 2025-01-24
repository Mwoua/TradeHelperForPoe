#include "SystemTray.h"
#include "version.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
    const QApplication lApp( argc, argv );
    lApp.setOrganizationName( TRADEHELPER_COMPANY_NAME );
    lApp.setApplicationName( TRADEHELPER_BINARY_NAME );
    lApp.setWindowIcon( QIcon( ":/TradeHelperForPoe.ico" ) );
    lApp.setQuitOnLastWindowClosed( false );

    QSystemTrayIcon lTrayIcon;
    SetupSystemTray( lTrayIcon );
    lTrayIcon.show();

    return lApp.exec();
}