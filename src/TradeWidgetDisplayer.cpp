#include "TradeWidgetDisplayer.hpp"

#include "Settings.hpp"

#include <QFrame>
#include <QTimer>

TradeWidgetDisplayer::TradeWidgetDisplayer()
    : mMainLayout( new QVBoxLayout( this ) )
{
    setWindowFlags( Qt::WindowStaysOnTopHint | Qt::Window | Qt::FramelessWindowHint );
    setLayout( mMainLayout );
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    mMainLayout->setContentsMargins( 1, 1, 1, 1 );
    mMainLayout->setSpacing( 0 );
    mMainLayout->setSizeConstraint( QLayout::SetFixedSize );

    const int lX = Settings::TradeWidgetX();
    const int lY = Settings::TradeWidgetY();
    move( lX, lY );
}

void TradeWidgetDisplayer::AddTradeWidget( QWidget &aWidget )
{
    QFrame *lLine = nullptr;
    if( mMainLayout->count() > 0 )
    {
        lLine = new QFrame( this );
        lLine->setFrameShape( QFrame::HLine );
        lLine->setFrameShadow( QFrame::Sunken );
        lLine->setLineWidth( 1 );
        mMainLayout->addWidget( lLine );
    }
    mMainLayout->addWidget( &aWidget );
    show();

    connect( &aWidget,
             &QObject::destroyed,
             this,
             [this, lLine]()
             {
                 // Need to be in a single shot so the previous widget is actually deleted and removed from the layout
                 QTimer::singleShot( 0,
                                     this,
                                     [this, lLine]()
                                     {
                                         delete lLine;
                                         if( mMainLayout->count() == 0 )
                                         {
                                             hide();
                                         }
                                         else if( auto *lLineToHide = qobject_cast<QFrame *>( mMainLayout->itemAt( 0 )->widget() ) )
                                         {
                                             lLineToHide->hide();
                                         }
                                     } );
             } );
}
