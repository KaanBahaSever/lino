#include "MainWindow.h"
#include "FindDialog.h"
#include "ReplaceDialog.h"
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <fstream>
#include <sstream>

// Forward declarations for menu callbacks
static void newFile_cb(Fl_Widget *, void *v);
static void openFile_cb(Fl_Widget *, void *v);
static void saveFile_cb(Fl_Widget *, void *v);
static void saveFileAs_cb(Fl_Widget *, void *v);
static void exit_cb(Fl_Widget *, void *v);
static void find_cb(Fl_Widget *, void *v);
static void replace_cb(Fl_Widget *, void *v);
static void about_cb(Fl_Widget *, void *v);

// Menu items structure
static Fl_Menu_Item menuItems[] = {
    {"&File", 0, 0, 0, FL_SUBMENU},
    {"&New", FL_COMMAND + 'n', newFile_cb, 0},
    {"&Open...", FL_COMMAND + 'o', openFile_cb, 0},
    {"&Save", FL_COMMAND + 's', saveFile_cb, 0},
    {"Save &As...", FL_COMMAND + FL_SHIFT + 's', saveFileAs_cb, 0},
    {0},
    {"&Edit", 0, 0, 0, FL_SUBMENU},
    {"&Find...", FL_COMMAND + 'f', find_cb, 0},
    {"&Replace...", FL_COMMAND + 'h', replace_cb, 0},
    {0},
    {"&Help", 0, 0, 0, FL_SUBMENU},
    {"&About", 0, about_cb, 0},
    {0},
    {0}};

MainWindow::MainWindow(int w, int h, const char *title)
    : Fl_Double_Window(w, h, title), isModified(false), findDialog(nullptr), replaceDialog(nullptr)
{
    // Set window background to white
    color(FL_WHITE);

    // Create menu bar
    menuBar = new Fl_Menu_Bar(0, 0, w, 30);
    menuBar->copy(menuItems, this);
    menuBar->box(FL_FLAT_BOX);

    // Create text buffer and editor
    textBuffer = new Fl_Text_Buffer();
    textBuffer->add_modify_callback(textModified_cb, this);

    // Add padding: left=10, right=10
    int leftPadding = 5;
    int rightPadding = 5;
    textEditor = new Fl_Text_Editor(leftPadding, 35, w - leftPadding - rightPadding, h - 35);
    textEditor->buffer(textBuffer);
    textEditor->textfont(FL_COURIER);
    textEditor->textsize(16);
    textEditor->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
    textEditor->color(FL_WHITE);  // Set text editor background to white
    textEditor->textcolor(FL_BLACK);  // Set text color to black
    textEditor->box(FL_FLAT_BOX);  // Remove border

    // Modern cursor style
    textEditor->cursor_style(Fl_Text_Display::SIMPLE_CURSOR);
    textEditor->cursor_color(FL_BLACK);

    // Set window properties
    resizable(textEditor);
    end();

    // Initialize dialogs
    findDialog = new FindDialog(this);
    replaceDialog = new ReplaceDialog(this);

    // Set window callback for close button
    callback(exit_cb, this);

    updateTitle();
}

MainWindow::~MainWindow()
{
    delete findDialog;
    delete replaceDialog;
    delete textBuffer;
}

// Static callback implementations
static void newFile_cb(Fl_Widget *, void *v)
{
    ((MainWindow *)v)->newFile();
}

static void openFile_cb(Fl_Widget *, void *v)
{
    ((MainWindow *)v)->openFile();
}

static void saveFile_cb(Fl_Widget *, void *v)
{
    ((MainWindow *)v)->saveFile();
}

static void saveFileAs_cb(Fl_Widget *, void *v)
{
    ((MainWindow *)v)->saveFileAs();
}

static void exit_cb(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;
    if (win->checkSaveBeforeAction())
    {
        win->hide();
    }
}

static void find_cb(Fl_Widget *, void *v)
{
    ((MainWindow *)v)->showFindDialog();
}

