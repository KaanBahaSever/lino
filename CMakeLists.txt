cmake_minimum_required(VERSION 3.15)
project(LinoNotepad VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Point to your FLTK installation
set(CMAKE_PREFIX_PATH "C:/fltk-static" CACHE PATH "Path to FLTK installation")

# Find FLTK package with static libraries
set(FLTK_USE_STATIC_LIBS TRUE)
find_package(FLTK REQUIRED)

# Include directories
include_directories(${FLTK_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/FindDialog.cpp
    src/ReplaceDialog.cpp
)

# Add Windows resource file
if(WIN32)
    set(SOURCES ${SOURCES} resources/app.rc)
endif()

# Header files
set(HEADERS
    include/MainWindow.h
    include/FindDialog.h
    include/ReplaceDialog.h
)

# Create executable
add_executable(lino ${SOURCES} ${HEADERS})

# Link FLTK libraries
target_link_libraries(lino ${FLTK_LIBRARIES})

# Platform-specific settings
if(WIN32)
    # Hide console window on Windows
    set_target_properties(lino PROPERTIES WIN32_EXECUTABLE TRUE)
    
    # Static linking to embed all DLLs
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    
    # Link Windows libraries required by FLTK
    target_link_libraries(lino 
        gdiplus  # GDI+ for graphics
        ole32    # OLE for clipboard
        uuid     # UUID for COM
        comctl32 # Common controls
        ws2_32   # Winsock
    )
    
    # Use static FLTK libraries
    set(FLTK_SKIP_FLUID TRUE)
    set(FLTK_SKIP_FORMS TRUE)
endif()

# Install target
install(TARGETS lino DESTINATION bin)

# Print build information
message(STATUS "==============================================")
message(STATUS "Lino Notepad Build Configuration")
message(STATUS "==============================================")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "FLTK Include Dir: ${FLTK_INCLUDE_DIR}")
message(STATUS "FLTK Libraries: ${FLTK_LIBRARIES}")
message(STATUS "==============================================")
