@echo on
cmake -DCMAKE_TOOLCHAIN_FILE=D:/source/truc/vcpkg/scripts/buildsystems/vcpkg.cmake -S D:/source/truc/ -B D:/source/truc/Web -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=D:/Emscripten/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten
cmd