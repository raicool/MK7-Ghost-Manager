#pragma once

#define STR_EXPAND(x)    #x
#define STR(x)           STR_EXPAND(x)

#define VERSION_MAJOR    0
#define VERSION_MINOR    2
#define VERSION_REVISION 6

#define VERSION          STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_REVISION)