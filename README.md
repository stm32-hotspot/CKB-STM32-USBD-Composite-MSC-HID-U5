# STM32 Composite Class USB Device Application (MSC + HID)  based on the STM32U5 

## Overview 

This project implements a composite USB device combining two classes:
- **Mass Storage Class (MSC)**: allows using internal SRAM as USB storage.
- **Human Interface Device (HID)**: emulates a user input device (mouse).

## Application Description

This application demonstrates how to implement a USB device combining Mass Storage Class (MSC) and Human Interface Device (HID) functionalities on the  STM32U545RE microcontroller. 

It showcases the use of the STM32 USB Full-Speed Device peripheral to create a composite USB device that is simultaneously recognized by the host PC as both a removable storage drive and a HID mouse. 

The NUCLEO-U545RE-Q board is used to illustrate this application, leveraging internal SRAM for mass storage and emulating a simple mouse through HID reports. 

### Objectives

This application aims to show how to:
- Create a composite USB device with MSC and HID interfaces.
- Provide USB mass storage functionality using internal SRAM.
- Emulate a USB HID mouse sending periodic movement reports.

### Key Features

This application showcases the following features:
- **USB Composite Stack**:  supports multiple interfaces.
- **MSC Interface**:
  - Implements SCSI commands for read/write.
  - Uses internal SRAM as storage medium.
- **HID Interface**:
  - Emulates a standard USB mouse.
  - Sends HID reports via interrupt endpoints.


### Hardware and Software environment

- This application runs on STM32U545RE devices.
- This application has been tested with STMicroelectronics NUCLEO-U545RE-Q boards and can be easily tailored to any other supported device and development board.
- This application is developed in STM32CubeIDE v1.18.0.  

### How to use it ?

 - Open and import the project into STM32CubeIDE v1.18.0 or newer.
 - Rebuild all files and load your image into target memory.
 - Run the application. 

## Additional resources

Please refer to [this](tbd) article for more details.

## Contributing and support request

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
