#include "TradeWidget.hpp"

#include "Currencies.hpp"
#include "PoeCommand.hpp"
#include "Settings.hpp"
#include "Utils/ElideLabel.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QTime>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

constexpr auto COLLAPSED_PROPERTY = "CollapseState";
constexpr auto ARROW_DOWN         = "\u2b9f"; // ⮟
constexpr auto ARROW_UP           = "\u2b9d"; // ⮝
constexpr auto SIDE_COLOR_SIZE    = 5;

TradeWidget::TradeWidget( Trade aTrade, PoeVersion aVersion )
    : mPoeVersion( aVersion )
    , mTrade( std::move( aTrade ) )
{
    SetupUi();
    setStyleSheet( QString( "TradeWidget{border-left: %1px solid %2;}QAbstractButton { padding: 1px; }" ).arg( SIDE_COLOR_SIZE ).arg( aTrade.mIncoming ? "green" : "red" ) );
    const int lWidth = Settings::TradeWidgetWidth();
    setFixedWidth( lWidth );
}

void TradeWidget::SetupUi()
{
    auto *lMainLayout = new QVBoxLayout( this );
    lMainLayout->setContentsMargins( SIDE_COLOR_SIZE, 0, 0, 0 );
    lMainLayout->setSpacing( 1 );

    lMainLayout->addLayout( SetupUiFirstRow() );
    lMainLayout->addWidget( SetupUiSecondRow() );
    lMainLayout->setAlignment( Qt::AlignTop );
}

QLayout *TradeWidget::SetupUiFirstRow()
{
    auto *lFirstRowLayout = new QHBoxLayout;
    lFirstRowLayout->setContentsMargins( 0, 0, 0, 0 );
    lFirstRowLayout->setSpacing( 1 );
    mCollapseButton = new QPushButton( ARROW_UP );
    mCollapseButton->setProperty( COLLAPSED_PROPERTY, false );
    connect( mCollapseButton, &QPushButton::clicked, this, &TradeWidget::ManageCollaspeState );
    lFirstRowLayout->addWidget( mCollapseButton );
    lFirstRowLayout->addWidget( BuildTradeItemWidget() );
    lFirstRowLayout->addStretch( 1 );
    lFirstRowLayout->addWidget( BuildTradePriceWidget() );
    mLbTime = new QLabel( "00:00" );
    lFirstRowLayout->addWidget( mLbTime );
    mTimer = new QTimer( this );
    mElapsedTimer.start();
    connect( mTimer, &QTimer::timeout, this, &TradeWidget::UpdateTime );
    mTimer->setInterval( 1000 );
    mTimer->start();

    if( mTrade.mIncoming )
    {
        auto *lPBInviteCustomer = new QPushButton( QIcon( ":/TradeHelper/Invite" ), "" );
        lFirstRowLayout->addWidget( lPBInviteCustomer );
        lPBInviteCustomer->setToolTip( "Invite customer to group" );
        connect( lPBInviteCustomer, &QPushButton::clicked, this, [this]() { PoeCommand( "/invite " + mTrade.mUser, mPoeVersion ); } );
    }
    else
    {
        auto *lPBToSellerHideout = new QPushButton( QIcon( ":/TradeHelper/SellerHideout" ), "" );
        lFirstRowLayout->addWidget( lPBToSellerHideout );
        lPBToSellerHideout->setToolTip( "Go to seller hideout" );
        connect( lPBToSellerHideout, &QPushButton::clicked, this, [this]() { PoeCommand( "/hideout " + mTrade.mUser, mPoeVersion ); } );
    }

    if( mTrade.mIncoming )
    {
        auto *lPBTrade = new QPushButton( QIcon( ":/TradeHelper/Trade" ), "" );
        lFirstRowLayout->addWidget( lPBTrade );
        lPBTrade->setToolTip( "Trade with customer" );
        connect( lPBTrade, &QPushButton::clicked, this, [this]() { PoeCommand( "/tradewith " + mTrade.mUser, mPoeVersion ); } );
    }

    auto *lPBThanks = new QPushButton( QIcon( ":/TradeHelper/Thanks" ), "" );
    lFirstRowLayout->addWidget( lPBThanks );
    lPBThanks->setToolTip( "Say thanks" );
    connect( lPBThanks, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::Thanks().toStdString(), mPoeVersion ); } );

    auto *lPBLeave = new QPushButton( QIcon( ":/TradeHelper/Leave" ), "" );
    lFirstRowLayout->addWidget( lPBLeave );
    lPBLeave->setToolTip( "Leave group" );
    connect( lPBLeave, &QPushButton::clicked, this, [this]() { PoeCommand( "/leave", mPoeVersion ); } );

    if( !mTrade.mIncoming )
    {
        auto *lPBHome = new QPushButton( QIcon( ":/TradeHelper/Home" ), "" );
        lFirstRowLayout->addWidget( lPBHome );
        lPBHome->setToolTip( "Go back to hideout" );
        connect( lPBHome, &QPushButton::clicked, this, [this]() { PoeCommand( "/hideout", mPoeVersion ); } );
    }

    auto *lPBWhisp = new QPushButton( QIcon( ":/TradeHelper/Whisp" ), "" );
    lFirstRowLayout->addWidget( lPBWhisp );
    lPBWhisp->setToolTip( "Send message to the customer" );
    connect( lPBWhisp, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " ", mPoeVersion, false ); } );

    auto *lPBClose = new QPushButton( QIcon( ":/TradeHelper/Close" ), "" );
    lFirstRowLayout->addWidget( lPBClose );
    lPBClose->setToolTip( "Close this trade widget" );
    connect( lPBClose,
             &QPushButton::clicked,
             this,
             [this]()
             {
                 BringPoeToForeground( mPoeVersion );
                 deleteLater();
             } );

    return lFirstRowLayout;
}

