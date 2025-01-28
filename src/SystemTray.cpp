#include "SystemTray.hpp"
#include "AboutForm.hpp"
#include "Settings.hpp"

#include <QAction>
#include <QApplication>

void SetupSystemTray( QSystemTrayIcon &aSystemTray )
{
    aSystemTray.setIcon( QIcon( ":/TradeHelperForPoe.ico" ) );

    auto *lSettingsAction = new QAction( "&Settings", &aSystemTray );
    QObject::connect( lSettingsAction,
                      &QAction::triggered,
                      &aSystemTray,
                      []()
                      {
                          auto lSettings = new Settings();
                          lSettings->show();
                      } );

    auto *lAboutAction = new QAction( "&About" );
    QObject::connect( lAboutAction, &QAction::triggered, qApp, []() { AboutForm().exec(); } );

    auto *lQuitAction = new QAction( "&Quit", &aSystemTray );
    QObject::connect( lQuitAction, &QAction::triggered, qApp, &QApplication::quit );

    auto *lTrayIconMenu = new QMenu();
    lTrayIconMenu->addAction( lSettingsAction );
    lTrayIconMenu->addSeparator();
    lTrayIconMenu->addAction( lAboutAction );
    lTrayIconMenu->addAction( lQuitAction );

    aSystemTray.setContextMenu( lTrayIconMenu );
}
