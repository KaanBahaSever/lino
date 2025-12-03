#include "ReplaceDialog.h"
#include "MainWindow.h"
#include <FL/fl_ask.H>
#include <cstring>

ReplaceDialog::ReplaceDialog(MainWindow* main)
    : Fl_Window(400, 200, "Replace"), mainWindow(main), lastSearchPos(0) {
    
    // Find input
    findInput = new Fl_Input(80, 20, 300, 25, "Find:");
    
    // Replace input
    replaceInput = new Fl_Input(80, 55, 300, 25, "Replace:");
    
    // Case sensitive checkbox
    caseSensitive = new Fl_Check_Button(80, 90, 200, 25, "Case Sensitive");
    
    // Buttons
    findButton = new Fl_Button(20, 130, 85, 30, "Find");
    findButton->callback(find_cb, this);
    
    replaceButton = new Fl_Button(115, 130, 85, 30, "Replace");
    replaceButton->callback(replace_cb, this);
    
    replaceAllButton = new Fl_Button(210, 130, 85, 30, "Replace All");
    replaceAllButton->callback(replaceAll_cb, this);
    
    closeButton = new Fl_Button(305, 130, 75, 30, "Close");
    closeButton->callback(close_cb, this);
    
    end();
    set_modal();
}

ReplaceDialog::~ReplaceDialog() {
}

void ReplaceDialog::find_cb(Fl_Widget*, void* v) {
    ((ReplaceDialog*)v)->find();
}

void ReplaceDialog::replace_cb(Fl_Widget*, void* v) {
    ((ReplaceDialog*)v)->replace();
}

void ReplaceDialog::replaceAll_cb(Fl_Widget*, void* v) {
    ((ReplaceDialog*)v)->replaceAll();
}

void ReplaceDialog::close_cb(Fl_Widget*, void* v) {
    ((ReplaceDialog*)v)->hide();
}

void ReplaceDialog::find() {
    const char* searchText = findInput->value();
    if (strlen(searchText) == 0) {
        fl_alert("Please enter text to find.");
        return;
    }
    
    int pos = findInBuffer(lastSearchPos);
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

void ReplaceDialog::replace() {
    Fl_Text_Buffer* buffer = mainWindow->getTextBuffer();
    const char* searchText = findInput->value();
    const char* replaceText = replaceInput->value();
    
    if (strlen(searchText) == 0) {
        fl_alert("Please enter text to find.");
        return;
    }
    
    // Check if there's a selection
    int start, end;
    if (buffer->selection_position(&start, &end)) {
        char* selectedText = buffer->selection_text();
        bool matches = false;
        
        if (caseSensitive->value()) {
            matches = (strcmp(selectedText, searchText) == 0);
        } else {
            matches = (strcasecmp(selectedText, searchText) == 0);
        }
        
        if (matches) {
            buffer->replace_selection(replaceText);
            lastSearchPos = start + strlen(replaceText);
        }
        free(selectedText);
    }
    
    // Find next occurrence
    find();
}

void ReplaceDialog::replaceAll() {
    const char* searchText = findInput->value();
    const char* replaceText = replaceInput->value();
    
    if (strlen(searchText) == 0) {
        fl_alert("Please enter text to find.");
        return;
    }
    
    int count = 0;
    lastSearchPos = 0;
    
    while (true) {
        int pos = findInBuffer(lastSearchPos);
        if (pos < 0) break;
        
        Fl_Text_Buffer* buffer = mainWindow->getTextBuffer();
        buffer->select(pos, pos + strlen(searchText));
        buffer->remove_selection();
        buffer->insert(pos, replaceText);
        
        lastSearchPos = pos + strlen(replaceText);
        count++;
    }
    
    fl_message("Replaced %d occurrence(s).", count);
    lastSearchPos = 0;
}

int ReplaceDialog::findInBuffer(int startPos) {
    Fl_Text_Buffer* buffer = mainWindow->getTextBuffer();
    const char* searchText = findInput->value();
    int searchLen = strlen(searchText);
    
    if (searchLen == 0) return -1;
    
    const char* text = buffer->text();
    int textLen = buffer->length();
    
    if (startPos >= textLen) {
        free((void*)text);
        return -1;
    }
    
    bool matchCase = caseSensitive->value();
    
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
    
    free((void*)text);
    return -1;
}

void ReplaceDialog::show() {
    lastSearchPos = 0;
    Fl_Window::show();
    findInput->take_focus();
}

void ReplaceDialog::reset() {
    lastSearchPos = 0;
    findInput->value("");
    replaceInput->value("");
}
