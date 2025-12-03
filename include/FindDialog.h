#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>

class MainWindow;

class FindDialog : public Fl_Window {
private:
    MainWindow* mainWindow;
    Fl_Input* findInput;
    Fl_Check_Button* caseSensitive;
    Fl_Button* findNextButton;
    Fl_Button* findPrevButton;
    Fl_Button* closeButton;
    
    int lastSearchPos;
    
    static void findNext_cb(Fl_Widget*, void* v);
    static void findPrev_cb(Fl_Widget*, void* v);
    static void close_cb(Fl_Widget*, void* v);
    static void input_cb(Fl_Widget*, void* v);
    
    void findNext();
    void findPrev();
    int findInBuffer(int startPos, bool forward);
    
public:
    FindDialog(MainWindow* main);
    virtual ~FindDialog();
    
    void show();
    void reset();
};

#endif // FINDDIALOG_H
