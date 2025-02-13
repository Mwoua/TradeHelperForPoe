#include "ClientMonitor.hpp"
#include "SystemTray.hpp"
#include "version.h"

#include <QApplication>
#include <QMainWindow>
#include <QStyleHints>

#include <memory>

int main( int argc, char *argv[] )
{
    const QApplication lApp( argc, argv );
    lApp.setOrganizationName( TRADEHELPER_COMPANY_NAME );
    lApp.setApplicationName( TRADEHELPER_BINARY_NAME );
    lApp.setWindowIcon( QIcon( ":/TradeHelperForPoe.ico" ) );
    lApp.setQuitOnLastWindowClosed( false );

    QApplication::setStyle( "Fusion" );
    QGuiApplication::styleHints()->setColorScheme( Qt::ColorScheme::Dark );

    auto lClientMonitor = std::make_unique<ClientMonitor>();
    QSystemTrayIcon lTrayIcon;
    SetupSystemTray( lTrayIcon, lClientMonitor );
    lTrayIcon.show();

    return lApp.exec();
}