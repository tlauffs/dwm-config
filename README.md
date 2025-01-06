# dwm - dynamic window manager
------------
dwm is an extremely fast, small, and dynamic window manager for X.


## Requirements
------------
In order to build dwm you need the Xlib header files.
This guide assumes your system has the latest updates before going ahead with the installation.

> [!NOTE]
> You may want to keep the source directories of the tools you download in a suitable location for future reference as you may need to recompile them to apply configuration changes.

<details>
  <summary>Arch</summary>

  Install dependencies:

  ```sh
  sudo pacman -S --needed base-devel extra/git extra/libx11 extra/libxcb extra/libxinerama extra/libxft extra/imlib2
  ```

  If you find yourself missing a library then this can usually be found by searching for the file name using pacman:

  ```sh
  $ pacman -F Xlib-xcb.h
  extra/libx11 1.6.12-1 [installed: 1.7.2-1]
  usr/include/X11/Xlib-xcb.h
  ```

</details>

<details>
  <summary>Debian/Ubuntu</summary>

  Install dependencies:

  ```sh
  sudo apt install build-essential git libx11-dev libx11-xcb-dev libxcb-res0-dev libxinerama-dev libxft-dev libimlib2-dev
  ```

  It is worth checking the version of gcc on debian based systems as they may come with older implementations that can result in compilation errors.

  ```sh
  gcc --version
  ```

  You would expect at least v8.x or above here.

  If you find yourself missing a library then this can usually be found by searching for the file name using apt-file, a tool that have to be installed separately:

  ```sh
  $ sudo apt install apt-file
  $ sudo apt-file update
  $ apt-file search xcb/res.h
  libxcb-res0-dev: /usr/include/xcb/res.h
  ```

</details>


<details>
  <summary>Void Linux</summary>

  Install dependencies:

  ```sh
  sudo xbps-install -Su base-devel libX11-devel libXft-devel libXinerama-devel freetype-devel fontconfig-devel libxcb-devel imlib2-devel
  ```

  If you find yourself missing a library then this can usually be found by searching for the file name using xlocate, a tool that have to be installed separately via the xtools package:

  ```sh
  $ xlocate yajl/yajl_gen.h
  yajl-devel-2.1.0._4      /usr/include/yajl/yajl_gen.h
  ```

</details>

## Installation
------------
Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install


## Running dwm
-----------
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm

### With Login Manager
- requirnment: xinit installed (maybe not installed on wayland systems)
- Place a dwm.desktop file will be placed in `/usr/share/xsessions/`.
```
    [Desktop Entry]
    Encoding=UTF-8
    Name=dwm
    Comment=Dynamic window manager
    Exec=dwm
    Icon=dwm
    Type=XSession
```
Add `exec dwm` at the end of your `~/.xinitrc` file.

## Without Login Manager

Add `exec dwm` at the end of your `~/.xinitrc` file.
run startx

## Configuration
-------------
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.
