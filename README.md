# Javaneh Pilot

Javaneh Pilot brings together code for a vehicle control and autopilot prototype. The repository includes C/C++, Python and microcontroller projects, along with supporting libraries and vehicle signal definitions.

## What's here

- `auto_pilot_c/` — the C/C++ autopilot project, including control, drive, braking, GPS, LiDAR, CAN and Kuksa-related components.
- `control/`, `drive/`, `perception/` and `planning/` — separate control, drive, perception and planning code.
- `Esp32/`, `stm32/` and `Hardware controler/` — microcontroller sketches and hardware controller code.
- `lib/` — supporting libraries and hardware examples.
- `JsonGenerator/` — a Python script and signal files for generating JSON from CSV data.
- `vspec/` — vehicle signal specifications and related definitions.

## Getting started

The different parts of this repository have different hardware and software requirements. Start with the directory you want to work on and check its source and configuration before running it.

The C/C++ project uses CMake (the top-level project specifies version 3.8 or later). Its build files reference dependencies including gRPC, Protocol Buffers, hiredis and SQLite; the exact setup depends on the components you build. The Python scripts also have their own dependencies—for example, the JSON generator imports pandas. Microcontroller sketches need a compatible board, toolchain, libraries and wiring.

These are project sources, not a verified one-command installation. Build and runtime steps have not been validated here, and some paths or settings may need to be adapted to your environment.

## Credentials and connections

The certificates and token previously included under `auto_pilot_c/cert/` were removed for public release. According to the repository owner, those credentials have been revoked. Git history may still contain their earlier versions. Set up your own credentials and configuration outside the repository if your environment needs them.

## Working with hardware

Some programs interact with vehicle systems, sensors or actuators. Check the configuration and understand the code and hardware setup before running them. Test in a controlled environment.

## License

No top-level license is currently provided. Check the license files within bundled libraries separately before reusing them.
