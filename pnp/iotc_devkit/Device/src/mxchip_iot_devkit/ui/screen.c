
#include <stdio.h>
#include <stdlib.h>
#include "wiring.h"
#include "IoT_DevKit_HW.h"
#include "azure_c_shared_utility/singlylinkedlist.h"
#include "screen.h"

void screen_main(void)
{
    textOutDevKitScreen(0, "IoT DevKit\r\nPnP Enabled\r\nRunning...", 0);
    textOutDevKitScreen(3, getIPAddress(), 0);
}

void screen_echo(const char* text)
{
    cleanDevKitScreen();
    textOutDevKitScreen(0, "Echo", 0);
    if (text == NULL || text[0] == 0)
    {
        textOutDevKitScreen(1, "<Empty string>", 1);
    }
    else
    {
        textOutDevKitScreen(1, text, 1);
    }
    delay(3000);
    screen_main();
}