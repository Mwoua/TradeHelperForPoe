#include "TradeWidget.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

constexpr auto COLLAPSED_PROPERTY = "CollapseState";
constexpr auto ARROW_DOWN         = "\u2b9f"; // ⮟
constexpr auto ARROW_UP           = "\u2b9d"; // ⮝⮝

TradeWidget::TradeWidget( Trade aTrade, PoeVersion aVersion )
    : mPoeVersion( aVersion )
{
    SetupUi( aTrade );
    setStyleSheet( "QAbstractButton { padding: 1px; }" );
}

void TradeWidget::SetupUi( const Trade &aTrade )
{
    auto *lMainLayout = new QVBoxLayout( this );
    lMainLayout->setContentsMargins( 0, 0, 0, 0 );
    lMainLayout->setSpacing( 1 );

    lMainLayout->addLayout( SetupUiFirstRow( aTrade ) );
    lMainLayout->addWidget( SetupUiSecondRow( aTrade ) );
    lMainLayout->setAlignment( Qt::AlignTop );
}

QLayout *TradeWidget::SetupUiFirstRow( const Trade &aTrade )
{
    auto *lFirstRowLayout = new QHBoxLayout;
    lFirstRowLayout->setContentsMargins( 0, 0, 0, 0 );
    lFirstRowLayout->setSpacing( 1 );
    mCollapseButton = new QPushButton( ARROW_UP );
    mCollapseButton->setProperty( COLLAPSED_PROPERTY, false );
    connect( mCollapseButton, &QPushButton::clicked, this, &TradeWidget::ManageCollaspeState );
    lFirstRowLayout->addWidget( mCollapseButton );
    lFirstRowLayout->addWidget( BuildTradeItemWidget( aTrade ) );
    lFirstRowLayout->addWidget( BuildTradePriceWidget( aTrade ) );
    mLbTime = new QLabel( "00:00" );
    lFirstRowLayout->addWidget( mLbTime );
    mTimer = new QTimer( this );
    mElapsedTimer.start();
    connect( mTimer, &QTimer::timeout, this, &TradeWidget::UpdateTime );
    mTimer->setInterval( 1000 );
    mTimer->start();

    if( aTrade.mIncoming )
    {
        auto *lPBInviteCustomer = new QPushButton( QIcon( ":/TradeHelper/Invite" ), "" );
        lFirstRowLayout->addWidget( lPBInviteCustomer );
        // TODO connect lPBInviteCustomer
    }
    else
    {
        auto *lPBToSellerHideout = new QPushButton( QIcon( ":/TradeHelper/SellerHideout" ), "" );
        lFirstRowLayout->addWidget( lPBToSellerHideout );
        // TODO connect lPBToSellerHideout
    }

    if( aTrade.mIncoming )
    {
        auto *lPBTrade = new QPushButton( QIcon( ":/TradeHelper/Trade" ), "" );
        lFirstRowLayout->addWidget( lPBTrade );
        // TODO connect lPBTrade
    }

    auto *lPBThanks = new QPushButton( QIcon( ":/TradeHelper/Thanks" ), "" );
    lFirstRowLayout->addWidget( lPBThanks );
    // TODO connect lPBThanks

    auto *lPBLeave = new QPushButton( QIcon( ":/TradeHelper/Leave" ), "" );
    lFirstRowLayout->addWidget( lPBLeave );
    // TODO connect lPBLeave

    if( !aTrade.mIncoming )
    {
        auto *lPBHome = new QPushButton( QIcon( ":/TradeHelper/Home" ), "" );
        lFirstRowLayout->addWidget( lPBHome );
        // TODO connect lPBHome
    }

    auto *lPBWhisp = new QPushButton( QIcon( ":/TradeHelper/Whisp" ), "" );
    lFirstRowLayout->addWidget( lPBWhisp );
    // TODO connect lPBWhisp

    auto *lPBClose = new QPushButton( QIcon( ":/TradeHelper/Close" ), "" );
    lFirstRowLayout->addWidget( lPBClose );
    connect( lPBClose, &QPushButton::clicked, this, &QWidget::deleteLater );

    return lFirstRowLayout;
}

QWidget *TradeWidget::SetupUiSecondRow( const Trade &aTrade )
{
    mSecondRow = new QWidget;
    mSecondRow->setContentsMargins( 0, 0, 0, 0 );
    auto *lSecondRowLayout = new QHBoxLayout( mSecondRow );
    lSecondRowLayout->setContentsMargins( 0, 0, 0, 0 );
    lSecondRowLayout->setSpacing( 1 );

    auto *lPBCustomer = new QPushButton( aTrade.mUser.c_str() );
    lPBCustomer->setFlat( true );
    lSecondRowLayout->addWidget( lPBCustomer );
    // TODO connect lPBCustomer

    lSecondRowLayout->addWidget( new QLabel( aTrade.mLeague.c_str() ) );
    if( aTrade.mComment.has_value() )
    {
        lSecondRowLayout->addWidget( new QLabel( aTrade.mComment.value().c_str() ) );
    }
    lSecondRowLayout->addStretch( 100 );

    auto *lPBMessage1 = new QPushButton( QIcon( ":/TradeHelper/Message1" ), "" );
    lSecondRowLayout->addWidget( lPBMessage1 );
    // TODO connect lPBMessage1

    auto *lPBMessage2 = new QPushButton( QIcon( ":/TradeHelper/Message2" ), "" );
    lSecondRowLayout->addWidget( lPBMessage2 );
    // TODO connect lPBMessage2

    if( aTrade.mIncoming )
    {
        auto *lPBWait = new QPushButton( QIcon( ":/TradeHelper/Wait" ), "" );
        lSecondRowLayout->addWidget( lPBWait );
        // TODO connect lPBWait

        auto *lPBSearch = new QPushButton( QIcon( ":/TradeHelper/Search" ), "" );
        lSecondRowLayout->addWidget( lPBSearch );
        // TODO connect lPBSearch
    }

    auto *lPBKick = new QPushButton( QIcon( ":/TradeHelper/Kick" ), "" );
    lSecondRowLayout->addWidget( lPBKick );
    // TODO connect lPBKick

    if( aTrade.mIncoming )
    {
        auto *lPBStillInterested = new QPushButton( QIcon( ":/TradeHelper/StillInterested" ), "" );
        lSecondRowLayout->addWidget( lPBStillInterested );
        // TODO connect lPBStillInterested
    }
    else
    {
        auto *lPBRepeat = new QPushButton( QIcon( ":/TradeHelper/Repeat" ), "" );
        lSecondRowLayout->addWidget( lPBRepeat );
        // TODO connect lPBRepeat
    }

    return mSecondRow;
}

QWidget *TradeWidget::BuildTradeItemWidget( const Trade &aTrade )
{
    auto *lReturn = new QWidget;
    auto *lLayout = new QHBoxLayout( lReturn );

    auto *lLb1 = new QLabel( aTrade.mItem.c_str() );
    lLayout->addWidget( lLb1 );
    // TODO parse item string for bulk trade or currency

    return lReturn;
}

QWidget *TradeWidget::BuildTradePriceWidget( const Trade &aTrade )
{
    auto *lReturn = new QWidget;
    auto *lLayout = new QHBoxLayout( lReturn );

    auto *lLb1 = new QLabel( aTrade.mPrice.value_or( "" ).c_str() );
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
