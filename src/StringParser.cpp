#include "StringParser.hpp"

#include "TradeWidgetFactory.hpp"

#include <regex>

const std::unordered_map<Language, std::vector<StringParser::Regex>> StringParser::REGEXES = {
    { Language::English,
      { { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Hi, I would like to buy your (.*) listed for (.*) in (.*) [(]stash tab \"(.*)[\"]; position: left ([0-9]*), top ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::Price,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Hi, I would like to buy your (.*) listed for (.*) in (.*))" ),
          .mMatches = { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Hi, I would like to buy your (.*) in (.*) [(]stash tab \"(.*)[\"]; position: left ([0-9]*), top ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Hi, I'd like to buy your (.*) for my (.*) in (.*))" ),
          .mMatches = { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } } } },
    { Language::French,
      { { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Bonjour, je souhaiterais t'acheter (.*) pour (.*) dans la ligue (.*) [(]onglet de réserve \"(.*)[\"] ; ([0-9]*)e en partant de la gauche, ([0-9]*)e en partant du haut[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::Price,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Bonjour, je souhaiterais t'acheter (.*) pour (.*) dans la ligue (.*))" ),
          .mMatches = { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } },
        { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Bonjour, je souhaiterais t'acheter (.*) dans la ligue (.*) [(]onglet de réserve \"(.*)[\"] ; ([0-9]*)e en partant de la gauche, ([0-9]*)e en partant du haut[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex = std::regex( R"((?:<.+> )?(.+): Salut, je voudrais t'acheter (.*) contre (.*) [(]ligue (.*)[)](.*))" ),
          .mMatches =
              { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League, StringParser::Matches::Comment } } } },
    { Language::Spanish,
      { { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Hola, quisiera comprar tu (.*) listado por (.*) en (.*) [(]pestaña de alijo \"(.*)[\"]; posición: izquierda ([0-9]*), arriba ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::Price,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Hola, quisiera comprar tu (.*) listado por (.*) en (.*))" ),
          .mMatches = { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } },
        { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Hola, quisiera comprar tu (.*) en (.*) [(]pestaña de alijo \"(.*)[\"]; posición: izquierda ([0-9]*), arriba ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex = std::regex( R"((?:<.+> )?(.+): Hola, quiero comprar tu[(]s[)] (.*) por mi[(]s[)] (.*) en la liga (.*).(.*))" ),
          .mMatches =
              { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League, StringParser::Matches::Comment } } } },
    { Language::German,
      { { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Hi, ich möchte '(.*)' zum angebotenen Preis von (.*) in der (.*)-Liga kaufen [(]Truhenfach \"(.*)[\"]; Position: ([0-9]*) von links, ([0-9]*) von oben[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::Price,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Hi, ich möchte '(.*)' zum angebotenen Preis von (.*) in der (.*)-Liga kaufen)" ),
          .mMatches = { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } },
        { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Hi, ich möchte '(.*)' in der (.*)-Liga kaufen [(]Truhenfach \"(.*)[\"]; Position: ([0-9]*) von links, ([0-9]*) von oben[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex = std::regex( R"((?:<.+> )?(.+): Hi, ich möchte dein (.*) für mein (.*) in der (.*)-Liga kaufen(.*))" ),
          .mMatches =
              { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League, StringParser::Matches::Comment } } } },
    { Language::BrazilianPortuguese,
      { { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Olá, eu gostaria de comprar seu (.*) listado por (.*) na (.*) [(]aba do baú: \"(.*)[\"]; posição: esquerda ([0-9]*), topo ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::Price,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Olá, eu gostaria de comprar seu (.*) listado por (.*) na (.*))" ),
          .mMatches = { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } },
        { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Olá, eu gostaria de comprar o seu item (.*) na (.*) [(]aba do baú: \"(.*)[\"]; posição: esquerda ([0-9]*), topo ([0-9]*)[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex = std::regex( R"((?:<.+> )?(.+): Olá, eu gostaria de comprar seu (.*) por meu\(s\) (.*) na (.*).(.*))" ),
          .mMatches =
              { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League, StringParser::Matches::Comment } } } },
    { Language::Russian,
      { { .mRegex = std::regex(
              R"((?:<.+> )?(.+): Здравствуйте, хочу купить у вас (.*) за (.*) в лиге (.*) [(]секция \"(.*)[\"]; позиция: ([0-9]*) столбец, ([0-9]*) ряд[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::Price,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Здравствуйте, хочу купить у вас (.*) за (.*) в лиге (.*))" ),
          .mMatches = { StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League } },
        { .mRegex =
              std::regex( R"((?:<.+> )?(.+): Здравствуйте, хочу купить у вас (.*) в лиге (.*) [(]секция \"(.*)[\"]; позиция: ([0-9]*) столбец, ([0-9]*) ряд[)](.*))" ),
          .mMatches = { StringParser::Matches::User,
                        StringParser::Matches::Item,
                        StringParser::Matches::League,
                        StringParser::Matches::Stash,
                        StringParser::Matches::PositionLeft,
                        StringParser::Matches::PositionTop,
                        StringParser::Matches::Comment } },
        { .mRegex   = std::regex( R"((?:<.+> )?(.+): Здравствуйте, хочу купить у вас (.*) за (.*) в лиге (.*).(.*))" ),
          .mMatches = {
              StringParser::Matches::User, StringParser::Matches::Item, StringParser::Matches::Price, StringParser::Matches::League, StringParser::Matches::Comment } } } } };

//// Thai Trades
// std::regex poeTradeRegexTHA = std::regex( "สวัสดี เราต้องการซื้อ (.*) ที่คุณตั้งขายไว้ในราคา (.*) ในลีก (.*) [(]แท็บ \"(.*)[\"] ตำแหน่ง: ซ้าย ([0-9]*), บน ([0-9]*)[)](.*)" );
// std::regex poeTradeNoLocationRegexTHA = std::regex( "สวัสดี เราต้องการซื้อ (.*) ที่คุณตั้งขายไว้ในราคา (.*) ในลีก (.*)" );
// std::regex poeTradeUnpricedRegexTHA = std::regex( "สวัสดี เราต้องการซื้อ (.*) ที่คุณตั้งขายไว้ในลีก (.*) [(]แท็บ \"(.*)[\"] ตำแหน่ง: ซ้าย ([0-9]*), บน ([0-9]*)[)](.*)" );
// std::regex poeTradeCurrencyRegexTHA = std::regex( "สวัสดี เราต้องการแลก (.*) ของเรากับ (.*) ของคุณในลีก (.*).(.*)" );
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
    if( !StringToIncoming( aLine, lTrade ) )
    {
        return;
    }

    std::smatch lMatches;
    try
    {
        for( auto const &[lLang, lRegexes] : REGEXES )
        {
            for( const auto &lRegexInfo : lRegexes )
            {
                if( std::regex_match( aLine, lMatches, lRegexInfo.mRegex ) )
                {
                    if( ( lMatches.size() - 1 ) != lRegexInfo.mMatches.size() )
                    {
                        // Robustness, should not happen
                        return;
                    }
                    lTrade.mLanguage     = lLang;
                    lTrade.mEntireString = lMatches[0];
                    lTrade.mEntireString.erase( 0, lTrade.mEntireString.find( ':' ) + 2 );
                    for( auto i = 0U; i < lMatches.size() - 1; ++i )
                    {
                        MatchToTradeItem( lTrade, lRegexInfo.mMatches[i], lMatches[i + 1].str() );
                    }
                    TradeWidgetFactory::Create( aTradeWidgetDisplayer, std::move( lTrade ), aVersion );
                    return;
                }
            }
        }
    }
    catch( const std::exception & )
    {
        // log error?
    }
}
bool StringParser::StringToIncoming( std::string &aInputString, Trade &aTradeInfo )
{
    auto CheckStart = []( std::string &aInputString, const std::string &aToFind )
    {
        if( aInputString.starts_with( aToFind ) )
        {
            aInputString = aInputString.substr( aToFind.size() + 1);
            return true;
        }
        else
        {
            return false;
        }
    };

    if( CheckStart(aInputString,  "From" ) )
    {
        aTradeInfo.mIncoming = true;
    }
    else if( CheckStart(aInputString,  "To" ) )
    {
        aTradeInfo.mIncoming = false;
    }
    else if( CheckStart(aInputString,  "De" ) )
    {
        aTradeInfo.mIncoming = true;
    }
    else if( CheckStart(aInputString,  "À" ) )
    {
        aTradeInfo.mIncoming = false;
    }
    else if( CheckStart(aInputString,  "Para" ) )
    {
        aTradeInfo.mIncoming = false;
    }
    else if( CheckStart(aInputString,  "An" ) )
    {
        aTradeInfo.mIncoming = false;
    }
    else if( CheckStart(aInputString,  "Von" ) )
    {
        aTradeInfo.mIncoming = true;
    }
    else if( CheckStart(aInputString,  "Кому" ) )
    {
        aTradeInfo.mIncoming = false;
    }
    else if( CheckStart(aInputString,  "От кого" ) )
    {
        aTradeInfo.mIncoming = true;
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
