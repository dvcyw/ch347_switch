# CH347 Voltage Switcher (CH347 電壓切換工具)

This tool is specifically designed for the **Yaojie Electronics CH347 2nd Generation Programmer V2.13**. It allows users to switch the logic voltage between 1.8V and 3.3V on Linux.

本工具專為 **瑤杰電子 CH347 第二代編程器 V2.13** 設計，用於在 Linux 系統下切換 1.8V 與 3.3V 的邏輯電壓。

## Overview (概述)

The Yaojie CH347 programmer uses a specific USB command to control its onboard LDO/level-shifters. This command is sent via `libusb` to interface 2 of the CH347 chip.

瑤杰電子出品的 CH347 編程器透過特定的 USB 指令來控制板載的 LDO/電位轉換晶片。本程式使用 `libusb` 向 CH347 的 Interface 2 發送特定的 11 位元組指令包來達成電壓切換。

## Prerequisites (系統需求)

- Linux OS
- `libusb-1.0` development library
- Root privileges (for USB access)

### Install libusb (安裝 libusb)

On Ubuntu/Debian:
```bash
sudo apt-get install libusb-1.0-0-dev
```

## Building (編譯)

Use `gcc` to compile the source code:

```bash
gcc ch347_switch.c -o ch347_switch -lusb-1.0
```

## Usage (使用方法)

Run the compiled binary with `sudo` and specify the desired voltage.

### Switch to 1.8V (切換至 1.8V)
```bash
sudo ./ch347_switch 1.8
```
*(Default behavior if no argument is provided / 若未提供參數，預設為 1.8V)*

### Switch to 3.3V (切換至 3.3V)
```bash
sudo ./ch347_switch 3.3
```

## Technical Details (技術細節)

- **VID:** `0x1A86`
- **PID:** `0x55DB`
- **Interface:** 2
- **Endpoint:** `0x06` (Bulk Out)
- **Payload (11 bytes):**
    - 1.8V: `CC 08 00 08 08 08 08 28 08 F8 08`
    - 3.3V: `CC 08 00 08 08 08 08 28 08 F0 08`

## License (授權)
MIT License
