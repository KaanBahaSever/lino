#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>

class MainWindow;

class ReplaceDialog : public Fl_Window {
private:
    MainWindow* mainWindow;
    Fl_Input* findInput;
    Fl_Input* replaceInput;
    Fl_Check_Button* caseSensitive;
    Fl_Button* findButton;
    Fl_Button* replaceButton;
    Fl_Button* replaceAllButton;
    Fl_Button* closeButton;
    
    int lastSearchPos;
    
    static void find_cb(Fl_Widget*, void* v);
    static void replace_cb(Fl_Widget*, void* v);
    static void replaceAll_cb(Fl_Widget*, void* v);
    static void close_cb(Fl_Widget*, void* v);
    
    void find();
    void replace();
    void replaceAll();
    int findInBuffer(int startPos);
    
public:
    ReplaceDialog(MainWindow* main);
    virtual ~ReplaceDialog();
    
    void show();
    void reset();
};

#endif // REPLACEDIALOG_H
