/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 4;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "UbuntuMono Nerd Font:size=12:antialias=true:autohint=true",
										"fontawesome:size=12",
										"JoyPixels:size=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_gray1[]       = "#23272F"; //"#222222"; //background color
static const char col_gray2[]       = "#444444"; //inactive window border color
static const char col_gray3[]       = "#FFFFFF"; //"#bbbbbb"; // font color
static const char col_gray4[]       = "#6b008f"; //"#eeeeee"; // current tag and current window font color
static const char col_cyan[]        = "#88C0D0"; //"#005577"; // top bar second color(blue) and active window color
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm1", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spterm2", "-g", "120x34", NULL };
const char *spcmd3[] = {"st", "-n", "spterm3", "-g", "120x34", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm1",     spcmd1},
	{"spterm2",    	spcmd2},
	{"spterm3",   	spcmd3},
};

static const char *rofi_menu[]  = { "/home/safwan/.config/scripts/rofimenu", NULL };
static const char *thunar[]  = {"thunar", NULL };

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */ 
	{ "Slack",	  	NULL,			NULL,		1 << 0, 		0,			  1 },
	{ "zoom",  	NULL,			NULL,		1 << 7,			0,			  0 },
	
	// { "VirtualBox Manager",	  	NULL,			NULL,		1 << 7, 		0,			  0 },
	// { "Brave-browser",  	NULL,			NULL,			1 << 0,			0,			  0 },
	{ NULL,		  "spterm1",		NULL,		SPTAG(0),		1,			 -1 },
	{ NULL,		  "spterm2",		NULL,		SPTAG(1),		1,			 -1 },
	{ NULL,		  "spterm3",		NULL,		SPTAG(2),		0,			 -1 },
};
// 0 for primary monitor and 1 for secondary -1 not specified follow focus/

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_m,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_m,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,		                XK_u,      incrgaps,       {.i = +2 } },
	{ MODKEY|ShiftMask, 		    XK_u,      incrgaps,       {.i = -2 } },
	// { MODKEY, 		                XK_i,      incrigaps,      {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_i,      incrigaps,      {.i = -1 } },
	// { MODKEY,                       XK_o,      incrogaps,      {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_o,      incrogaps,      {.i = -1 } },
	// { MODKEY,                       XK_6,      incrihgaps,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_6,      incrihgaps,     {.i = -1 } },
	// { MODKEY,                       XK_7,      incrivgaps,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_7,      incrivgaps,     {.i = -1 } },
	// { MODKEY,                       XK_8,      incrohgaps,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_8,      incrohgaps,     {.i = -1 } },
	// { MODKEY,                       XK_9,      incrovgaps,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_0,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,		                XK_q,      killclient,     {0} },
	{ MODKEY,		            	XK_t,	    setlayout,		{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,				XK_t,		setlayout,		{.v = &layouts[5]} }, /* bstack */
	{ MODKEY,						XK_y,		setlayout,		{.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,				XK_y,		setlayout,		{.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,						XK_e,		setlayout,		{.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,				XK_d,		setlayout,		{.v = &layouts[1]} }, /* monocle */
	{ MODKEY,						XK_x,		setlayout,		{.v = &layouts[7]} }, /* grid */
	{ MODKEY,						XK_w,		setlayout,		{.v = &layouts[11]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,				XK_w,		setlayout,		{.v = &layouts[12]} }, /* centeredfloatingmaster */
	{ MODKEY,						XK_f,		setlayout,		{.v = &layouts[13]} }, /* all float */
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ 0,							XK_F9,		spawn,		   {.v = rofi_menu } },
	{ Mod1Mask,                     XK_Tab,     spawn,         SHCMD("/home/safwan/.config/scripts/rofiwindows") },
	{ Mod1Mask,						XK_Return,		spawn,	    SHCMD("terminator") },
	{ MODKEY|ShiftMask,				XK_x,		    spawn,          SHCMD("betterlockscreen -l") },
	{ ControlMask|Mod1Mask,				XK_t,		spawn,     {.v = thunar } },
	{ ControlMask|Mod1Mask,				XK_b,		spawn,          SHCMD("brave") },
	{ ControlMask|Mod1Mask,				XK_s,		spawn,          SHCMD("com.slack.Slack") },
	{ ControlMask|Mod1Mask,				XK_z,		spawn,          SHCMD("us.zoom.Zoom") },
	{ ControlMask|Mod1Mask,				XK_c,		spawn,          SHCMD("com.giuspen.cherrytree") },
	{ ControlMask|Mod1Mask,				XK_m,		spawn,          SHCMD("mailspring") },
	{ ControlMask|Mod1Mask,				XK_g,		spawn,          SHCMD("google-chrome-stable") },
	{ ControlMask|Mod1Mask,				XK_o,		spawn,          SHCMD("obs") },
	{ ControlMask|Mod1Mask,				XK_f,		spawn,          SHCMD("flameshot gui") },
	{ ControlMask|Mod1Mask,				XK_v,		spawn,          SHCMD("virtualbox") },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn, SHCMD("~/.config/polybar/scripts/pavolume.sh --down") },
	{ 0,                            XF86XK_AudioMute,           spawn, SHCMD("~/.config/polybar/scripts/pavolume.sh --togmute") },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn, SHCMD("~/.config/polybar/scripts/pavolume.sh --up") },
	{ 0, 							XF86XK_MonBrightnessUp,	    spawn,		SHCMD("xbacklight -inc 15") },
	{ 0, 							XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") },
	{ MODKEY|Mod1Mask,    			XK_h,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY|Mod1Mask,            	XK_j,	   togglescratch,  {.ui = 1 } },
	{ MODKEY|Mod1Mask,            	XK_k,	   togglescratch,  {.ui = 2 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_c,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, // click layout symbol
	// { ClkWinTitle,          0,              Button2,        zoom,           {0} },                // click window title zoom mean make master
    { ClkWinTitle,          0,              Button1,        focusstack,     {.i = +1 } },
	{ ClkWinTitle,          0,        	    Button3,        focusstack,     {.i = -1 } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },    // this is the status bar ram date cpu
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },   // the actual window
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },  // tags buttons 
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
