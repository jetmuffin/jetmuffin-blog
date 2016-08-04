title: OSX系统列举USB设备方法
date: 2016-03-13 21:38:34
tags:
---
Linux 用户在列举USB设备时，可以使用`lsusb`命令来查看所有已经连接的usb设备，或者进入`/dev/bus/usb/001`之类进行查看。

<!-- more -->
而在Mac OS下，系统自带的命令则是`system_profiler SPUSBDataType`，使用这条命令可以查看所有连接的USB设备，如下：

```
 jeff@promote ⮀ ~ ⮀ system_profiler SPUSBDataType
USB:

    USB 3.0 Bus:

      BSD Name: en4
      Host Controller Driver: AppleUSBXHCIWPT
      PCI Device ID: 0x9cb1
      PCI Revision ID: 0x0003
      PCI Vendor ID: 0x8086

        USB3.0 Hub:

          Product ID: 0x0612
          Vendor ID: 0x05e3  (Genesys Logic, Inc.)
          Version: 92.16
          Speed: Up to 5 Gb/sec
          Manufacturer: GenesysLogic
          Location ID: 0x14400000 / 6
          Current Available (mA): 1800
          Current Required (mA): 0
          Extra Operating Current (mA): 0

        Bluetooth USB Host Controller:

          Product ID: 0x8290
          Vendor ID: 0x05ac  (Apple Inc.)
          Version: 1.11
          Speed: Up to 12 Mb/sec
          Manufacturer: Broadcom Corp.
          Location ID: 0x14300000 / 2
          Current Available (mA): 1000
          Current Required (mA): 0
          Extra Operating Current (mA): 0
          Built-In: Yes

        Razer DeathAdder:

          Product ID: 0x0016
          Vendor ID: 0x1532
          Version: 1.00
          Speed: Up to 12 Mb/sec
          Manufacturer: Razer
          Location ID: 0x14200000 / 4
          Current Available (mA): 1000
          Current Required (mA): 100
          Extra Operating Current (mA): 0

        USB2.0 Hub:

          Product ID: 0x0610
          Vendor ID: 0x05e3  (Genesys Logic, Inc.)
          Version: 92.16
          Speed: Up to 480 Mb/sec
          Manufacturer: GenesysLogic
          Location ID: 0x14100000 / 5
          Current Available (mA): 1000
          Current Required (mA): 100
          Extra Operating Current (mA): 0
          BSD Name: en4

            Apple USB Ethernet Adapter:

              Product ID: 0x1402
              Vendor ID: 0x05ac  (Apple Inc.)
              Version: 0.01
              Serial Number: 1460AB
              Speed: Up to 480 Mb/sec
              Manufacturer: Apple Inc.
              Location ID: 0x14140000 / 7
              Current Available (mA): 1000
              Current Required (mA): 250
              Extra Operating Current (mA): 0
              BSD Name: en4

            iPhone:

              Product ID: 0x12a8
              Vendor ID: 0x05ac  (Apple Inc.)
              Version: 7.02
              Serial Number: 1ba089f6c8745ae921e95dea83879c7e36b43b9c
              Speed: Up to 480 Mb/sec
              Manufacturer: Apple Inc.
              Location ID: 0x14130000 / 8
              Current Available (mA): 1000
              Current Required (mA): 500
              Extra Operating Current (mA): 0
              Sleep current (mA): 500
```

而在图形界面上`EL Capitan`的操作步骤为：
* 点击左上角苹果标志
* 点击`关于本机`-> `系统报告` -> `USB`
* 然后则可以查看系统总线上挂载的USB信息了

**最后说下为啥写这个：**最近在研究怎么在`docker container`中挂载USB，首先要获取总线上的USB设备，这是个开头吧。 