QWidget *TradeWidget::SetupUiSecondRow()
{
    mSecondRow = new QWidget;
    mSecondRow->setContentsMargins( 0, 0, 0, 0 );
    auto *lSecondRowLayout = new QHBoxLayout( mSecondRow );
    lSecondRowLayout->setContentsMargins( 0, 0, 0, 0 );
    lSecondRowLayout->setSpacing( 1 );

    auto *lPBCustomer = new QPushButton( mTrade.mUser.c_str() );
    lPBCustomer->setFlat( true );
    lPBCustomer->setToolTip( "Get info about customer" );
    lSecondRowLayout->addWidget( lPBCustomer );
    lSecondRowLayout->insertSpacing( -1, 2 );
    connect( lPBCustomer, &QPushButton::clicked, this, [this]() { PoeCommand( "/whois " + mTrade.mUser, mPoeVersion ); } );

    lSecondRowLayout->addWidget( new ElideLabel( mTrade.mLeague.c_str() ) );
    lSecondRowLayout->insertSpacing( -1, 2 );
    if( mTrade.mStash.has_value() )
    {
        lSecondRowLayout->addWidget( new ElideLabel( mTrade.mStash.value().c_str() ) );
        lSecondRowLayout->insertSpacing( -1, 2 );
    }
    lSecondRowLayout->addStretch( 1 );
    if( mTrade.mComment.has_value() )
    {
        auto *lLabel = new ElideLabel( mTrade.mComment.value().c_str() );
        lLabel->setToolTip( mTrade.mComment.value().c_str() );
        lSecondRowLayout->addWidget( lLabel );
    }
    lSecondRowLayout->addStretch( 1 );

    if( Settings::CustomMessage1().size() > 0 )
    {
        auto *lPBMessage1 = new QPushButton( QIcon( ":/TradeHelper/Message1" ), "" );
        lPBMessage1->setToolTip( QString( "Send \"%1\" to customer" ).arg( Settings::CustomMessage1() ) );
        lSecondRowLayout->addWidget( lPBMessage1 );
        connect( lPBMessage1, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::CustomMessage1().toStdString(), mPoeVersion ); } );
    }

    if( Settings::CustomMessage2().size() > 0 )
    {
        auto *lPBMessage2 = new QPushButton( QIcon( ":/TradeHelper/Message2" ), "" );
        lPBMessage2->setToolTip( QString( "Send \"%1\" to customer" ).arg( Settings::CustomMessage2() ) );
        lSecondRowLayout->addWidget( lPBMessage2 );
        connect( lPBMessage2, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::CustomMessage2().toStdString(), mPoeVersion ); } );
    }

    if( mTrade.mIncoming )
    {
        auto *lPBWait = new QPushButton( QIcon( ":/TradeHelper/Wait" ), "" );
        lPBWait->setToolTip( QString( "Send \"%1\" to customer" ).arg( Settings::BusyString() ) );
        lSecondRowLayout->addWidget( lPBWait );
        connect( lPBWait, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::BusyString().toStdString(), mPoeVersion ); } );

        auto *lPBSearch = new QPushButton( QIcon( ":/TradeHelper/Search" ), "" );
        lPBSearch->setToolTip( "Search for item in stash.\nPlease open stash first" );
        lSecondRowLayout->addWidget( lPBSearch );
        connect( lPBSearch, &QPushButton::clicked, this, [this]() { PoeCommandSearch( mPoeVersion, mTrade.mItem ); } );
    }

    auto *lPBKick = new QPushButton( QIcon( ":/TradeHelper/Kick" ), "" );
    lPBKick->setToolTip( "Kick customer from group" );
    lSecondRowLayout->addWidget( lPBKick );
    connect( lPBKick, &QPushButton::clicked, this, [this]() { PoeCommand( "/kick " + mTrade.mUser, mPoeVersion ); } );

    if( mTrade.mIncoming )
    {
        auto *lPBStillInterested = new QPushButton( QIcon( ":/TradeHelper/StillInterested" ), "" );
        lSecondRowLayout->addWidget( lPBStillInterested );
        lPBStillInterested->setToolTip( "Ask user if he still wants the item" );
        connect( lPBStillInterested,
                 &QPushButton::clicked,
                 this,
                 [this]()
                 {
                     std::string lStillInterested = " Hi, are you still interested in " + mTrade.mItem;
                     if( mTrade.mPrice.has_value() )
                     {
                         lStillInterested += " for " + mTrade.mPrice.value();
                     }

                     PoeCommand( "@" + mTrade.mUser + lStillInterested, mPoeVersion );
                 } );
    }
    else
    {
        auto *lPBRepeat = new QPushButton( QIcon( ":/TradeHelper/Repeat" ), "" );
        lSecondRowLayout->addWidget( lPBRepeat );
        lPBRepeat->setToolTip( "Send the trade request again" );
        connect( lPBRepeat, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + mTrade.mEntireString, mPoeVersion ); } );
    }

    return mSecondRow;
}

