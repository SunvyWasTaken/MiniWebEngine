@echo on
set currentPath=%cd%
cd D:/Emscripten/emsdk
git pull
call emsdk.bat activate latest
call emsdk_env.bat
cd %currentPath%/vcpkg
call bootstrap-vcpkg.bat -disableMetrics"
vcpkg install --triplet wasm32-emscripten
cd %currentPath%
emcmake cmake -B %currentPath%/Web -S %currentPath%
Pause