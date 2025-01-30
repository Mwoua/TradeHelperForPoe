#include "TradeWidget.hpp"

#include "PoeCommand.hpp"
#include "Settings.hpp"
#include "Utils/ElideLabel.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

constexpr auto COLLAPSED_PROPERTY = "CollapseState";
constexpr auto ARROW_DOWN         = "\u2b9f"; // ⮟
constexpr auto ARROW_UP           = "\u2b9d"; // ⮝

TradeWidget::TradeWidget( Trade aTrade, PoeVersion aVersion )
    : mPoeVersion( aVersion )
    , mTrade( std::move( aTrade ) )
{
    SetupUi();
    setStyleSheet( "QAbstractButton { padding: 1px; }" );
    const int lWidth = Settings::TradeWidgetWidth();
    setFixedWidth( lWidth );
}

void TradeWidget::SetupUi()
{
    auto *lMainLayout = new QVBoxLayout( this );
    lMainLayout->setContentsMargins( 0, 0, 0, 0 );
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
        connect( lPBInviteCustomer, &QPushButton::clicked, this, [this]() { PoeCommand( "/invite " + mTrade.mUser, mPoeVersion ); } );
    }
    else
    {
        auto *lPBToSellerHideout = new QPushButton( QIcon( ":/TradeHelper/SellerHideout" ), "" );
        lFirstRowLayout->addWidget( lPBToSellerHideout );
        connect( lPBToSellerHideout, &QPushButton::clicked, this, [this]() { PoeCommand( "/hideout " + mTrade.mUser, mPoeVersion ); } );
    }

    if( mTrade.mIncoming )
    {
        auto *lPBTrade = new QPushButton( QIcon( ":/TradeHelper/Trade" ), "" );
        lFirstRowLayout->addWidget( lPBTrade );
        connect( lPBTrade, &QPushButton::clicked, this, [this]() { PoeCommand( "/tradewith " + mTrade.mUser, mPoeVersion ); } );
    }

    auto *lPBThanks = new QPushButton( QIcon( ":/TradeHelper/Thanks" ), "" );
    lFirstRowLayout->addWidget( lPBThanks );
    connect( lPBThanks, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::Thanks().toStdString(), mPoeVersion, false ); } );

    auto *lPBLeave = new QPushButton( QIcon( ":/TradeHelper/Leave" ), "" );
    lFirstRowLayout->addWidget( lPBLeave );
    connect( lPBLeave, &QPushButton::clicked, this, [this]() { PoeCommand( "/leave", mPoeVersion, false ); } );

    if( !mTrade.mIncoming )
    {
        auto *lPBHome = new QPushButton( QIcon( ":/TradeHelper/Home" ), "" );
        lFirstRowLayout->addWidget( lPBHome );
        connect( lPBHome, &QPushButton::clicked, this, [this]() { PoeCommand( "/hideout", mPoeVersion, false ); } );
    }

    auto *lPBWhisp = new QPushButton( QIcon( ":/TradeHelper/Whisp" ), "" );
    lFirstRowLayout->addWidget( lPBWhisp );
    connect( lPBWhisp, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " ", mPoeVersion, false ); } );

    auto *lPBClose = new QPushButton( QIcon( ":/TradeHelper/Close" ), "" );
    lFirstRowLayout->addWidget( lPBClose );
    connect( lPBClose, &QPushButton::clicked, this, &QWidget::deleteLater );

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
    lSecondRowLayout->addWidget( lPBCustomer );
    connect( lPBCustomer, &QPushButton::clicked, this, [this]() { PoeCommand( "/whois " + mTrade.mUser, mPoeVersion ); } );

    lSecondRowLayout->addWidget( new ElideLabel( mTrade.mLeague.c_str() ) );
    lSecondRowLayout->addStretch( 1 );
    if( mTrade.mComment.has_value() )
    {
        lSecondRowLayout->addWidget( new ElideLabel( mTrade.mComment.value().c_str() ) );
    }
    lSecondRowLayout->addStretch( 1 );

    if( Settings::CustomMessage1().size() > 0 )
    {
        auto *lPBMessage1 = new QPushButton( QIcon( ":/TradeHelper/Message1" ), "" );
        lSecondRowLayout->addWidget( lPBMessage1 );
        connect( lPBMessage1, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::CustomMessage1().toStdString(), mPoeVersion ); } );
    }

    if( Settings::CustomMessage2().size() > 0 )
    {
        auto *lPBMessage2 = new QPushButton( QIcon( ":/TradeHelper/Message2" ), "" );
        lSecondRowLayout->addWidget( lPBMessage2 );
        connect( lPBMessage2, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::CustomMessage2().toStdString(), mPoeVersion ); } );
    }

    if( mTrade.mIncoming )
    {
        auto *lPBWait = new QPushButton( QIcon( ":/TradeHelper/Wait" ), "" );
        lSecondRowLayout->addWidget( lPBWait );
        connect( lPBWait, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + Settings::BusyString().toStdString(), mPoeVersion ); } );

        auto *lPBSearch = new QPushButton( QIcon( ":/TradeHelper/Search" ), "" );
        lSecondRowLayout->addWidget( lPBSearch );
        connect( lPBSearch,
                 &QPushButton::clicked,
                 this,
                 [this]()
                 {
                     PoeCommandSearch( mPoeVersion, mTrade.mItem  );
                 } );
    }

    auto *lPBKick = new QPushButton( QIcon( ":/TradeHelper/Kick" ), "" );
    lSecondRowLayout->addWidget( lPBKick );
    connect( lPBKick, &QPushButton::clicked, this, [this]() { PoeCommand( "/kick " + mTrade.mUser, mPoeVersion ); } );

    if( mTrade.mIncoming )
    {
        auto *lPBStillInterested = new QPushButton( QIcon( ":/TradeHelper/StillInterested" ), "" );
        lSecondRowLayout->addWidget( lPBStillInterested );
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
        connect( lPBRepeat, &QPushButton::clicked, this, [this]() { PoeCommand( "@" + mTrade.mUser + " " + mTrade.mEntireString, mPoeVersion ); } );
    }

    return mSecondRow;
}

QWidget *TradeWidget::BuildTradeItemWidget()
{
    auto *lReturn = new QWidget;
    auto *lLayout = new QHBoxLayout( lReturn );

    auto *lLb1 = new ElideLabel( mTrade.mItem.c_str() );
    lLayout->addWidget( lLb1 );
    // TODO parse item string for bulk trade or currency

    return lReturn;
}

QWidget *TradeWidget::BuildTradePriceWidget()
{
    auto *lReturn = new QWidget;
    auto *lLayout = new QHBoxLayout( lReturn );

    auto *lLb1 = new ElideLabel( mTrade.mPrice.value_or( "" ).c_str() );
    lLayout->addWidget( lLb1 );
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
