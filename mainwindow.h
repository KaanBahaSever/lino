#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFont>
#include <QKeyEvent>
#include <QResizeEvent> // Include this for QResizeEvent

class FindBar; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPlainTextEdit *plainTextEdit; // Move from private to public
    QString currentFilePath;       // Move from private to public

protected:
    void keyPressEvent(QKeyEvent *event) override; // Add this
    void resizeEvent(QResizeEvent *event) override; // Add this line
    void closeEvent(QCloseEvent *event) override; // Eklendi

private slots:
    void showFindDialog();
    void showReplaceDialog();
    void findNext();
    void increaseFontSize();
    void decreaseFontSize();
    void resetFontSize();
    void showAboutDialog();
    void findBarFindNext();
    void findBarFindPrev();
    void findBarHide();
    void showFontDialog();
    void newFile();      // Add this
    void openFile();     // Add this
    void saveFile();     // Add this
    void quickSave(); // Add this
    void toggleReplacePanel(); // Yeni slot
    void findBarReplaceAll();  // Yeni slot
    void findBarReplaceNext(); // Yeni slot

private:
    // QPlainTextEdit *plainTextEdit; // Remove this line
    // QString currentFilePath;       // Remove this line
    QString lastFindText;
    int lastFindPos;
    QFont editorFont;
    void setupMenus();
    FindBar *findBar;
    void showFindBar();
    void findAllMatches(const QString &text);
    QVector<int> matchPositions;
    int currentMatchIndex;
    void setFindBarHeight(bool expanded); // Yeni yardımcı fonksiyon
    bool isModified; // Eklendi
};

#endif // MAINWINDOW_H
