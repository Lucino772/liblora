# liblora

## Project Structure

```ascii
examples
core
├── lib
│   ├── sx127x
│   ├── sx130x
│   └── ...
├── proto
│   ├── lorawan
│   └── ...
├── hal
└── classes
platform-io
arduino
linux
├── service
└── driver
```

<!-- examples -->
<u>`examples/`</u>:

This directory contains all the examples for multiple platforms.

<!-- core -->
<u>`core/`</u>:

This directory contains an implementation in C (`lib/`) and C++ (`classes/`) of each radio. It also contains the implementation of different protocols, LoRaWAN for example (`proto/`). The last sub-directory, `hal/` contains a abstraction layer of all the radios.

Note: I'm note sure yet if it's possible to build an interesting **HAL**.

<!-- platform.io -->
<u>`platform-io/`</u>:

This directory contains a **platform.io** library compatible on multiple platforms.

<!-- linux -->
<u>`linux/`</u>:

This directory contains the code for a linux service and a linux driver. Both programs can be used by other programs on the system to communicate with the LoRa module.

NOTE: This is not a mature idea yet.