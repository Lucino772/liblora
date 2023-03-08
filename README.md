# liblora

## Project Structure

Multiple examples can be found in the [`examples`](/examples/) directory. Those examples are organized by platform:
- [`examples/pio`](/examples/pio/): examples that use the **Platform.io** library.
- [`examples/arduino`](/examples/arduino/): examples that use the **Arduino** library.
- [`examples/linux`](/examples/linux/): Check out [Linux Platform](#linux-platform).

Different libraries and programs are available depending on the platform:
- [`platforms/pio`](/platforms/pio/): contains the **Platform.io** library (C++). [Examples](/examples/pio)
- [`platforms/arduino`](/platforms/arduino/): contains the **Arduino** library (C++). [Examples](/examples/arduino)
- [`platforms/linux`](/platforms/linux/): Check out [Linux Platform](#linux-platform).

All the libraries are based on the same C code which can be found in the `core/` directory.

## Linux Platform

<u>1. Platform.io</u>

The easiest way of using this library on linux is by using **Platform.io**. **Platform.io** allows you to compile the library on a Raspberry Pi.

[Examples](/examples/pio/)

<u>2. Linux Service</u>

*Coming Soon*

[Examples](/examples/linux/service)

<u>3. Linux Driver</u>

*Coming Soon*


[Examples](/examples/linux/driver)
