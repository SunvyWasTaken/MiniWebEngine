cmake_minimum_required(VERSION 3.15)

project(Game VERSION 1.0)

file(GLOB_RECURSE Sources "*.h" "*.cpp" ".txt" ".glsl")

if (NOT EMSCRIPTEN)
    macro(GroupSources curdir)
        file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
        ${PROJECT_SOURCE_DIR}/${curdir}/* ${PROJECT_SOURCE_DIR}/${curdir}/${curdir}/*)

        foreach(child ${children})

            if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                GroupSources(${curdir}/${child})
            else()
                string(REPLACE "/" "\\" groupname ${curdir})
                source_group(${groupname} FILES
                ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            endif()
        endforeach()
    endmacro()

    # Execute the macro
    GroupSources(Sources)
endif()

if(NOT EMSCRIPTEN)
    find_package(assimp CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(EnTT CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(glfw3 CONFIG)
    find_package(glad CONFIG REQUIRED)
    find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(Freetype REQUIRED)
endif()

set(TEXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Ressources/")

add_executable(${PROJECT_NAME} ${Sources})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

if(NOT EMSCRIPTEN)
    if(WIN32)
        # Sous Windows, utiliser une commande PowerShell pour trouver les dossiers nomm s "Public"
        execute_process(
            COMMAND powershell -Command "Get-ChildItem -Recurse -Directory | Where-Object { $_.Name -eq 'Public' } | ForEach-Object { $_.FullName }"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE PUBLIC_DIRS
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    else()
        # Sous Linux/macOS, utiliser la commande Unix `find`
        execute_process(
            COMMAND find ${CMAKE_CURRENT_SOURCE_DIR} -type d -name Public
            OUTPUT_VARIABLE PUBLIC_DIRS
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()

    # Remplacer les nouvelles lignes par des espaces pour garantir que les chemins sont bien s par s
    string(REPLACE "\n" " " PUBLIC_DIRS "${PUBLIC_DIRS}")

    # Transformer le r sultat en une liste CMake
    separate_arguments(PUBLIC_DIRS)

    # Ajouter chaque dossier trouv  dans les includes
    foreach(DIR ${PUBLIC_DIRS})
        target_include_directories(${PROJECT_NAME} PRIVATE ${DIR})
    endforeach()
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Core/Public/gpch.h")

target_link_libraries(${PROJECT_NAME} PUBLIC
    MiniMotor_compiler_flags
)

if(EMSCRIPTEN)
    message("Compiling with Emscripten")

    set(SHADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)

    target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/vcpkg_installed/wasm32-emscripten/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Core/Public"
        "${CMAKE_CURRENT_SOURCE_DIR}/Sources/ECS/Public"
        "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Inputs/Public"
        "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Render/Public"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".html"
        LINK_FLAGS "-sUSE_GLFW=3 -sFULL_ES3=1 -sWASM=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sNO_DISABLE_EXCEPTION_CATCHING --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/Shaders@/Shaders --preload-file ${CMAKE_SOURCE_DIR}/Ressources@/Ressources"
    )

else()
    target_link_libraries(${PROJECT_NAME} PUBLIC
        assimp::assimp
        EnTT::EnTT
        glm::glm-header-only
        glfw
        glad::glad
        imgui::imgui
	    spdlog::spdlog_header_only
        unofficial::omniverse-physx-sdk::sdk
        Freetype::Freetype
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${TEXTURES_DIR}"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Ressources"
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${TEXTURES_DIR}"
    "${PROJECT_BINARY_DIR}/Ressources"
)
