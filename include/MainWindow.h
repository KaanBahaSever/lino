#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Menu_Bar.H>
#include <string>

class FindDialog;
class ReplaceDialog;

class MainWindow : public Fl_Double_Window {
private:
    Fl_Menu_Bar* menuBar;
    Fl_Text_Editor* textEditor;
    Fl_Text_Buffer* textBuffer;
    std::string currentFilePath;
    bool isModified;
    
    FindDialog* findDialog;
    ReplaceDialog* replaceDialog;
    
    // Text buffer callback
    static void textModified_cb(int pos, int nInserted, int nDeleted,
                                int nRestyled, const char* deletedText, void* cbArg);
    
    // Implementation methods
    void updateTitle();
    
public:
    MainWindow(int w, int h, const char* title);
    virtual ~MainWindow();
    
    void loadFile(const std::string& filepath);
    Fl_Text_Editor* getTextEditor() { return textEditor; }
    Fl_Text_Buffer* getTextBuffer() { return textBuffer; }
    
    // Public methods for menu callbacks
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void showFindDialog();
    void showReplaceDialog();
    void showAboutDialog();
    bool checkSaveBeforeAction();
};

#endif // MAINWINDOW_H
