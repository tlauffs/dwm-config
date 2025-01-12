#!/bin/bash

# requirnments
sudo apt update
sudo apt install -y libconfig-dev libdbus-1-dev libegl-dev libev-dev libgl-dev libepoxy-dev libpcre2-dev libpixman-1-dev libx11-xcb-dev libxcb1-dev libxcb-composite0-dev libxcb-damage0-dev libxcb-dpms0-dev libxcb-glx0-dev libxcb-image0-dev libxcb-present-dev libxcb-randr0-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-shape0-dev libxcb-util-dev libxcb-xfixes0-dev libxext-dev meson ninja-build uthash-dev cmake libxft-dev libimlib2-dev libxinerama-dev libxcb-res0-dev alsa-utils

# programs
sudo apt install rofi kitty network-manager dunst feh flameshot brightnessctl pavucontrol lxappearance -y

# build
sudo make clean install

# picom
    mkdir -p ~/build
    if [ ! -d ~/build/picom ]; then
        if ! git clone https://github.com/FT-Labs/picom.git ~/build/picom; then
            echo "Failed to clone the repository"
            return 1
        fi
    else
        echo "Repository already exists, skipping clone"
    fi

    cd ~/build/picom || { echo "Failed to change directory to picom"; return 1; }

    # Build the project
    if ! meson setup --buildtype=release build; then
        echo "Meson setup failed"
        return 1
    fi

    if ! ninja -C build; then
        echo "Ninja build failed"
        return 1
    fi

    # Install the built binary
    if ! sudo ninja -C build install; then 
        echo "Failed to install the built binary"
        return 1
    fi

    echo "Picom animations installed successfully"

# allow brightnessctl non sudo
sudo usermod -aG input $USER
sudo usermod -aG video $USER
sudo chmod o+w /sys/class/backlight/*/brightness