static void replace_cb(Fl_Widget *, void *v)
{
    ((MainWindow *)v)->showReplaceDialog();
}

static void about_cb(Fl_Widget *, void *v)
{
    ((MainWindow *)v)->showAboutDialog();
}

void MainWindow::textModified_cb(int, int, int, int, const char *, void *cbArg)
{
    MainWindow *win = (MainWindow *)cbArg;
    win->isModified = true;
    win->updateTitle();
}

void MainWindow::newFile()
{
    if (checkSaveBeforeAction())
    {
        textBuffer->text("");
        currentFilePath.clear();
        isModified = false;
        updateTitle();
    }
}

void MainWindow::openFile()
{
    if (!checkSaveBeforeAction())
        return;

    Fl_Native_File_Chooser chooser;
    chooser.title("Open File");
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    chooser.filter("Text Files\t*.txt\nAll Files\t*");
    
    if (chooser.show() == 0)
    {
        loadFile(chooser.filename());
    }
}

void MainWindow::loadFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        textBuffer->text(buffer.str().c_str());
        currentFilePath = filepath;
        isModified = false;
        updateTitle();
        file.close();
    }
    else
    {
        fl_alert("Cannot open file: %s", filepath.c_str());
    }
}

void MainWindow::saveFile()
{
    if (currentFilePath.empty())
    {
        saveFileAs();
    }
    else
    {
        std::ofstream file(currentFilePath);
        if (file.is_open())
        {
            file << textBuffer->text();
            isModified = false;
            updateTitle();
            file.close();
        }
        else
        {
            fl_alert("Cannot save file: %s", currentFilePath.c_str());
        }
    }
}

void MainWindow::saveFileAs()
{
    Fl_Native_File_Chooser chooser;
    chooser.title("Save File As");
    chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    chooser.filter("Text Files\t*.txt\nAll Files\t*");
    chooser.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM);
    
    if (chooser.show() == 0)
    {
        currentFilePath = chooser.filename();
        saveFile();
    }
}

void MainWindow::showFindDialog()
{
    // Center dialog on text editor
    int dialog_x = textEditor->x() + (textEditor->w() - findDialog->w()) / 2;
    int dialog_y = textEditor->y() + (textEditor->h() - findDialog->h()) / 2;
    findDialog->position(dialog_x + x(), dialog_y + y());
    findDialog->show();
}

void MainWindow::showReplaceDialog()
{
    // Center dialog on text editor
    int dialog_x = textEditor->x() + (textEditor->w() - replaceDialog->w()) / 2;
    int dialog_y = textEditor->y() + (textEditor->h() - replaceDialog->h()) / 2;
    replaceDialog->position(dialog_x + x(), dialog_y + y());
    replaceDialog->show();
}

void MainWindow::showAboutDialog()
{
    // Center the message dialog by setting message position
    fl_message_position(x() + w() / 2, y() + h() / 2);
    fl_message("Lino Notepad\n\nA simple text editor built with FLTK\n\n"
               "Features:\n"
               "- Basic text editing\n"
               "- Find and Replace\n"
               "- Keyboard shortcuts\n\n"
               "Version 1.0");
}

void MainWindow::updateTitle()
{
    std::string title;
    if (currentFilePath.empty())
    {
        title = "Untitled";
    }
    else
    {
        // Extract filename from path
        size_t pos = currentFilePath.find_last_of("/\\");
        title = (pos != std::string::npos) ? currentFilePath.substr(pos + 1) : currentFilePath;
    }

    if (isModified)
    {
        title = "* " + title;
    }
    title += " - Lino Notepad";
    copy_label(title.c_str());
}

bool MainWindow::checkSaveBeforeAction()
{
    if (isModified)
    {
        int choice = fl_choice("Do you want to save changes?", "Don't Save", "Cancel", "Save");
        if (choice == 1)
        {
            return false; // Cancel
        }
        else if (choice == 2)
        {
            saveFile();
            return !isModified; // Return true if save succeeded
        }
        // choice == 0 means "Don't Save", continue without saving
    }
    return true;
}


