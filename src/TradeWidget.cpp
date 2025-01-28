#include "TradeWidget.hpp"

#include <QLabel>
#include <QVBoxLayout>

TradeWidget::TradeWidget( Trade aTrade, PoeVersion aVersion )
{
    auto *lMainLayout = new QVBoxLayout( this );
    lMainLayout->addWidget( new QLabel( "pouet" ) );
}
