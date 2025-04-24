@echo off
set currentPath=%cd%

if exist %currentPath%/vcpkg (
ECHO Update vcpkg
cmd /c "cd vcpkg && git pull"
) else (
ECHO Clone vcpkg
ECHO =============================
cmd /c git clone https://github.com/Microsoft/vcpkg.git
)

ECHO Initializing vcpkg
ECHO =============================
cmd /c "cd vcpkg && bootstrap-vcpkg.bat -disableMetrics"

ECHO Initializing vcpkg packages
ECHO =============================
cd vcpkg
vcpkg install

ECHO Initializing CMake project
ECHO =============================
cd %currentPath%
cmake -DCMAKE_TOOLCHAIN_FILE=%currentPath%/vcpkg/scripts/buildsystems/vcpkg.cmake  -S %currentPath% -B %currentPath%/build

ECHO =============================
ECHO Install done

Pause