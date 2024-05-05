# W806 CMake template

Intended to use with [Xuantie-800-gcc-elf-newlib](https://github.com/crosstyan/Xuantie-800-gcc-elf-newlib-prebuilt),
which have `gcc-13` support. (I guess old toolchain should work fine...)

Derived from [wm-sdk-w806](https://github.com/IOsetting/wm-sdk-w806).

<sup><sub>I'm not a fan of Makefile and it make me headache when debugging it</sub></sup>

## From `demo.elf` to `fls`

See [make_firmware.sh.in](https://github.com/github0null/w800_cmake_sdk/blob/master/tools/make_firmware.sh.in), which is
almost equivalent
to [rules.mk](https://github.com/IOsetting/wm-sdk-w806/blob/03b0f7fec247b05e16b5abb8c2310958f07114e9/tools/W806/rules.mk#L79-L103)
from `wm-sdk-w806`.
See
also [sdk.conf](https://github.com/github0null/w800_cmake_sdk/blob/e8d91ca604f0b623d9097273bdb69ebaf47785ee/sdk.conf#L18-L23).

If you don't care about encryption and signature stuff, things are pretty simple.

```bash
csky-unknown-elf-objcopy -Obinary demo.elf demo.bin
wm_tool -b demo.bin -o demo -it 1 -fc 0 -ra 0x080D0400 -ih 0x080D0000 -ua 0x08010000 -nh 0 -un 0
```

- `-b demo.bin`: Specifies the input binary file (`demo.bin`) to be packaged.
- `-o demo`: Specifies the output firmware file name prefix (`demo`).
- `-it 1`: Sets the firmware image layout type to 1 (user image).
- `-fc 0`: Disables firmware compression.
- `-ra 0x080D0400`: Sets the runtime position (address) of the firmware to `0x080D0400`.
- `-ih 0x080D0000`: Sets the image header storage location to `0x080D0000`.
- `-ua 0x08010000`: Sets the upgrade storage location (OTA address) to `0x08010000`.
- `-nh 0`: Sets the next image header storage location to 0 (not used).
- `-un 0`: Sets the upd no version number to 0 (not used).

## Flashing

See [crosstyan/wm_tool_rs](https://github.com/crosstyan/wm_tool_rs).

### With `wm_tool`

You can also use the original [wm_tool](https://github.com/crosstyan/wm_tool).

Following commands are stolen
from [github0null/w800_cmake_sdk](https://github.com/github0null/w800_cmake_sdk/blob/e8d91ca604f0b623d9097273bdb69ebaf47785ee/tools/flash_firmware.sh.in#L43C1-L43C76)

```bash
wm_tool -dl "$firmware" -ws 2M -ds 2M -rs rts -c $port -sl str -ws 115200
```

Here's the explanation generated by AI. Take it with a grain of salt.

<blockquote>

- `-dl "$firmware"`: Specifies the firmware file to be downloaded. The `$firmware` variable should contain the path to
  the firmware file.
- `-ws 2M`: Sets the serial port speed during normal work (after firmware download) to 2 Mbps.
- `-ds 2M`: Sets the serial port speed during firmware download to 2 Mbps.
- `-rs rts`: Sets the device reset method to use the serial port's RTS (Request to Send) pin to control the device
  reset.
- `-c $port`: Specifies the serial port to connect to for downloading the firmware. The `$port` variable should contain
  the name of the serial port (e.g., COM3, /dev/ttyUSB0).
- `-sl str`: Sets the display format for the log information output from the serial port to string mode.
- `-ws 115200`: Sets the serial port speed during normal work (after firmware download) to 115200 bps. This option
  overrides the previous `-ws 2M` setting.

In summary, this command does the following:

1. Connects to the specified serial port (`$port`) to communicate with the W800 device.
2. Sets the serial port speed to 2 Mbps during firmware download.
3. Uses the RTS pin to control the device reset.
4. Downloads the specified firmware file (`$firmware`) to the W800 device.
5. Sets the serial port speed to 115200 bps for normal operation after the firmware download.
6. Displays the log information output from the serial port in string format.

</blockquote>

### Serial Monitor

```bash
wm_tool -c ttyUSB0 -sl str -ws 115200
```

### Erase

```bash
wm_tool -c ttyUSB0 -rs rts -eo all
```

## Debugging

Not available yet. See [W806 debugging (part 1)](https://www.blaatschaap.be/w806-debugging-part-1/).

## Reference

- [联盛德 HLK-W806 (七): 兼容开发板 LuatOS Air103](https://www.cnblogs.com/milton/p/15676414.html)
- [Air103 at LuatOS](https://wiki.luatos.com/chips/air103/index.html)
- [github0null/w800_cmake_sdk](https://github.com/github0null/w800_cmake_sdk)
- [crosstyan/wm_tool](https://github.com/crosstyan/wm_tool)
- [W806 debugging (part 1)](https://www.blaatschaap.be/w806-debugging-part-1/)
- [Air103 资料汇总(软硬件资料,固件下载,技术支持)](https://doc.openluat.com/article/3674)
- [About AT mode when download firmware](https://github.com/IOsetting/wm-sdk-w806/blob/03b0f7fec247b05e16b5abb8c2310958f07114e9/platform/component/auto_dl/auto_dl.c#L26-L31)
- [联盛德微W806芯片移植RTthread-Nano](https://www.cnblogs.com/BlogsOfLei/p/15674854.html)
- [OpenHarmony LiteOS指令集移植指南（C-SKY)](https://bbs.huaweicloud.com/blogs/308678)
- [E804](https://www.xrvm.cn/product/xuantie/E804)
