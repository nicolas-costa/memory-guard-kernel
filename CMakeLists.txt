cmake_minimum_required(VERSION 3.12)
project(MemoryGuardKernel C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(MemoryGuardKernel
        src/DeviceDispatch/DeviceDispatch.c
        src/DeviceDispatch/DeviceDispatch.h
        src/EntryPoint.c
        src/EntryPoint.h
        src/IoManagement/IoManagement.c
        src/IoManagement/IoManagement.h
        src/KernelCallbacks/KernelCallbacks.c
        src/KernelCallbacks/KernelCallbacks.h
        src/resources/resource.h
        src/stdafx.c
        src/stdafx.h)
