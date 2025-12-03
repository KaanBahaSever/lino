# Lino Notepad

A simple, lightweight text editor built with FLTK (Fast Light Toolkit) featuring search and replace functionality.

## Features

- **Basic Text Editing**: Full-featured text editor with syntax highlighting support
- **Find and Replace**: Comprehensive search functionality with case-sensitive options
- **Keyboard Shortcuts**: Quick access to common operations
- **Cross-Platform**: Runs on Windows, Linux, and macOS
- **Lightweight**: Fast startup and minimal resource usage

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
