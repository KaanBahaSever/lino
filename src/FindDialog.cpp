#include "FindDialog.h"
#include "MainWindow.h"
#include <FL/fl_ask.H>
#include <cstring>

FindDialog::FindDialog(MainWindow* main)
    : Fl_Window(400, 150, "Find"), mainWindow(main), lastSearchPos(0) {
    
    // Find input
    findInput = new Fl_Input(80, 20, 300, 25, "Find:");
    findInput->callback(input_cb, this);
    findInput->when(FL_WHEN_ENTER_KEY);
    
    // Case sensitive checkbox
    caseSensitive = new Fl_Check_Button(80, 55, 200, 25, "Case Sensitive");
    
    // Buttons
    findNextButton = new Fl_Button(80, 95, 90, 30, "Find Next");
    findNextButton->callback(findNext_cb, this);
    
    findPrevButton = new Fl_Button(180, 95, 90, 30, "Find Prev");
    findPrevButton->callback(findPrev_cb, this);
    
    closeButton = new Fl_Button(280, 95, 90, 30, "Close");
    closeButton->callback(close_cb, this);
    
    end();
    set_modal();
}

FindDialog::~FindDialog() {
}

void FindDialog::findNext_cb(Fl_Widget*, void* v) {
    ((FindDialog*)v)->findNext();
}

void FindDialog::findPrev_cb(Fl_Widget*, void* v) {
    ((FindDialog*)v)->findPrev();
}

void FindDialog::close_cb(Fl_Widget*, void* v) {
    ((FindDialog*)v)->hide();
}

void FindDialog::input_cb(Fl_Widget*, void* v) {
    ((FindDialog*)v)->findNext();
}

void FindDialog::findNext() {
    const char* searchText = findInput->value();
    if (strlen(searchText) == 0) {
        fl_alert("Please enter text to find.");
        return;
    }
    
    int pos = findInBuffer(lastSearchPos, true);
    if (pos >= 0) {
        Fl_Text_Buffer* buffer = mainWindow->getTextBuffer();
        mainWindow->getTextEditor()->insert_position(pos);
        buffer->select(pos, pos + strlen(searchText));
        lastSearchPos = pos + 1;
    } else {
        fl_alert("Text not found.");
        lastSearchPos = 0;
    }
}

void FindDialog::findPrev() {
    const char* searchText = findInput->value();
    if (strlen(searchText) == 0) {
        fl_alert("Please enter text to find.");
        return;
    }
    
    int pos = findInBuffer(lastSearchPos - 2, false);
    if (pos >= 0) {
        Fl_Text_Buffer* buffer = mainWindow->getTextBuffer();
        mainWindow->getTextEditor()->insert_position(pos);
        buffer->select(pos, pos + strlen(searchText));
        lastSearchPos = pos;
    } else {
        fl_alert("Text not found.");
        Fl_Text_Buffer* buffer = mainWindow->getTextBuffer();
        lastSearchPos = buffer->length();
    }
}

int FindDialog::findInBuffer(int startPos, bool forward) {
    Fl_Text_Buffer* buffer = mainWindow->getTextBuffer();
    const char* searchText = findInput->value();
    int searchLen = strlen(searchText);
    
    if (searchLen == 0) return -1;
    
    const char* text = buffer->text();
    int textLen = buffer->length();
    
    if (startPos < 0) startPos = textLen - 1;
    if (startPos >= textLen) startPos = 0;
    
    bool matchCase = caseSensitive->value();
    
    if (forward) {
        for (int i = startPos; i <= textLen - searchLen; i++) {
            bool match = true;
            for (int j = 0; j < searchLen; j++) {
                char c1 = text[i + j];
                char c2 = searchText[j];
                if (!matchCase) {
                    c1 = tolower(c1);
                    c2 = tolower(c2);
                }
                if (c1 != c2) {
                    match = false;
                    break;
                }
            }
            if (match) {
                free((void*)text);
                return i;
            }
        }
    } else {
        for (int i = startPos; i >= 0; i--) {
            bool match = true;
            for (int j = 0; j < searchLen; j++) {
                char c1 = text[i + j];
                char c2 = searchText[j];
                if (!matchCase) {
                    c1 = tolower(c1);
                    c2 = tolower(c2);
                }
                if (c1 != c2) {
                    match = false;
                    break;
                }
            }
            if (match) {
                free((void*)text);
                return i;
            }
        }
    }
    
    free((void*)text);
    return -1;
}

void FindDialog::show() {
    lastSearchPos = 0;
    Fl_Window::show();
    findInput->take_focus();
}

void FindDialog::reset() {
    lastSearchPos = 0;
    findInput->value("");
}
