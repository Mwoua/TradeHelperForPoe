#include "PoeCommand.hpp"

#if defined( _WIN32 ) || defined( _WIN64 )
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include "windows.h"

#include <thread>
#include <vector>

void PoeCommand( const std::string &aCommand, PoeVersion aVersion, bool aSendCommand )
{
    if( !BringPoeToForeground( aVersion ) )
    {
        return;
    }

    std::vector<INPUT> lInputs;
    INPUT lEnterInputDown{};
    lEnterInputDown.type     = INPUT_KEYBOARD;
    lEnterInputDown.ki.wVk   = VK_RETURN;
    INPUT lEnterInputUp      = lEnterInputDown;
    lEnterInputUp.ki.dwFlags = KEYEVENTF_KEYUP;
    lInputs.push_back( lEnterInputDown );
    lInputs.push_back( lEnterInputUp );

    if( const int lSize = MultiByteToWideChar( CP_UTF8, 0, aCommand.c_str(), aCommand.size(), nullptr, 0 ); lSize <= 0 )
    {
        // failed conversion, fallback to ascii
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
    }
    else
    {
        std::wstring lUTF16String;
        lUTF16String.resize( lSize );
        MultiByteToWideChar( CP_UTF8, 0, aCommand.c_str(), aCommand.size(), lUTF16String.data(), lUTF16String.size() );
        for( auto ch : lUTF16String )
        {
            INPUT input      = { 0 };
            input.type       = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_UNICODE;
            input.ki.wScan   = ch;
            lInputs.push_back( input );

            input.ki.dwFlags |= KEYEVENTF_KEYUP;
            lInputs.push_back( input );
        }
    }
    if( aSendCommand )
    {
        lInputs.push_back( lEnterInputDown );
        lInputs.push_back( lEnterInputUp );
    }

    SendInput( lInputs.size(), lInputs.data(), sizeof( INPUT ) );
}

void PoeCommandSearch( PoeVersion aVersion, const std::string &aToSearch )
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

    using namespace std::chrono_literals;
    std::this_thread::sleep_for( 500ms );

    lInputs.clear();
    for( auto ch : aToSearch )
    {
        INPUT input      = { 0 };
        input.type       = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        input.ki.wScan   = ch;
        lInputs.push_back( input );

        input.ki.dwFlags |= KEYEVENTF_KEYUP;
        lInputs.push_back( input );
    }
    SendInput( lInputs.size(), lInputs.data(), sizeof( INPUT ) );
}

bool BringPoeToForeground( PoeVersion aVersion )
{
    if( auto lHandle = FindWindow( NULL, ( aVersion == PoeVersion::Poe1 ) ? "Path of Exile" : "Path of Exile 2" ) )
    {
        return SetForegroundWindow( lHandle );
    }
    else
    {
        return false;
    }
}

#else
void PoeCommand( const std::string &aCommand, PoeVersion aVersion, bool aSendCommand )
{
    // TODO
}
void PoeCommandSearch( PoeVersion aVersion, const std::string &aToSearch ) {}

bool BringPoeToForeground( PoeVersion aVersion ) { return false; }
#endif
