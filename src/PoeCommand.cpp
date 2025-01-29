#include "PoeCommand.hpp"

#if defined( _WIN32 ) || defined( _WIN64 )
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include "windows.h"

#include <vector>

namespace
{
    // Local Private functions
    bool BringPoeToForeground( PoeVersion aVersion );
} // namespace

void PoeCommand( const std::string &aCommand, PoeVersion aVersion, bool aSendCommand )
{
    if( !BringPoeToForeground( aVersion ) )
    {
        return;
    }
    // TODO convert utf8 command to unicode https://stackoverflow.com/a/59730985/1156580

    std::vector<INPUT> lInputs;
    INPUT lEnterInputDown{};
    lEnterInputDown.type     = INPUT_KEYBOARD;
    lEnterInputDown.ki.wVk   = VK_RETURN;
    INPUT lEnterInputUp      = lEnterInputDown;
    lEnterInputUp.ki.dwFlags = KEYEVENTF_KEYUP;
    lInputs.push_back( lEnterInputDown );
    lInputs.push_back( lEnterInputUp );
    for( auto ch : aCommand )
    {
        INPUT input      = { 0 };
        input.type       = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        input.ki.wScan   = ch;
        lInputs.push_back( input );

        input.ki.dwFlags |= KEYEVENTF_KEYUP;
        lInputs.push_back( input );
    }
    if( aSendCommand )
    {
        lInputs.push_back( lEnterInputDown );
        lInputs.push_back( lEnterInputUp );
    }

    SendInput( lInputs.size(), lInputs.data(), sizeof( INPUT ) );
}

void PoeCommandSearch( PoeVersion aVersion )
{
    if( !BringPoeToForeground( aVersion ) )
    {
        return;
    }

    std::vector<INPUT> lInputs( 4, {} );
    lInputs[0].type   = INPUT_KEYBOARD;
    lInputs[0].ki.wVk = VK_CONTROL;
    lInputs[1].type   = INPUT_KEYBOARD;
    lInputs[1].ki.wVk = 'F';
    lInputs[2]        = lInputs[1];
    lInputs[2].ki.dwFlags |= KEYEVENTF_KEYUP;
    lInputs[3] = lInputs[0];
    lInputs[3].ki.dwFlags |= KEYEVENTF_KEYUP;

    SendInput( lInputs.size(), lInputs.data(), sizeof( INPUT ) );
}

namespace
{
    bool BringPoeToForeground( PoeVersion aVersion )
    {
#ifdef _DEBUG
        if( auto lHandle = FindWindow( NULL, R"(C:\Users\User1217\Desktop\TradeHelperForPoe\Client1.txt - Notepad++)" ) )
#else
        if( auto lHandle = FindWindow( NULL, ( aVersion == PoeVersion::Poe1 ) ? "Path of Exile" : "Path of Exile 2" ) )
#endif
        {
            return SetForegroundWindow( lHandle );
        }
        else
        {
            return false;
        }
    }
} // namespace
#else
void PoeCommand( const std::string &aCommand, PoeVersion aVersion, bool aSendCommand )
{
    // TODO
}
void PoeCommandSearch( PoeVersion aVersion ) {}
#endif
