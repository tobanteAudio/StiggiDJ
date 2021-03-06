add_library(ta_compiler_warnings INTERFACE)
add_library(ta::compiler_warnings ALIAS ta_compiler_warnings)

if((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
    target_compile_options(ta_compiler_warnings INTERFACE /W3)
    if (TA_ENABLE_WERROR)
        target_compile_options(ta_compiler_warnings INTERFACE /WX)
    endif (TA_ENABLE_WERROR)
else ()
    target_compile_options(ta_compiler_warnings
        INTERFACE
            -Wall
            -Wcast-align
            -Wextra
            -Wnarrowing
            -Woverloaded-virtual
            -Wpedantic
            -Wreorder
            -Wshadow
            -Wsign-compare
            -Wno-sign-conversion
            -Wstrict-aliasing
            -Wswitch-enum
            -Wuninitialized
            -Wunreachable-code
            -Wunused-parameter
            -Wzero-as-null-pointer-constant
            $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
                -Wbool-conversion
                -Wconditional-uninitialized
                -Wconstant-conversion
                # -Wconversion
                -Wextra-semi
                -Winconsistent-missing-destructor-override
                -Wint-conversion
                -Wnullable-to-nonnull-conversion
                -Wshadow-all
                -Wshift-sign-overflow
                -Wshorten-64-to-32
                -Wunused-private-field
            >
            $<$<CXX_COMPILER_ID:AppleClang>:
                -Wno-poison-system-directories
            >
            $<$<CXX_COMPILER_ID:GNU>:
                # -Wlogical-op
                -Wno-maybe-uninitialized
                -Wredundant-decls
                -Wno-free-nonheap-object
                $<$<VERSION_GREATER:CXX_COMPILER_VERSION,"11.0.0">:-Wno-free-nonheap-object>
            >
    )
    if (TA_ENABLE_WERROR)
        target_compile_options(ta_compiler_warnings INTERFACE -Werror)
    endif (TA_ENABLE_WERROR)
endif ()
