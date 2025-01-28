#include "AboutForm.hpp"
#include "version.h"

#include <QLabel>
#include <QVBoxLayout>

AboutForm::AboutForm( QWidget *aParent )
{
    setWindowTitle( "About " );
    setMinimumSize( 200, 50 ); // Workaround this bug https://bugreports.qt.io/browse/QTBUG-73258

    auto *lMainLayout = new QVBoxLayout;
    setLayout( lMainLayout );

    lMainLayout->addWidget( new QLabel( QString( "%1 %2" ).arg( TRADEHELPER_BINARY_NAME ).arg( TRADEHELPER_VERSION_STRING ) ) );
    lMainLayout->addWidget( new QLabel( "Small utility to make trading item easier in path of exile." ) );
    auto *lLbGithub = new QLabel( "See <a href=\"https://github.com/Mwoua/TradeHelperForPoe\">https://github.com/Mwoua/TradeHelperForPoe</a>" );
    lLbGithub->setTextInteractionFlags( Qt::TextBrowserInteraction );
    lLbGithub->setOpenExternalLinks( true );
    lMainLayout->addWidget( lLbGithub );
    lMainLayout->addWidget( new QLabel( QString( "This program uses the Qt framework licensed under LGPL, version %1" ).arg( QT_VERSION_STR ) ) );
}
