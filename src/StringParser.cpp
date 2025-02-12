#include "StringParser.hpp"

#include "TradeWidgetFactory.hpp"

#include <regex>

const std::unordered_map<Language, std::vector<StringParser::Regex>> StringParser::REGEXES = {
    { Language::English,
      { { .mRegex = std::regex( R"((\S+) (.+): Hi, I would like to buy your (.*) listed for (.*) in (.*) [(]stash tab \"(.*)[\"]; position: left ([0-9]*), top ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::FromTo,
                        StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::Price,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((\S+) (.+): Hi, I would like to buy your (.*) listed for (.*) in (.*))" ),
          .mMatches = { StringParser::Matches::FromTo, StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } },
        { .mRegex   = std::regex( R"((\S+) (.+): Hi, I would like to buy your (.*) in (.*) [(]stash tab \"(.*)[\"]; position: left ([0-9]*), top ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::FromTo,
                        StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((\S+) (.+): Hi, I'd like to buy your (.*) for my (.*) in (.*))" ),
          .mMatches = {
              StringParser::Matches::FromTo, StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } } } } };

//// Portuguese Trades
// std::regex poeTradeRegexPOR =
//     std::regex( "Olá, eu gostaria de comprar seu (.*) listado por (.*) na (.*) [(]aba do baú: \"(.*)[\"]; posição: esquerda ([0-9]*), topo ([0-9]*)[)](.*)" );
// std::regex poeTradeNoLocationRegexPOR = std::regex( "@(.*) Olá, eu gostaria de comprar seu (.*) listado por (.*) na (.*)" );
// std::regex poeTradeUnpricedRegexPOR =
//     std::regex( "Olá, eu gostaria de comprar o seu item (.*) na (.*) [(]aba do baú: \"(.*)[\"]; posição: esquerda ([0-9]*), topo ([0-9]*)[)](.*)" );
// std::regex poeTradeCurrencyRegexPOR = std::regex( "Olá, eu gostaria de comprar seu (.*) por meu(s) (.*) na (.*).(.*)" );
//// Russian Trades
// std::regex poeTradeRegexRUS =
//     std::regex( "Здравствуйте, хочу купить у вас (.*) за (.*) в лиге (.*) [(]секция \"(.*)[\"]; позиция: ([0-9]*) столбец, ([0-9]*) ряд[)](.*)" );
// std::regex poeTradeNoLocationRegexRUS = std::regex( "Здравствуйте, хочу купить у вас (.*) за (.*) в лиге (.*)" );
// std::regex poeTradeUnpricedRegexRUS =
//     std::regex( "Здравствуйте, хочу купить у вас (.*) в лиге (.*) [(]секция \"(.*)[\"]; позиция: ([0-9]*) столбец, ([0-9]*) ряд[)](.*)" );
// std::regex poeTradeCurrencyRegexRUS = std::regex( "Здравствуйте, хочу купить у вас (.*) за (.*) в лиге (.*).(.*)" );
//// Thai Trades
// std::regex poeTradeRegexTHA = std::regex( "สวัสดี เราต้องการซื้อ (.*) ที่คุณตั้งขายไว้ในราคา (.*) ในลีก (.*) [(]แท็บ \"(.*)[\"] ตำแหน่ง: ซ้าย ([0-9]*), บน ([0-9]*)[)](.*)" );
// std::regex poeTradeNoLocationRegexTHA = std::regex( "สวัสดี เราต้องการซื้อ (.*) ที่คุณตั้งขายไว้ในราคา (.*) ในลีก (.*)" );
// std::regex poeTradeUnpricedRegexTHA = std::regex( "สวัสดี เราต้องการซื้อ (.*) ที่คุณตั้งขายไว้ในลีก (.*) [(]แท็บ \"(.*)[\"] ตำแหน่ง: ซ้าย ([0-9]*), บน ([0-9]*)[)](.*)" );
// std::regex poeTradeCurrencyRegexTHA = std::regex( "สวัสดี เราต้องการแลก (.*) ของเรากับ (.*) ของคุณในลีก (.*).(.*)" );
//// German Trades
// std::regex poeTradeRegexGER = std::regex(
//     "Hi, ich möchte '(.*)' zum angebotenen Preis von (.*) in der (.*)-Liga kaufen [(]Truhenfach \"(.*)[\"]; Position: ([0-9]*) von links, ([0-9]*) von oben[)](.*)"
//     );
// std::regex poeTradeNoLocationRegexGER = std::regex( "Hi, ich möchte '(.*)' zum angebotenen Preis von (.*) in der (.*)-Liga kaufen" );
// std::regex poeTradeUnpricedRegexGER =
//     std::regex( "Hi, ich möchte '(.*)' in der (.*)-Liga kaufen [(]Truhenfach \"(.*)[\"]; Position: ([0-9]*) von links, ([0-9]*) von oben[)](.*)" );
// std::regex poeTradeCurrencyRegexGER = std::regex( "Hi, ich möchte dein (.*) für mein (.*) in der (.*)-Liga kaufen(.*)" );
//// French Trades
// std::regex poeTradeRegexFRE = std::regex( "Bonjour, je souhaiterais t'acheter (.*) pour (.*) dans la ligue (.*) [(]onglet de réserve \"(.*)[\"] ; ([0-9]*)e en
// partant "
//                                           "de la gauche, ([0-9]*)e en partant du haut[)](.*)" );
// std::regex poeTradeNoLocationRegexFRE = std::regex( "Bonjour, je souhaiterais t'acheter (.*) pour (.*) dans la ligue (.*)" );
// std::regex poeTradeUnpricedRegexFRE = std::regex( "Bonjour, je souhaiterais t'acheter (.*) dans la ligue (.*) [(]onglet de réserve \"(.*)[\"] ; ([0-9]*)e en partant
// "
//                                                   "de la gauche, ([0-9]*)e en partant du haut[)](.*)" );
// std::regex poeTradeCurrencyRegexFRE = std::regex( "Salut, je voudrais t'acheter (.*) contre (.*) [(]ligue (.*)[)](.*)" );
//// Spanish Trades
// std::regex poeTradeRegexSPA =
//     std::regex( "Hola, quisiera comprar tu (.*) listado por (.*) en (.*) [(]pestaña de alijo \"(.*)[\"]; posición: izquierda ([0-9]*), arriba ([0-9]*)[)](.*)" );
// std::regex poeTradeNoLocationRegexSPA = std::regex( "Hola, quisiera comprar tu (.*) listado por (.*) en (.*)" );
// std::regex poeTradeUnpricedRegexSPA =
//     std::regex( "Hola, quisiera comprar tu (.*) en (.*) [(]pestaña de alijo \"(.*)[\"]; posición: izquierda ([0-9]*), arriba ([0-9]*)[)](.*)" );
// std::regex poeTradeCurrencyRegexSPA = std::regex( "Hola, quiero comprar tu[(]s[)] (.*) por mi[(]s[)] (.*) en la liga (.*).(.*)" );
//// Japanese Trades ; Add exception for group order as league is first
// std::regex poeTradeRegexJAP = std::regex(
//     "こんにちは、(.*) リーグで (.*) で売っている、あなたの (.*) を購入したいです [(]スタッシュタブ \"(.*)[\"]; 位置: 左から ([0-9]*), 上から ([0-9]*)[)](.*)" );
// std::regex poeTradeNoLocationRegexJAP = std::regex( "こんにちは、(.*) リーグで (.*) で売っている、あなたの (.*) を購入したいです" );
// std::regex poeTradeUnpricedRegexJAP =
//     std::regex( "こんにちは、(.*) リーグであなたの (.*) を購入したいです [(]スタッシュタブ \"(.*)[\"]; 位置: 左から ([0-9]*), 上から ([0-9]*)[)](.*)" );
// std::regex poeTradeCurrencyRegexJAP = std::regex( "こんにちは、私は(.*)であなたの(.*)を私の(.*)で購入したいです。(.*)" );
//// Korean Trades ; Add exception for group order as league is second and price first
// std::regex poeTradeRegexKOR =
//     std::regex( "안녕하세요, (.*) 올려놓은 (.*) 리그의 (.*)[(]을[)]를 구매하고 싶습니다 [(]보관함 탭 \"(.*)[\"], 위치: 왼쪽 ([0-9]*), 상단 ([0-9]*)[)](.*)" );
// std::regex poeTradeNoLocationRegexKOR = std::regex( "안녕하세요, (.*) 올려놓은 (.*) 리그의 (.*)[(]을[)]를 구매하고 싶습니다" );
// std::regex poeTradeUnpricedRegexKOR =
//     std::regex( "안녕하세요, (.*) 리그의 (.*)[(]을[)]를 구매하고 싶습니다 [(]보관함 탭 \"(.*)[\"], 위치: 왼쪽 ([0-9]*), 상단 ([0-9]*)[)](.*)" );
// std::regex poeTradeCurrencyRegexKOR = std::regex( "안녕하세요, (.*) 리그의 (.*)[(]을[)]를 (.*)[(]으[)]로 구매하고 싶습니다(.*)" );

// poe1
//  2024/09/10 23:16:05 1911664312 cff94598 [INFO Client 7976] @From
//  PurpleRainn: Hi, I would like to buy your level 20 20% Power Charge On Critical Support listed for 38 chaos in Settlers (stash tab "¤¤";
//  position: left 1, top 3)
//  2025/01/23 17:00:49 683015812 3ef2336b [INFO Client 6548] @From
//  Navarraso: Hi, I would like to buy your Heavy Belt listed for 1 exalted in Standard (stash tab "~price 1 exalted"; position: left 1, top 6)
//  poe2
//

StringParser::StringParser( TradeWidgetDisplayer &aTradeWidgetDisplayer, std::string aLine, PoeVersion aVersion )
{
    if( auto lPosAt = aLine.find( " @" ); lPosAt != std::string::npos )
    {
        aLine = aLine.substr( lPosAt + 2 );
    }
    else
    {
        return;
    }

    Trade lTrade;
    if( !StringToLanguageAndIncoming( aLine, lTrade ) )
    {
        return;
    }

    std::smatch lMatches;
    try
    {
        for( const auto &lRegexPair : REGEXES.at( lTrade.mLanguage ) )
        {
            if( std::regex_match( aLine, lMatches, lRegexPair.mRegex ) )
            {
                if( ( lMatches.size() - 1 ) != lRegexPair.mMatches.size() )
                {
                    // Robustness, should not happen
                    return;
                }
                lTrade.mEntireString = lMatches[0];
                lTrade.mEntireString.erase( 0, lTrade.mEntireString.find( ':' ) + 2 );
                for( auto i = 0U; i < lMatches.size() - 1; ++i )
                {
                    MatchToTradeItem( lTrade, lRegexPair.mMatches[i], lMatches[i + 1].str() );
                }
                TradeWidgetFactory::Create( aTradeWidgetDisplayer, std::move( lTrade ), aVersion );
                return;
            }
        }
    }
    catch( const std::exception & )
    {
        // log error?
    }
}
bool StringParser::StringToLanguageAndIncoming( const std::string &aInputString, Trade &aTradeInfo )
{
    if( aInputString.starts_with( "From" ) )
    {
        aTradeInfo.mLanguage = Language::English;
        aTradeInfo.mIncoming = true;
    }
    else if( auto lPosTo = aInputString.find( "To" ); lPosTo != std::string::npos )
    {
        aTradeInfo.mLanguage = Language::English;
        aTradeInfo.mIncoming = false;
    }
    else
    {
        return false;
    }
    return true;
}

void StringParser::MatchToTradeItem( Trade &aTradeInfo, Matches aMatchType, const std::string &aValue )
{
    switch( aMatchType )
    {
    case StringParser::Matches::Item:
        aTradeInfo.mItem = aValue;
        break;
    case StringParser::Matches::User:
        aTradeInfo.mUser = aValue;
        break;
    case StringParser::Matches::Price:
        aTradeInfo.mPrice = aValue;
        break;
    case StringParser::Matches::League:
        aTradeInfo.mLeague = aValue;
        break;
    case StringParser::Matches::Stash:
        aTradeInfo.mStash = aValue;
        break;
    case StringParser::Matches::PositionLeft:
        aTradeInfo.mPositionLeft = aValue;
        break;
    case StringParser::Matches::PositionTop:
        aTradeInfo.mPositionTop = aValue;
        break;
    case StringParser::Matches::Comment:
        aTradeInfo.mComment = aValue;
        break;
    default:
        break;
    }
}
