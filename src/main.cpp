#include "MainWindow.h"
#include <FL/Fl.H>

int main(int argc, char** argv) {
    // Create main window
    MainWindow* window = new MainWindow(800, 600, "Lino Notepad");
    
    // If a file is passed as argument, load it
    if (argc > 1) {
        window->loadFile(argv[1]);
    }
    
    // Show window and run application
    window->show();
    return Fl::run();
}
