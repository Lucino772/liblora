# liblora

## Project Structure

```ascii
examples
core
├── include
│   └── liblora/core
│       ├── radios
│       │   ├── sx127x
│       │   ├── sx130x
│       │   └── ...
│       ├── proto
│       │   ├── lorawan
│       │   └── ...
│       └── hal
└── src
platforms
├── pio
│   ├── include
│   │   └── liblora
│   │       ├── core -> /corelib/liblora/core
│   │       ├── sx127x
│   │       └── sx130x
│   └── src
├── arduino
└── linux
```
