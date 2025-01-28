#pragma once

#include "Defines.hpp"

#include <string>

class TradeWidgetDisplayer;

/// Parse input string and display relevant widget
class StringParser
{
  public:
    StringParser( TradeWidgetDisplayer &aTradeWidgetDisplayer, std::string aLine, PoeVersion aVersion );

    enum class Matches
    {
        Item,
        User,
        Price,
        League,
        Stash,
        PositionLeft,
        PositionTop,
        Comment
    };

  private:
    void MatchToTradeItem( Trade &aTradeInfo, Matches aMatchType, const std::string &aValue );
};
