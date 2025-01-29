#pragma once

#include "Defines.hpp"

#include <string>

void PoeCommand( const std::string &aCommand, PoeVersion aVersion, bool aSendCommand = true );
void PoeCommandSearch( PoeVersion aVersion );