#pragma once

#include "resource.h"
#include "logging.h"

void loadDefaultForwards(void); // Loads the default portforward.config.txt file if it exists
void loadForwardingFile(TCHAR *filename);
void betterCloseSocket(SOCKET s);