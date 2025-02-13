#pragma once

#include "Defines.hpp"

#include <regex>
#include <string>
#include <unordered_map>

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
    };

    static const std::unordered_map<Language, std::vector<StringParser::Regex>> REGEXES;

    static bool StringToLanguageAndIncoming( const std::string &aInputString, Trade &aTradeInfo );
    static void MatchToTradeItem( Trade &aTradeInfo, Matches aMatchType, const std::string &aValue );
};
