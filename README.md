# Lino Notepad

A simple, lightweight text editor built with FLTK (Fast Light Toolkit) featuring search and replace functionality.

## Features

- **Basic Text Editing**: Full-featured text editor with syntax highlighting support
- **Find and Replace**: Comprehensive search functionality with case-sensitive options
- **Keyboard Shortcuts**: Quick access to common operations
- **Cross-Platform**: Runs on Windows, Linux, and macOS
- **Lightweight**: Fast startup and minimal resource usage

## Prerequisites

Before building Lino Notepad, you need to install the following:

### Windows

1. **CMake** (3.15 or higher)
   - Download from [cmake.org](https://cmake.org/download/)
   - Or install via `winget`: `winget install Kitware.CMake`

2. **FLTK** (Fast Light Toolkit)
   - Download from [fltk.org](https://www.fltk.org/software.php)
   - Or build from source:
     ```powershell
     git clone https://github.com/fltk/fltk.git
     cd fltk
     mkdir build; cd build
     cmake .. -G "MinGW Makefiles"
     cmake --build . --config Release
     cmake --install . --prefix "C:/Program Files/FLTK"
     ```

3. **C++ Compiler** (MinGW-w64 or MSVC)
   - MinGW-w64: Download from [mingw-w64.org](https://www.mingw-w64.org/)
   - Or MSVC: Install Visual Studio with C++ workload

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libfltk1.3-dev
```

### Linux (Fedora/RedHat)

```bash
sudo dnf install gcc-c++ cmake fltk-devel
```

### macOS

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake fltk
```

## Building

### Step 1: Clone or navigate to the project directory

```powershell
cd c:\Users\BAHA\Desktop\lino
```

### Step 2: Create build directory

```powershell
mkdir build
cd build
```

### Step 3: Configure with CMake

**For MinGW (Windows):**
```powershell
cmake .. -G "MinGW Makefiles"
```

**For MSVC (Windows):**
```powershell
cmake .. -G "Visual Studio 17 2022"
```

**For Linux/macOS:**
```bash
cmake ..
```

### Step 4: Build the project

**For MinGW:**
```powershell
cmake --build . --config Release
```

**For MSVC:**
```powershell
cmake --build . --config Release
```

**For Linux/macOS:**
```bash
make
```

### Step 5: Run the application

**Windows:**
```powershell
.\lino.exe
```

**Linux/macOS:**
```bash
./lino
```

## Installation

To install Lino Notepad system-wide:

```bash
sudo cmake --install .
```

Or specify a custom installation directory:

```bash
cmake --install . --prefix /your/custom/path
```

## Usage

### Basic Operations

- **New File**: `Ctrl+N` or File → New
- **Open File**: `Ctrl+O` or File → Open
- **Save File**: `Ctrl+S` or File → Save
- **Save As**: `Ctrl+Shift+S` or File → Save As
- **Exit**: `Ctrl+Q` or File → Exit

### Find and Replace

- **Find**: `Ctrl+F` or Edit → Find
- **Replace**: `Ctrl+H` or Edit → Replace

#### Find Dialog
- Enter text to search
- Toggle "Case Sensitive" for exact matches
- Click "Find Next" or "Find Prev" to navigate results
- Press Enter to find next occurrence

#### Replace Dialog
- Enter text to find and replacement text
- Click "Find" to locate next occurrence
- Click "Replace" to replace current selection
- Click "Replace All" to replace all occurrences at once

### Opening Files from Command Line

You can open a file directly by passing it as an argument:

```powershell
.\lino.exe myfile.txt
```

## Project Structure

```
lino/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── include/                # Header files
│   ├── MainWindow.h        # Main window class
│   ├── FindDialog.h        # Find dialog class
│   └── ReplaceDialog.h     # Replace dialog class
├── src/                    # Source files
│   ├── main.cpp            # Application entry point
│   ├── MainWindow.cpp      # Main window implementation
│   ├── FindDialog.cpp      # Find dialog implementation
│   └── ReplaceDialog.cpp   # Replace dialog implementation
└── build/                  # Build directory (created during build)
```

## Troubleshooting

### FLTK not found

If CMake cannot find FLTK, you can specify its location:

```powershell
cmake .. -DFLTK_DIR="C:/Program Files/FLTK/lib/cmake/fltk"
```

### Missing DLLs on Windows

If you get DLL errors when running the application, make sure the FLTK DLLs are in your PATH or copy them to the same directory as the executable.

### Build Errors

1. Make sure all prerequisites are installed
2. Clear the build directory and try again:
   ```powershell
   cd build
   rm -r *
   cmake ..
   cmake --build .
   ```

## Development

### Adding New Features

1. Add header files to `include/`
2. Add implementation files to `src/`
3. Update `CMakeLists.txt` to include new files
4. Rebuild the project

### Code Style

- Use meaningful variable names
- Follow C++17 standards
- Add comments for complex logic
- Keep functions focused and concise

## License

This project is provided as-is for educational and personal use.

## Acknowledgments

- Built with [FLTK](https://www.fltk.org/) - Fast Light Toolkit
- Uses CMake for cross-platform building

## Version History

- **1.0** (2025-12-03): Initial release
  - Basic text editing
  - Find and replace functionality
  - File operations (new, open, save)
  - Keyboard shortcuts

## Contact

For issues or suggestions, please create an issue in the project repository.
