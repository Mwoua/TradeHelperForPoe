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

void PoeCommand( const std::string &aCommand, PoeVersion aVersion )
{
    if( !BringPoeToForeground( aVersion ) )
    {
        return;
    }

    std::vector<INPUT> vec;
    INPUT lEnterInputDown{};
    lEnterInputDown.type     = INPUT_KEYBOARD;
    lEnterInputDown.ki.wVk   = VK_RETURN;
    INPUT lEnterInputUp      = lEnterInputDown;
    lEnterInputUp.ki.dwFlags = KEYEVENTF_KEYUP;
    vec.push_back( lEnterInputDown );
    vec.push_back( lEnterInputUp );
    for( auto ch : aCommand )
    {
        INPUT input      = { 0 };
        input.type       = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        input.ki.wScan   = ch;
        vec.push_back( input );

        input.ki.dwFlags |= KEYEVENTF_KEYUP;
        vec.push_back( input );
    }
    vec.push_back( lEnterInputDown );
    vec.push_back( lEnterInputUp );

    SendInput( vec.size(), vec.data(), sizeof( INPUT ) );
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
void PoeCommand( const std::string &aCommand, PoeVersion aVersion )
{
    // TODO
}
#endif
