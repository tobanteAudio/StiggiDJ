cmake_minimum_required(VERSION 3.15)
project(app-stiggi-dj VERSION 0.1.0)

option(TA_ENABLE_COVERAGE   "Build with coverage enabled"                       OFF)
option(TA_ENABLE_ASAN       "Build with address sanitizer enabled"              OFF)
option(TA_ENABLE_UBSAN      "Build with undefined behavior sanitizer enabled"   OFF)
option(TA_ENABLE_TSAN       "Build with thread sanitizer enabled"               OFF)
option(TA_ENABLE_MSAN       "Build with memory sanitizer enabled"               OFF)
option(TA_ENABLE_WERROR     "Build with warnings as errors"                     OFF)

# Only do these if this is the main project,
# and not if it is included through add_subdirectory.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Caches build artifacts for faster builds
    find_program(CCACHE ccache)
    if (CCACHE)
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    endif ()

    # Set C11 with no extensions enabled. Fail if the
    # standard is not supported.
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    # Set C++17 with no extensions enabled. Fail if the
    # standard is not supported.
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # library settings
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(BUILD_SHARED_LIBS OFF)

    # misc
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Minimum macOS version
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CompilerOptions)
include(CompilerWarnings)
include(CodeCoverage)

add_library(ta_rubberband)
add_library(ta::rubberband ALIAS ta_rubberband)
target_sources(ta_rubberband PRIVATE 3rd_party/rubberband/single/RubberBandSingle.cpp)
target_compile_definitions(ta_rubberband PUBLIC NOMINMAX=1)
target_include_directories(ta_rubberband PUBLIC 3rd_party/rubberband)

add_subdirectory(3rd_party/JUCE)
set_directory_properties(PROPERTIES JUCE_COMPANY_COPYRIGHT "SPDX: GPL-3.0-only")
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME "tobantAudio")
set_directory_properties(PROPERTIES JUCE_COMPANY_WEBSITE "https://github.com/tobanteAudio/StiggiDJ")
set_directory_properties(PROPERTIES JUCE_COMPANY_EMAIL "tobanteAudio@gmail.com")

juce_add_gui_app(StiggiDJ
    PRODUCT_NAME "Stiggi DJ"
    BUNDLE_ID "com.tobanteAudio.stiggi-dj"
    VERSION ${CMAKE_PROJECT_VERSION}
    HARDENED_RUNTIME_ENABLED TRUE
)

target_sources(StiggiDJ
    PRIVATE
        "src/Main.cpp"
        "src/MainComponent.hpp"
        "src/MainComponent.cpp"

        "src/Core/Array.hpp"

        "src/DSP/DJPlayer.cpp"
        "src/DSP/DJPlayer.hpp"

        "src/UI/Component/JogWheel.cpp"
        "src/UI/Component/JogWheel.hpp"
        "src/UI/Component/Placeholder.cpp"
        "src/UI/Component/Placeholder.hpp"
        "src/UI/Component/WaveformDisplay.cpp"
        "src/UI/Component/WaveformDisplay.hpp"

        "src/UI/Section/Display.cpp"
        "src/UI/Section/Display.hpp"
        "src/UI/Section/MainSection.cpp"
        "src/UI/Section/MainSection.hpp"
        "src/UI/Section/SideBarLeft.cpp"
        "src/UI/Section/SideBarLeft.hpp"
        "src/UI/Section/SideBarRight.cpp"
        "src/UI/Section/SideBarRight.hpp"
)

target_compile_definitions(StiggiDJ
    PRIVATE
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:StiggiDJ,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:StiggiDJ,JUCE_VERSION>"
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_REPORT_APP_USAGE=0
        DONT_SET_USING_JUCE_NAMESPACE=1
        JUCE_USE_CURL=0
        JUCE_WEB_BROWSER=0
)

target_include_directories(StiggiDJ
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(StiggiDJ
    PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_utils
    PUBLIC
        ta::rubberband
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        ta::compiler_options
        ta::compiler_warnings
        ta::coverage
)
