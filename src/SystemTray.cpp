#include "SystemTray.hpp"

#include "AboutForm.hpp"
#include "Settings.hpp"
#include "version.h"

#include <QAction>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <regex>

namespace
{
    void CheckForUpdate( QSystemTrayIcon &aSystemTray, QNetworkReply *aReply );
}

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

    auto *lManager = new QNetworkAccessManager( &aSystemTray );
    QObject::connect( lManager, &QNetworkAccessManager::finished, &aSystemTray, [&aSystemTray]( QNetworkReply *aReply ) { CheckForUpdate( aSystemTray, aReply ); } );
    lManager->get( QNetworkRequest( QUrl( "https://raw.githubusercontent.com/Mwoua/TradeHelperForPoe/refs/heads/main/src/version.h" ) ) );
}

namespace
{
    void CheckForUpdate( QSystemTrayIcon &aSystemTray, QNetworkReply *aReply )
    {
        if( aReply->error() == QNetworkReply::NoError )
        {
            const std::string lVersionFileContents = QString( aReply->readAll() ).toStdString();

            const std::regex lRegexMajor( R"(^#define TRADEHELPER_MAJOR_VERSION\s+(\d+))" );
            const std::regex lRegexMinor( R"(^#define TRADEHELPER_MINOR_VERSION\s+(\d+))" );
            const std::regex lRegexPatch( R"(^#define TRADEHELPER_PATCH_VERSION\s+(\d+))" );

            std::smatch lMatchesMajor, lMatchesMinor, lMatchesPatch;

            if( std::regex_search( lVersionFileContents, lMatchesMajor, lRegexMajor ) && std::regex_search( lVersionFileContents, lMatchesMinor, lRegexMinor ) &&
                std::regex_search( lVersionFileContents, lMatchesPatch, lRegexPatch ) )
            {
                const int lLatestMajor = std::stoi( lMatchesMajor[1].str() );
                const int lLatestMinor = std::stoi( lMatchesMinor[1].str() );
                const int lLatestPatch = std::stoi( lMatchesPatch[1].str() );

                if( lLatestMajor > TRADEHELPER_MAJOR_VERSION || lLatestMinor > TRADEHELPER_MINOR_VERSION || lLatestPatch > TRADEHELPER_PATCH_VERSION )
                {
                    aSystemTray.setToolTip( "New version available" );
                    aSystemTray.setIcon(QIcon( ":/TradeHelperForPoe_msg.ico" ));
                }
            }
        }

        aReply->deleteLater();
    }
} // namespace