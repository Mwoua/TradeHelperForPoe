#pragma once

#include <optional>
#include <string>

enum class PoeVersion
{
    Poe1,
    Poe2
};

enum class Language
{
    English,
    French,
    Spanish,
    German,
    BrazilianPortuguese,
    Russian,
    Thai
};

struct Trade
{
    Language mLanguage                       = Language::English;
    std::string mEntireString                = "";
    bool mIncoming                           = false;
    std::string mItem                        = "";
    std::string mLeague                      = "";
    std::string mUser                        = "";
    std::optional<std::string> mPrice        = std::nullopt;
    std::optional<std::string> mStash        = std::nullopt;
    std::optional<std::string> mPositionLeft = std::nullopt;
    std::optional<std::string> mPositionTop  = std::nullopt;
    std::optional<std::string> mComment      = std::nullopt;
};
