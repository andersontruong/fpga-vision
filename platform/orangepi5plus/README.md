# Setup on Orange Pi 5 Plus + 256GB eMMC + R6 Wi-Fi6+BT5.2 Wi-Fi module

## 1. Installation
1. Connect R6 Wi-Fi card to the M.2 E-key slot
2. Connect NiteFury II to M.2 M-key slot on the back
3. Add other peripherals and minimum 5V/3A USB-C power supply to `DC_IN`

## 2. Booting
- [Ubuntu 22.04 Desktop Image for aarch64](https://joshua-riek.github.io/ubuntu-rockchip-download/boards/orangepi-5-plus.html)
- [Install Ubuntu onto eMMC](https://github.com/Joshua-Riek/ubuntu-rockchip/wiki/Ubuntu-24.04-LTS#install-ubuntu-onto-an-emmc-from-linux)

## 3. XDMA Drivers
- [XDMA Drivers Fork for aarch64](https://github.com/laurencebarker/Saturn/tree/main/linuxdriver)
- [Run tests using RHS Research sample project for NiteFury II](https://github.com/RHSResearchLLC/NiteFury-and-LiteFury/tree/master/Sample-Projects/Project-0/Host)
