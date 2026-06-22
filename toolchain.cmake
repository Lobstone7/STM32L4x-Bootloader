# Tell CMake this is bare-metal
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

# Compiler
set(CMAKE_C_COMPILER "C:/Program Files/Arm/GNU Toolchain mingw-w64-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc.exe")
set(CMAKE_ASM_COMPILER "C:/Program Files/Arm/GNU Toolchain mingw-w64-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc.exe")

# Prevent CMake from adding host flags like -fPIC
set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_ASM_FLAGS_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")

# Disable try-compile executable (important for embedded)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Disable position independent code globally
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)