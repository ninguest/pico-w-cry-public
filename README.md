# Ninguest's Pico W Projects

Some Pico W projects in C SDK.

## Project Structure

- `build/`: Directory for build output.
- `CMakeLists.txt`: CMake configuration file.
- `header.h`: Header file containing function declarations and macros.
- `lwipopts.h`: Configuration file for the lwIP stack.
- `main.c`: Main source file for the project.
- `pico_sdk_import.cmake`: CMake script to import the Pico SDK.

## Getting Started

### Prerequisites

- [CMake](https://cmake.org/)
- [Pico SDK](https://github.com/raspberrypi/pico-sdk)

### Building the Project

1. Clone the repository:
    ```sh
    git clone <repository-url>
    cd ninguest-usb-serial-communication
    ```

2. Initialize the Pico SDK submodule:
    ```sh
    git submodule update --init
    ```

3. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

4. Configure the project with CMake:
    ```sh
    【Windows】 cmake -G "Ninja" ..
    【Mac/Linux】 cmake ..
    ```

5. Build the project:
    ```sh
    【Windows】 ninja
    【Mac/Linux】 make
    ```

### Running the Project

1. Connect your Ninguest device to your computer via USB.
2. Flash the compiled binary to the device.
3. Open a serial terminal to communicate with the device.

## License

This project is licensed under the GNU GENERAL PUBLIC LICENSE - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)

- [LearnEmbeddedSystems/rp2040-project-template](https://github.com/LearnEmbeddedSystems/rp2040-project-template)

## YouTube Demo

#### [PICO W USB Serial Communication DEMO 🔗](https://www.youtube.com/watch?v=pG0NRNOnaoA)

[![PICO W USB Serial Communication DEMO](https://img.youtube.com/vi/pG0NRNOnaoA/maxresdefault.jpg)](https://www.youtube.com/watch?v=pG0NRNOnaoA)