QWidget *TradeWidget::BuildTradeItemWidget()
{
    auto *lReturn = new QWidget;
    auto *lLayout = new QHBoxLayout( lReturn );

    auto *lLb1 = new ElideLabel( mTrade.mItem.c_str() );
    lLb1->setToolTip( mTrade.mItem.c_str() );
    lLayout->addWidget( lLb1 );
    // TODO parse item string for bulk trade or currency

    return lReturn;
}

QWidget *TradeWidget::BuildTradePriceWidget()
{
    auto *lReturn = new QWidget;

    if( mTrade.mPrice.has_value() )
    {
        const std::string lPrice = mTrade.mPrice.value();
        lReturn->setToolTip( lPrice.c_str() );
        auto *lLayout = new QHBoxLayout( lReturn );
        lLayout->setContentsMargins( 0, 0, 0, 0 );
        lLayout->setSpacing( 2 );
        const std::string lAmount = lPrice.substr( 0, lPrice.find( ' ' ) );
        std::string lCurrency     = lPrice.substr( lPrice.find( ' ' ) + 1 );

        try
        {
            if( lCurrency.size() > 0 )
            {
                std::string lCurrencyPath = "";

                try
                {
                    lCurrencyPath = CurrencyToImageString( CURRENCIES.at( mTrade.mLanguage ).at( lCurrency ), mPoeVersion );
                }
                catch( const std::exception & )
                {
                    // fallback to english, some trades are sent in native language, other in english
                    lCurrencyPath = CurrencyToImageString( CURRENCIES.at( Language::English ).at( lCurrency ), mPoeVersion );
                }

                auto *lLbCount = new QLabel( lAmount.c_str() );
                auto *lLbImage = new QLabel();
                lLbImage->setPixmap( QPixmap( lCurrencyPath.c_str() ) );
                lLayout->addWidget( lLbCount );
                lLayout->addWidget( lLbImage );
            }
            else
            {
                throw std::invalid_argument( "Unexpected input format" );
            }
        }
        catch( const std::exception & )
        {
            // Error, just write the string
            auto *lLb1 = new QLabel( mTrade.mPrice.value().c_str() );
            lLayout->addWidget( lLb1 );
        }
    }

    // TODO parse item string to display the currency

    return lReturn;
}

void TradeWidget::UpdateTime() { mLbTime->setText( QTime( 0, 0 ).addMSecs( mElapsedTimer.elapsed() ).toString( "mm:ss" ) ); }

void TradeWidget::ManageCollaspeState()
{
    const bool lCollapsed = mCollapseButton->property( COLLAPSED_PROPERTY ).toBool();
    if( lCollapsed )
    {
        mCollapseButton->setText( ARROW_UP );
        mCollapseButton->setProperty( COLLAPSED_PROPERTY, false );
        mSecondRow->show();
    }
    else
    {
        mCollapseButton->setText( ARROW_DOWN );
        mCollapseButton->setProperty( COLLAPSED_PROPERTY, true );
        mSecondRow->hide();
    }
}

void TradeWidget::paintEvent( QPaintEvent *e )
{
    // Needed so that stylesheet is correctly applied to the global widget
    QStyleOption opt;
    opt.initFrom( this );
    QPainter p( this );
    style()->drawPrimitive( QStyle::PE_Widget, &opt, &p, this );
}
