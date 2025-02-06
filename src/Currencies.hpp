#pragma once

#include "Defines.hpp"

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

// IMPORTANT NOTE : Currency string must the exact same (case too) that the on sent by the trade website

// For each language, list all relevant currencies and their associated icon
// Note: we can add a lot of the currencies here, but since the exchange market exist, its barely useful

const std::unordered_map<Language, std::unordered_map<std::string, std::string>> CURRENCIES_POE1{
    { Language::English,
      { { "chaos", POE1_CHAOS }, { "divine", POE1_DIV }, { "exalted", POE1_EX }, { "mirror", POE1_MIRROR }, { "alchemy", POE1_ALCH }, { "annulment", POE1_ANNUL } } } };

const std::unordered_map<Language, std::unordered_map<std::string, std::string>> CURRENCIES_POE2{
    { Language::English,
      { { "chaos", POE2_CHAOS }, { "divine", POE2_DIV }, { "exalted", POE2_EX }, { "mirror", POE2_MIRROR }, { "alchemy", POE2_ALCH }, { "annulment", POE2_ANNUL } } } };
