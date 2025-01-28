#include "TradeWidgetFactory.hpp"

#include "TradeWidget.hpp"
#include "TradeWidgetDisplayer.hpp"


void TradeWidgetFactory::Create( TradeWidgetDisplayer &aTradeWidgetDisplayer, Trade aTrade, PoeVersion aVersion )
{
    auto *lTradeWidget = new TradeWidget( std::move( aTrade ), aVersion );
    aTradeWidgetDisplayer.AddTradeWidget( *lTradeWidget );
}
