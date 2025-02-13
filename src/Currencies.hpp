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

// Note: we can add a lot of the currencies here, but since the exchange market exist, its barely useful
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
// Minor note: for english, some trade are sent with full name some with short name
const std::unordered_map<Language, std::unordered_map<std::string, eCurrencies>> CURRENCIES{ { Language::English,
                                                                                               { { "alch", eCurrencies::alch },
                                                                                                 { "Orb of Alchemy", eCurrencies::alch },
                                                                                                 { "annul", eCurrencies::annul },
                                                                                                 { "Orb of Annulment", eCurrencies::annul },
                                                                                                 { "chaos", eCurrencies::chaos },
                                                                                                 { "Chaos Orb", eCurrencies::chaos },
                                                                                                 { "divine", eCurrencies::divine },
                                                                                                 { "Divine Orb", eCurrencies::divine },
                                                                                                 { "exalted", eCurrencies::exalt },
                                                                                                 { "Exalted Orb", eCurrencies::exalt },
                                                                                                 { "mirror", eCurrencies::mirror },
                                                                                                 { "Mirror of Kalandra", eCurrencies::mirror } } },
                                                                                             { Language::French,
                                                                                               { { "Orbe d'alchimie", eCurrencies::alch },
                                                                                                 { "Orbe d'annulation", eCurrencies::annul },
                                                                                                 { "Orbe du chaos", eCurrencies::chaos },
                                                                                                 { "Orbe divin", eCurrencies::divine },
                                                                                                 { "Orbe exalté", eCurrencies::exalt },
                                                                                                 { "Miroir de Kalandra", eCurrencies::mirror } } },
                                                                                             { Language::Spanish,
                                                                                               { { "Orbe de alquimia", eCurrencies::alch },
                                                                                                 { "Orbe de anulación", eCurrencies::annul },
                                                                                                 { "Orbe de caos", eCurrencies::chaos },
                                                                                                 { "Orbe divino", eCurrencies::divine },
                                                                                                 { "Orbe exaltado", eCurrencies::exalt },
                                                                                                 { "Espejo de Kalandra", eCurrencies::mirror } } },
                                                                                             { Language::German,
                                                                                               { { "Sphäre der Alchemie", eCurrencies::alch },
                                                                                                 { "Sphäre der Annullierung", eCurrencies::annul },
                                                                                                 { "Chaossphäre", eCurrencies::chaos },
                                                                                                 { "Göttliche Sphäre", eCurrencies::divine },
                                                                                                 { "Erhabene Sphäre", eCurrencies::exalt },
                                                                                                 { "Spiegel von Kalandra", eCurrencies::mirror } } } };
