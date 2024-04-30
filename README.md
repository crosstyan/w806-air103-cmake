# W806 CMake template

Intended to use with [Xuantie-800-gcc-elf-newlib](https://github.com/crosstyan/Xuantie-800-gcc-elf-newlib-prebuilt),
which have `gcc-13` support. (I guess old toolchain should work fine...)

Derived from [wm-sdk-w806](https://github.com/IOsetting/wm-sdk-w806).

<sup><sub>I'm not a fan of Makefile</sub></sup>

```bash
csky-unknown-elf-objcopy -Obinary demo.elf demo.bin
```

## Reference

- [联盛德 HLK-W806 (七): 兼容开发板 LuatOS Air103](https://www.cnblogs.com/milton/p/15676414.html)
- [Air103](https://wiki.luatos.com/chips/air103/index.html)
- [github0null/w800_cmake_sdk](https://github.com/github0null/w800_cmake_sdk)
- [https://github.com/crosstyan/wm_tool](https://github.com/crosstyan/wm_tool)
