#pragma once

#include "Defines.hpp"

#include <regex>
#include <string>

class TradeWidgetDisplayer;

/// Parse input string and display relevant widget
class StringParser
{
  public:
    StringParser( TradeWidgetDisplayer &aTradeWidgetDisplayer, std::string aLine, PoeVersion aVersion );

  private:
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

    struct Regex
    {
        std::regex mRegex;
        std::vector<StringParser::Matches> mMatches;
        Language mLanguage;
    };

    static const std::vector<StringParser::Regex> REGEXES;

    void MatchToTradeItem( Trade &aTradeInfo, Matches aMatchType, const std::string &aValue );
};
