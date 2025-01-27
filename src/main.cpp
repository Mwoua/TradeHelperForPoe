#include "ClientMonitor.hpp"
#include "SystemTray.hpp"
#include "version.h"

#include <QApplication>
#include <QStyleHints>

int main( int argc, char *argv[] )
{
    const QApplication lApp( argc, argv );
    lApp.setOrganizationName( TRADEHELPER_COMPANY_NAME );
    lApp.setApplicationName( TRADEHELPER_BINARY_NAME );
    lApp.setWindowIcon( QIcon( ":/TradeHelperForPoe.ico" ) );
    lApp.setQuitOnLastWindowClosed( false );

    QApplication::setStyle( "Fusion" );
    QGuiApplication::styleHints()->setColorScheme( Qt::ColorScheme::Dark );

    QSystemTrayIcon lTrayIcon;
    SetupSystemTray( lTrayIcon );
    lTrayIcon.show();

    const ClientMonitor lClientMonitor;

    return lApp.exec();
}