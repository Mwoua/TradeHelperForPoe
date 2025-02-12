#pragma once

#include "Defines.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

constexpr auto POE1_ALCH   = ":/Poe1/alch";
constexpr auto POE1_ANNUL  = ":/Poe1/annul";
constexpr auto POE1_CHAOS  = ":/Poe1/chaos";
constexpr auto POE1_DIV    = ":/Poe1/divine";
constexpr auto POE1_EX     = ":/Poe1/exalted";
constexpr auto POE1_MIRROR = ":/Poe1/mirror";

constexpr auto POE2_ALCH   = ":/Poe2/alch";
constexpr auto POE2_ANNUL  = ":/Poe2/annul";
constexpr auto POE2_CHAOS  = ":/Poe2/chaos";
constexpr auto POE2_DIV    = ":/Poe2/divine";
constexpr auto POE2_EX     = ":/Poe2/exalted";
constexpr auto POE2_MIRROR = ":/Poe2/mirror";

enum class eCurrencies
{
    Undefined,
    alch,
    annul,
    chaos,
    divine,
    exalt,
    mirror
};
constexpr std::string CurrencyToImageString( eCurrencies aCurrency, PoeVersion aPoeVersion )
{
    switch( aCurrency )
    {
    case eCurrencies::alch:
        return aPoeVersion == PoeVersion::Poe1 ? POE1_ALCH : POE2_ALCH;
    case eCurrencies::annul:
        return aPoeVersion == PoeVersion::Poe1 ? POE1_ANNUL : POE2_ANNUL;
    case eCurrencies::chaos:
        return aPoeVersion == PoeVersion::Poe1 ? POE1_CHAOS : POE2_CHAOS;
    case eCurrencies::divine:
        return aPoeVersion == PoeVersion::Poe1 ? POE1_DIV : POE2_DIV;
    case eCurrencies::exalt:
        return aPoeVersion == PoeVersion::Poe1 ? POE1_EX : POE2_EX;
    case eCurrencies::mirror:
        return aPoeVersion == PoeVersion::Poe1 ? POE1_MIRROR : POE2_MIRROR;
    default:
        throw std::invalid_argument( "Unknown currency" );
    }
}
// IMPORTANT NOTE : Currency string must the exact same (case too) that the on sent by the trade website

// For each language, list all relevant currencies and their associated icon
// Note: we can add a lot of the currencies here, but since the exchange market exist, its barely useful

const std::unordered_map<Language, std::unordered_map<std::string, eCurrencies>> CURRENCIES{ { Language::English,
                                                                                               { { "alchemy", eCurrencies::alch },
                                                                                                 { "annulment", eCurrencies::annul },
                                                                                                 { "chaos", eCurrencies::chaos },
                                                                                                 { "divine", eCurrencies::divine },
                                                                                                 { "exalted", eCurrencies::exalt },
                                                                                                 { "mirror", eCurrencies::mirror } } } };
