/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */

// sysytray
/* 0 means no systray */
static const int showsystray = 1;
/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayonleft = 0;
/* systray spacing */
static const unsigned int systrayspacing = 2;
/* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systraypinning = 0;
/* 1: if pinning fails, display systray on the first monitor, False: display *
 * systray on the last monitor*/
static const int systraypinningfailfirst = 1;

static const int topbar = 1; /* 0 means bottom bar */
static const char *fonts[] = {"JetBrainsMono Nerd Font:size=12"};
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=12";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[] = "#005577";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

// autostart
static const char *const autostart[] = {
    "flameshot", NULL,
    "dunst",     NULL,
    "nm-applet", NULL,
    "picom",     "-b",
    NULL,        "sh",
    "-c",        "feh --randomize --bg-fill ~/wallpapers/rose-pine/*",
    NULL,        NULL /* terminate */
};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;   /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn",
// dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf",
// col_gray4, NULL };
static const char *dmenucmd[] = {"rofi", "-show", "drun", NULL};
static const char *termcmd[] = {"kitty", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    // rofi
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    // kitty
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    // kill window
    {MODKEY, XK_q, killclient, {0}},

    // bar
    {MODKEY, XK_b, togglebar, {0}},
    // focus
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    // quit
    {MODKEY | ShiftMask, XK_q, quit, {0}},

    // change number of master windows
    {MODKEY | ShiftMask, XK_i, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_d, incnmaster, {.i = -1}},

    // chagne size of master stack
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},

    // swap master with current window
    {MODKEY, XK_space, zoom, {0}},

    // switch to previous tab
    {MODKEY | ShiftMask, XK_Tab, view, {0}},

    // hold mod and use mouse to move windows
    // master layout
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    // floatung layout
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    // tab/window layout
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    // toggle window floating state
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},

    // change layout
    // {MODKEY, XK_space, setlayout, {0}},

    // show all open windows
    {MODKEY, XK_0, view, {.ui = ~0}},
    // reassign to single window
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

    // change monitor focus
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},

    // move window to diffrent monitor
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

    // brightness and audio
    {0, XF86XK_MonBrightnessUp, spawn,
     SHCMD("sh -c /usr/bin/brightnessctl set 10%-; notify-send -t 200 -u low 'Brightness' $(brightnesctl get) "
           "'%')")},
    {0, XF86XK_MonBrightnessDown, spawn,
     SHCMD("sh -c /usr/bin/brightnessctl set 10%-; notify-send -t 200 -u low 'Brightness' $(brightnessctl get) "
           "'%')")},
    {0, XF86XK_AudioLowerVolume, spawn,
     SHCMD("amixer sset Master 5%- unmute; notify-send -t 200 -u low 'Volume' $(amixer get "
           "Master | grep -oP '\\[([0-9]+%)\\]' | head -n 1)")},
    {0, XF86XK_AudioMute, spawn,
     SHCMD("amixer sset Master $(amixer get Master | grep -q '\\[on\\]' && "
           "echo 'mute' || echo 'unmute'); notify-send -t 200 -u low 'Audio' $(amixer get "
           "Master | grep -oP '\\[([a-zA-Z]+)\\]' | head -n 1)")},
    {0, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("amixer sset Master 5%+ unmute; notify-send -t 200 -u low 'Volume' $(amixer get "
           "Master | grep -oP '\\[([0-9]+%)\\]' | head -n 1)")},
    // wallpaper
    {MODKEY | ShiftMask, XK_w, spawn,
     SHCMD("feh --randomize --bg-fill ~/wallpapers/rose-pine/*")},

    // MODKEY + Number: Switch to the corresponding tag.
    // MODKEY + Control + Number: Toggle the tag visibility.
    // MODKEY + Shift + Number: Move the currently focused window to the tag.
    // MODKEY + Control + Shift + Number: Add/remove the focused window to/from
    // the tag without removing it from other tags.
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
};
