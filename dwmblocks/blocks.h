 
//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
    /*Icon*/    /*Command*/     /*Update Interval*/ /*Update Signal*/
    
    {"", "/home/safwan/.config/scripts/dtscripts/kernel",           3600,               2},

    {"", "/home/safwan/.config/scripts/dtscripts/pacupdate",        3600,               0},

    {"", "/home/safwan/.config/scripts/weather",		    1800,		        3},

    {"",    "/home/safwan/.config/scripts/dwmdisk",             3600,                   0},
    
    {"", "/home/safwan/.config/scripts/dtscripts/memory",           6,                  1},

    {"", "/home/safwan/.config/scripts/dtscripts/volume",           2,                  10},

    {"",     "/home/safwan/.config/scripts/dwmcpu",                 2,                  0},

    //{"",     "/home/safwan/.config/scripts/batt",                   10,                 0},

    {"", "/home/safwan/.config/scripts/dtscripts/clock",            5,                  0},


};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " ";
static unsigned int delimLen = 5;
