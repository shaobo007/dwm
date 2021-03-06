#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 8;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 8;       /* vert inner gap between windows */
static const unsigned int gappoh    = 8;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 8;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "Hack Nerd Font Mono:size=13" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]        = "#42A5F5";
static const unsigned int baralpha = 0xc8;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
//static const char *tags[] = { "???", "???", "???", "???", "???", "???", "???", "???", "???" };
static const char *tags[] = { "\uf120", "\uf7ae", "\uf121", "\uf04b", "\ue62e", "\uf251", "\ue727", "\uf537", "\uf684" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                   instance    title       tags mask     isfloating   monitor */
	{ "Gimp",                  NULL,       NULL,       0,            1,           -1 },
	//{ "Emulator",              NULL,       NULL,       0,            1,           -1 },
	//{ "google-chrome-stable",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tile",     tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "H[]",      deck },
	{ "TTT",      bstack },
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
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *tabtermcmd[]  = {"tabbed", "-r 2", "st", "-w", "''", NULL};
static const char *browsercmd[]  = { "google-chrome-stable", NULL };
static const char *surfcmd[]  = { "tabbed", "surf", "-e", NULL };
//control the sys volume
static const char *upvol[]   = { "/home/shaobo/scripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/shaobo/scripts/vol-down.sh",  NULL };
static const char *mutevol[] = { "/home/shaobo/scripts/vol-toggle.sh",  NULL };
//restart vpn server
static const char *start_vpn[] = { "/home/shaobo/scripts/bin/vpn",  NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "90x30", NULL };

static Key keys[] = {
	/* modifier            key                      function            argument */
	{ MODKEY,              XK_d,                    spawn,              {.v = dmenucmd } },
	{ MODKEY,              XK_Return,               spawn,              {.v = termcmd } },
	{ MODKEY|Mod1Mask,     XK_Return,               spawn,              {.v = tabtermcmd } },
  { MODKEY,              XK_c,                    spawn,              {.v = browsercmd } },
  { MODKEY|Mod1Mask,     XK_c,                    spawn,              {.v = surfcmd } },
	{ 0,                   XF86XK_AudioLowerVolume, spawn,              {.v = downvol } },
	{ 0,                   XF86XK_AudioMute,        spawn,              {.v = mutevol } },
	{ 0,                   XF86XK_AudioRaiseVolume, spawn,              {.v = upvol   } },
	{ MODKEY,              XK_minus,                spawn,              {.v = downvol } },
	{ MODKEY,              XK_bracketleft,          spawn,              {.v = mutevol } },
	{ MODKEY,              XK_equal,                spawn,              {.v = upvol   } },
	{ MODKEY|ShiftMask,    XK_j,                    rotatestack,        {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_k,                    rotatestack,        {.i = -1 } },
	{ MODKEY,              XK_j,                    focusstack,         {.i = +1 } },
	{ MODKEY,              XK_k,                    focusstack,         {.i = -1 } },
	{ MODKEY,              XK_h,                    viewtoleft,         {0} },
	{ MODKEY,              XK_l,                    viewtoright,        {0} },
	{ MODKEY|ShiftMask,    XK_h,                    tagtoleft,          {0} },
	{ MODKEY|ShiftMask,    XK_l,                    tagtoright,         {0} },
	{ MODKEY|ShiftMask,    XK_u,                    incnmaster,         {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_i,                    incnmaster,         {.i = -1 } },
	{ MODKEY,              XK_u,                    setmfact,           {.f = -0.05 } },
	{ MODKEY,              XK_i,                    setmfact,           {.f = +0.05 } },
  { MODKEY|ControlMask,  XK_u,                    setcfact,           {.f = -0.05} },
  { MODKEY|ControlMask,  XK_i,                    setcfact,           {.f = +0.05} },
  { MODKEY|ControlMask,  XK_o,                    setcfact,           {.f =  0.00} },
	{ MODKEY,              XK_e,                    hidewin,            {0} },
	{ MODKEY|ShiftMask,    XK_e,                    restorewin,         {0} },
	{ MODKEY,              XK_o,                    hideotherwins,      {0}},
	{ MODKEY|ShiftMask,    XK_o,                    restoreotherwins,   {0}},
	{ MODKEY|ShiftMask,    XK_Return,               zoom,               {0} },
	{ MODKEY,              XK_Tab,                  view,               {0} },
	{ MODKEY,              XK_q,                    killclient,         {0} },
	{ MODKEY|ShiftMask,    XK_t,                    setlayout,          {.v = &layouts[0]} }, /* Tile */
	{ MODKEY|ShiftMask,    XK_r,                    setlayout,          {.v = &layouts[1]} }, /* monocle "[M]" */
	{ MODKEY|ShiftMask,    XK_y,                    setlayout,          {.v = &layouts[2]} }, /* spiral "[@]"" (fibonacci)i */
	{ MODKEY|ShiftMask,    XK_d,                    setlayout,          {.v = &layouts[3]} }, /* deck "H[]"" */
	{ MODKEY|ShiftMask,    XK_b,                    setlayout,          {.v = &layouts[4]} }, /* bstack "TTT" */
	{ MODKEY|ShiftMask,    XK_c,                    setlayout,          {.v = &layouts[5]} }, /* centeredmaster  "|M|"*/
	{ MODKEY|ShiftMask,    XK_f,                    setlayout,          {.v = &layouts[6]} }, /* centeredfloatingmaster ">M>" */
	{ MODKEY,              XK_f,                    fullscreen,         {0} },
	{ MODKEY,              XK_space,                setlayout,          {0} },
	{ MODKEY|ShiftMask,    XK_space,                togglefloating,     {0} },
	{ MODKEY,              XK_grave,                togglescratch,      {.v = scratchpadcmd } },
	{ MODKEY,              XK_0,                    view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                    tag,                {.ui = ~0 } },
	{ MODKEY,              XK_comma,                focusmon,           {.i = -1 } },
	{ MODKEY,              XK_period,               focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,                tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,               tagmon,             {.i = +1 } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ MODKEY|ControlMask,  XK_q,                    quit,               {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

