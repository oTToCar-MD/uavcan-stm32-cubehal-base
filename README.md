# uavcan-stm32-cubehal-base

This repository contains ready to use boilerplate code for a ST STM32 F1 / F4 firmware project that uses ST CubeHAL and [UAVCAN v0](https://legacy.uavcan.org/). The default version of this is meant to be used with generated code from [CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html).  Originally written by Tim Wiesner for internal use in the oTToCar 1:10 autonomous vehicle.

Further goodies included here:

- Visual Studio Code Build Task and Launch Configuration
- Launch profiles for STlink (non-RTOS), JLink (non-RTOS) and JLink (RTOS-aware)
- Firmware hashing 
- C++ ABI bindings 
- Git information retrieval to be compiled into firmware
- UAVCAN service to retrieve build information
- Easy to use Makefile build system
- FreeRTOS configuration 

The following libraries/components are included:
* [libuavcan](https://github.com/UAVCAN/libuavcan)
* [libuavcan STM32 driver](https://github.com/UAVCAN/platform_specific_components/tree/4e0a24756090d218b469f561d7e0d35e5f2f5427)

## Prequisites

**This is meant to be used on Linux!**. Install build-essential and git.

### [GNU Arm Embedded Toolchain (arm-none-eabi)](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)

In order for bus-node-base to be compilable, it requires at minimum a version of the GNU Arm Embedded Toolchain that supports C++17. You should, however, stick to the latest version if possible.

##### Distributions older than 2019

**DO NOT** install the toolchain via *apt* from the Ubuntu archives or a PPA, as these sources usually provide very outdated versions.

1. Goto the download site and download the Linux x86_64 Tarball.
2. Extract the files to the target install directory, e.g. by `$ cd $install_dir && tar xjf gcc-arm-none-eabi-*-yyyymmdd-linux.tar.bz2`
3. Add the bin directory permanently to your PATH environment variable using the following commands:

  ```
  $ export PATH=$PATH:$install_dir/gcc-arm-none-eabi-*/bin
  $ arm-none-eabi-gcc
  ```

  If you are using Ubuntu, have a look at [this](https://help.ubuntu.com/community/EnvironmentVariables) to select an appropriate location for the script so it will either affect all shells of a particular user or the entire system.

You should also have a look at the file `share/doc/gcc-arm-none-eabi/readme.txt` which contains additional information about installation and usage.

##### Something newer than 2019

Just install gcc-arm-none-eabi and check that you have at least version 9 installed.


### [Visual Studio Code](https://code.visualstudio.com/)

You can, of course, choose any code (or even plain text) editor for programming the UAVCAN nodes, but VS Code makes the whole edit-compile-debug cycle much more comfortable. It incorporates git, the possibility to configure multiple launch and debug targets, debugging support for Cortex-M targets (via plugins) and is overall highly customizable, so I *really* recommend its utilization.

1. Download the .deb package (it does not matter wether you choose the default one or the Insiders Edition) from the official site. Alternatively, you can install it via snap by running `sudo snap install code --classic` or `sudo snap install code-insiders --classic` from a terminal.

2. Now open VS Code and open the extensions tab (either by clicking on the icon on the leftmost bar or by pressing *Ctrl+Shift+X*). Search for the following extension IDs and add them by clicking *Install*:
   
  | Name | Extension ID | Description |
  | - | - | - |
  | [ARM](https://marketplace.visualstudio.com/items?itemName=dan-c-underwood.arm) | dan-c-underwood.arm | Language support for ARM assembly |
  | [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) | ms-vscode.cpptools | C/C++ IntelliSense, debugging, and code browsing |
  | [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) | marus25.cortex-debug | ARM Cortex-M GDB Debugger support for VSCode |
  | [Cortex-Debug: Device Support Pack - STM32F1](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug-dp-stm32f1) | marus25.cortex-debug-dp-stm32f1 | SVD definitions for Cortex-Debug for viewing peripheral registers of STM32F1 devices |
  | [Cortex-Debug: Device Support Pack - STM32F4](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug-dp-stm32f4) | marus25.cortex-debug-dp-stm32f4 | SVD definitions for Cortex-Debug for viewing peripheral registers of STM32F4 devices |
  | [LinkerScript](https://marketplace.visualstudio.com/items?itemName=zixuanwang.linkerscript) | zixuanwang.linkerscript | Language support for GNU linker script |

### [CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)

CubeMX is a graphic tool developed by ST that allows you to configure STM32 microcontrollers easily. It generates C initialization code based on the chosen peripherals and automatically adds the required drivers from the STM32CubeHAL. While it still has some bugs that require attention, this tool is a very valuable tool that simplifies the configuration process a lot compared to writing all the initialization code by hand.

Download the tool from ST's website. Unpack the ZIP file and run the `SetupSTM32CubeMX-x.x.x.linux` file.

### [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)

We primarily use the [J-Link EDU](https://www.segger.com/products/debug-probes/j-link/models/j-link-edu/) debug probe for flashing and debugging our microcontroller targets. To be able to use it, you have to download the [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) (direct [link](https://www.segger.com/downloads/jlink/JLink_Linux_x86_64.deb) for the 64-bit DEB installer) and install it.

## Project setup

In order to create a new bus node running UAVCAN, you need to create an empty repository and add this one as a submodule. Run `git submodule update --init --recursive` to properly initialize the nested submodules. Afterwards, add a *Makefile* that looks like the following:

```make
TARGET  := example-node
RTOS    := freertos
DEVICE  := stm32f103re

UAVCAN_STM32_TIMER_NUMBER   := 7
UAVCAN_STM32_NUM_IFACES     := 1

STM32_PCLK1     := 42000000
STM32_TIMCLK1   := 84000000

INCDIRS         := include/
SOURCES         := src/main.cpp

# Actual build engine
include uavcan-stm32-cubehal-base/include.mk
```

| Parameter | Meaning |
| - | - |
| `TARGET` | The name of the resulting firmware image (without extension) |
| `RTOS` | Can be one of the following:<ul><li>`none` There is no OS being used</li><li>`freertos` FreeRTOS is being used</li></ul> |
| `DEVICE` | Device identifier of your target MCU, e.g. `stm32f103re`. Can be omitted if you do not intend to flash using a J-Link from a command line. |
| `UAVCAN_STM32_TIMER_NUMBER` | The number of the hardware timer used for clock functions. Consider using a *basic timer* (see the [STM32 timers](#stm32-timers) section below). |
| `UAVCAN_STM32_NUM_IFACES` | The number of CAN interfaces that will be used (some have two). Defaults to 1 if not specified. |
| `STM32_PCLK1` | PCLK1 (APB1) frequency in Hertz (see [clock configuration](#clock-configuration)) |
| `STM32_TIMCLK1` | TIMCLK1 frequency in Hertz (see [clock configuration](#clock-configuration)) |
| `INCDIRS` | List of include directories |
| `SOURCES` | List of application source files to be compiled |
| `UAVCAN_MEMPOOL_SIZE` | *optional* Memory pool size for internal uavcan allocations. Default 16KiB |
| `UAVCAN_RX_QUEUE_SIZE` | *optional* RX queu size. Hard minimum is 10. Default 128 |

Also add a `.gitignore` file. Just copy `uavcan-stm32-cubehal-base/templates/gitignore` to the project root and prefix its filename with a dot (`.`).

## CubeMX setup

Now you need to create a new CubeMX project. To do so, open the application and click *File > New project ...* or hit *Ctrl+N*. Select the MCU and the package you are going to use and then click *Start Project* on top of the page.

Follow the instructions below **in the order they are listed**. The headlines correspond to the tabs in the project window.

### Project manager

#### Project

* Change the Application Structure to **Advanced**.
* Select **Makefile** as the Toolchain / IDE.
* Ignore all "Empty project name" errors and check if the settings stuck after saving the project.

#### Code Generator

* Select **Copy only the necessary library files**. Otherwise you end up having more than 100 MiB to upload.
* Check **Generate peripheral initialization as a pair of .c/.h files per peripheral**.
* Enable **Set all free pins as analog**.
* If you want, check *Enable Full Assert*.

Now save the project by clicking *File > Save Project As ..* or by hitting *Ctrl+A*. You will have to choose a folder as the location for the project, so create a new sub-directory in the project directory called *TARGET.cubemx*, where *TARGET* corresponds to the variable you defined earlier in the Makefile. So in the example above our folder would be named *example-node.cubemx*.

### Pinout and configuration

1. Goto *System Core > RCC*. Select **Crystal/Ceramic Resonator** for the High Speed Clock (HSE).
2. Goto *System Core > SYS*. Select **Trace Asynchronous Sw** as the debug interface.
3. Activate and configure all peripherals you want to use.
4. You especially need to enable **CAN** (or **CAN1** if your MCU has multiple CAN controllers). Further setup of the CAN peripheral is not required, as this will be done by the libuavcan driver.

#### When using FreeRTOS

1. Goto *Middleware > FREERTOS > Mode* and select **CMSIS_V2** as the interface.
2. Under *Configuration > Config parameters* configure FreeRTOS as desired.
    * Enable **RECORD_STACK_HIGH_ADDRESS** to ease debugging issues related to stack overflows.
    * Pay attention to **TOTAL_HEAP_SIZE** as this value is most likely way to small and prevents you from choosing an appropriate task stack size later on.
    * Select **heap_1** as the memory management scheme.
    * Choose **Option2** for **CHECK_FOR_STACK_OVERFLOW**.
    * Make sure that **USE_TRACE_FACILITY** is **enabled**, otherwise the firmware can't interact with the Tracealyzer. Leave disabled when Tracealyzer support is not required.
3. Next, on the *Tasks and Queues* tab, create the individual tasks and queues as necessary.
    * Note that the default task **cannot** be deleted and that the Code Generation Option **must** be *Default*. Renaming, however, is possible.
    * If you want to define your tasks outside the generated *main.c*, choose *External* for the Code Generation Option. *Keep in mind that you will have to define your task function as `extern "C"` when defining them in a C++ source file.*
4. Goto *System Core > SYS > Mode* and change the *Timebase Source* to a timer **different from SysTick and the timer chosen for UAVCAN** (see Makefile options and the [STM32 timers](#stm32-timers) section below).
5. Goto *System Core > NVIC > Configuration* and put in a preemtion priority between *LIBRARY_LOWEST_INTERRUPT_PRIORITY* (usually 15) and *LIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY* (usually 5). Higher values mean lower priority. **This must be done for every interrupt that will call FreeRTOS function in its handler. Violating this rule will result in a hard fault eventually.** Depending on the version of CubeMX there may still be a checkbox column named *Use FreeRTOS functions*. If available use this instead

### Clock configuration

While the view in this section very much depends on your MCU, there are a few things that should always be done here.

1. Enter the frequency of the HSE crystal in use.
2. Select **HSE** as the source of the PLL.
3. Make sure the system clock MUX uses **PLLCLK** as its input.
4. Enter your desired system clock (HCLK).
5. Only alter the peripheral clock settings if you have special requirements.
6. If there are any red fields in the clock tree, click 'Resolve Clock Issues' on the top of the page.
7. Note down the **APB1 peripheral clocks** and **APB1 Timer clocks** values; they are required in the project's Makefile (see above).

## First-time code generation

When you are done configuring the CubeMX project, click *GENERATE CODE* at the top of the page. CubeMX will display a warning message if there is anything misconfigured. When the process has finished, you have to make some minor addittions to the generated files.

### Makefile

Go to the *.cubemx* sub-directory and open the *Makefile* in an editor. Delete everything except the following variables/sections:

* C_SOURCES
* ASM_SOURCES
* CPU
* FPU
* FLOAT_ABI
* MCU
* AS_DEFS
* C_DEFS
* AS_INCLUDES
* C_INCLUDES
* LDSCRIPT
* **The EOF marker at the end of the file**

### Chip definitions

You need to provide a file called `chip.h` that includes the HAL file for the MCU and defines the names of the interrupt service routines for the CAN driver. The file will be used by the UAVCAN driver and the Tracealyzer.

Implementations for *STM32F1* and *STM32F4* series chips are already in the include path. If you use another MCU take a look at *chip/chip.h*.

## Visual Studio Code configuration

### Include paths and preprocessor definitions

1. If not already present, create a directory called `.vscode` in you project's root directory.
2. Copy `uavcan-stm32-cubehal-base/templates/c_cpp_properties.json` into said directory.
3. Adjust the configuration to your needs.
  * Add the include paths listed under *C_INCLUDES* in `*.cubemx/Makefile`. Most are platform-specific and are thus not part of the template.
  * Add the include path for the *chip.h*
  * Add all of your own include paths
  * Provide a proper definition for your MCU family

### Launch configurations

Just copy `uavcan-stm32-cubehal-base/templates/launch.json` to `.vscode/`. This default launch set contains three configurations, one for the J-Link probe without an RTOS being used, another one for the J-Link with FreeRTOS running and one for an ST-LINK device (available on ST's nucleo boards, without using an RTOS).

Foreach configuration you have to adjust some attributes:
* `executable`: Insert the correct name of the TARGET
* `device`: The actuel part number of the MCU, e.g. *STM32F103RE*

For some MCUs of the STM32F1 and STM32F4 families there are peripheral register definitions (SVD files) available through the two Device Support Packs you installed earlier. In this case setting the `device` attribute is sufficient. However, if your MCU is not covered (see [Debugging](#debugging) section on how to find out), you will have to provide an SVD file on your own, just search for it on [ST's website](https://www.st.com/content/st_com/en.html). Put the file somewhere in your project directory and add an `svdFile` attribute like the following:
```json
"svdFile": "{workspaceRoot}/STM32F446.svd"
```

### Build tasks

Lastly, copy `uavcan-stm32-cubehal-base/templates/tasks.json` to `.vscode/`. This file contains four different build tasks:

| Task | Decription |
| - | - |
| Clean all | Deletes the entire *build* directory |
| Build firmware (Debug) | Creates a debug build, which is task run most often. Can be run by *Ctrl+Shift+B* |
| Build firmware (Release) | Creates a release build with `-O3` option |
| Build firmware (Release, size-optimized) | Creates a release build with `-Os` option |

## Building

* **From within Visual Studio Code**

  To build the firmware in debug mode just press *Ctrl+Shift+B*. To clean up or do a release build, run the corresponding task via the *Tasks: Run Task* command from the command palette.

* **Externally (shell)**

  You just need to run `make` inside your project's root directory for the debug build. For the release build, append `RELEASE=1` and optionally `RELEASE_OPT=x`, where *x* is a valid GCC optimization option, like `3` or `s`.
  
  To remove all build-related files run `make clean`.

The resulting firmware image can be found in `<your-project>/build/{debug,release}/<TARGET>.bin`. If anything goes wrong, see the [Known issues](#known-issues) section below.

## Flashing via command line

Flashing via the command line takes some additional steps. If you start debugging from Visual Studio Code you can skip this section since the target MCU will be flashed automatically by the debug task.

* via *ST-LINK*

  Run `make stflash` for the debug build. Append `RELEASE=1` for the release build.

* via *J-Link*

  If you are flashing for the first time, you need to to copy `bus-node-base/templates/flash_commands.jlink` to the project's root directory and modify it. In the second line adjust the name of your target binary, e.e. `example-node.bin`. Also make sure to have included a definition for `DEVICE` in your Makefile.

  After that and each subsequent time, run `make jflash`.

  **NOTE:** You cannot switch between debug/release easily. This command always uploads the file you specified in *flash_commands.jlink*.

## Debugging

*This section is yet to be written.*

## Known issues

### Multiple defintition of 'SysTick_Handler'

**Do not comment out this line when tracealyzer is disabled!**  
Including the Tracealyzer headers currently results in compilation errors since the *SysTick_Handler* will be defined twice. To fix this, open *.cubemx/Core/Inc/FreeRTOSConfig.h* and head for the end of the file. Search for the following line:

```C
#define xPortSysTickHandler SysTick_Handler
```

Comment this line out.

Unfortunately this line is not in a user code section and thus **needs to be commented out each time the code is re-generated**.

### Duplicate lines in the CubeMX Makefile

Whenever you re-generate the code using CubeMX, it will adjust the Makefile in the sub-directory by updating sources and include paths. Sometimes it inserts a duplicate of the last line of either of these resulting in syntax errors. In this case, just remove the duplicate lines.

### make: *** No rule to make target needed by 'all'.  Stop.

You have declared files in INCDIRS or SOURCES that don't exist.

### Firmware immediately jumps into defaultHandler when Tracealyzer is disabled.

FreeRTOS doesn't have its SysTick_Handler linked. Do not remove the #define generated by CubeMX.

## Additional notes

### STM32 timers

uavcan-stm32-cubehal-base requires 3 different timers for the following components:

* **The FreeRTOS scheduler**

  The scheduler's tick function must be called at an regular interval (by default 1000 Hz) to be able to switch contexts. The **SysTick** timer was designed specifically for that purpose, so if you activate FreeRTOS within CubeMX the SysTick timer will automatically be used as the tick source.

* **STM32CubeHAL**

  The CubeHAL requires the use of a timer for timeout detection. When not using FreeRTOS this is the SysTick timer by default. When using it, you can choose any other timer for this purpose, but make sure it is not the same timer used for the UAVCAN driver. Consider using a **basic timer** if available.

* **UAVCAN driver**

  The UAVCAN driver uses a timer for clock functions. You can choose any timer connected to *APB1* other than the one used by CubeHAL for this purpose. Consider using a **basic timer** if available.

* **GetBuildInformation**
  Some code to start the build information service.

  ```C++
  auto &node = base::getNode();
  uavcan::ServiceServer<GetBuildInformation> buildInfoServer{node};
  buildInfoServer.start(
            [](const uavcan::ReceivedDataStructure<GetBuildInformation::Request> &request,
               GetBuildInformation::Response &response) {
                response.branch_name = build_info::BranchName;
                response.build_time = build_info::BuildTime;
                response.commit_hash = build_info::CommitHashLong;
                response.is_debug = build_info::IsDebugBuild;
                response.is_dirty = build_info::IsDirty;
            });
  ```


See [AN4013](https://www.st.com/resource/en/application_note/dm00042534-stm32-crossseries-timer-overview-stmicroelectronics.pdf) for an overview of the various kinds of timers used in STM32 microcontrollers and specific timer numbers and types in the device families.