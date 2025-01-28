#include "TradeWidgetDisplayer.hpp"

TradeWidgetDisplayer::TradeWidgetDisplayer()
    : mMainLayout( new QVBoxLayout( this ) )
{
    setWindowFlags( Qt::WindowStaysOnTopHint | Qt::Window | Qt::FramelessWindowHint );
    setLayout( mMainLayout );
}

void TradeWidgetDisplayer::AddTradeWidget( QWidget &aWidget )
{
    mMainLayout->addWidget( &aWidget );
    show();
}
