#include "mainwindow.h"
#include <QFontDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextCursor>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

// --- FindBar widget ---
class FindBar : public QWidget {
    // Q_OBJECT
public:
    QLineEdit *lineEdit;
    QPushButton *btnPrev;
    QPushButton *btnNext;
    QPushButton *btnClose;
    QLabel *countLabel;
    explicit FindBar(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedHeight(36);
        setStyleSheet("background:#f0f0f0; border:1px solid #bbb; border-radius:6px;");
        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(8, 4, 8, 4);
        lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText("Find...");
        btnPrev = new QPushButton("▲", this);
        btnNext = new QPushButton("▼", this);
        btnClose = new QPushButton("✕", this);
        countLabel = new QLabel(this);
        btnPrev->setFixedSize(28, 28);
        btnNext->setFixedSize(28, 28);
        btnClose->setFixedSize(28, 28);
        layout->addWidget(lineEdit);
        layout->addWidget(countLabel);
        layout->addWidget(btnPrev);
        layout->addWidget(btnNext);
        layout->addWidget(btnClose);
    }
};
// --- End FindBar widget ---

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    findBar = new FindBar(this); // parent'ı this!
    findBar->setFixedWidth(350);
    findBar->hide();

    plainTextEdit = new QPlainTextEdit(centralWidget);
    QFont initialFont = plainTextEdit->font();
    initialFont.setPointSize(12);
    plainTextEdit->setFont(initialFont);
    plainTextEdit->setStyleSheet("QPlainTextEdit { padding: 5px; }");
    layout->addWidget(plainTextEdit);

    editorFont = plainTextEdit->font();
    lastFindPos = 0;

    connect(findBar->btnClose, &QPushButton::clicked, this, &MainWindow::findBarHide);
    connect(findBar->btnNext, &QPushButton::clicked, this, &MainWindow::findBarFindNext);
    connect(findBar->btnPrev, &QPushButton::clicked, this, &MainWindow::findBarFindPrev);
    connect(findBar->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::findBarFindNext);
    connect(findBar->lineEdit, &QLineEdit::textChanged, [this](const QString &text){
        findAllMatches(text);
    });

    matchPositions.clear();
    currentMatchIndex = -1;

    setupMenus();
    currentFilePath.clear(); // Add this to track the current file path
}

void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *newAction = fileMenu->addAction(QIcon(), tr("New"));
    QAction *openAction = fileMenu->addAction(QIcon(), tr("Open..."));
    QAction *saveAction = fileMenu->addAction(QIcon(), tr("Save As..."));

    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QAction *findAction = editMenu->addAction(QIcon(), tr("Find..."));
    QAction *replaceAction = editMenu->addAction(QIcon(), tr("Replace..."));
    editMenu->addSeparator();
    QAction *fontAction = editMenu->addAction(QIcon(), tr("Font...")); // Add font action
    QAction *incFontAction = editMenu->addAction(QIcon(), tr("Increase Font Size"));
    QAction *decFontAction = editMenu->addAction(QIcon(), tr("Decrease Font Size"));
    QAction *resetFontAction = editMenu->addAction(QIcon(), tr("Reset Font Size"));

    connect(findAction, &QAction::triggered, this, &MainWindow::showFindDialog);
    connect(replaceAction, &QAction::triggered, this, &MainWindow::showReplaceDialog);
    connect(fontAction, &QAction::triggered, this, &MainWindow::showFontDialog); // Connect font action
    connect(incFontAction, &QAction::triggered, this, &MainWindow::increaseFontSize);
    connect(decFontAction, &QAction::triggered, this, &MainWindow::decreaseFontSize);
    connect(resetFontAction, &QAction::triggered, this, &MainWindow::resetFontSize);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAction = helpMenu->addAction(QIcon(), tr("About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);

    // Sadece menülere stil uygula, menuBar'a değil!
    QString menuStyle =
        "QMenu::icon { width: 0px; }"
        "QMenu::item { padding: 0px 10px 0px 10px; min-height: 32px; }"
        "QMenu::item:selected { background-color: #e0f0ff; }"
        "QMenu { border-radius: 0px; }"; // border-radius sıfırlandı
    editMenu->setStyleSheet(menuStyle);
    helpMenu->setStyleSheet(menuStyle);
    fileMenu->setStyleSheet(menuStyle);
}

void MainWindow::showFindDialog()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Find"), tr("Find what:"), QLineEdit::Normal, lastFindText, &ok);
    if (ok && !text.isEmpty()) {
        lastFindText = text;
        lastFindPos = 0;
        findNext();
    }
}

void MainWindow::findNext()
{
    if (lastFindText.isEmpty())
        return;
    QString content = plainTextEdit->toPlainText();
    int idx = content.indexOf(lastFindText, lastFindPos, Qt::CaseSensitive);
    if (idx != -1) {
        QTextCursor cursor = plainTextEdit->textCursor();
        cursor.setPosition(idx);
        cursor.setPosition(idx + lastFindText.length(), QTextCursor::KeepAnchor);
        plainTextEdit->setTextCursor(cursor);
        lastFindPos = idx + lastFindText.length();
    } else {
        QMessageBox::information(this, tr("Find"), tr("No more occurrences found."));
        lastFindPos = 0;
    }
}

void MainWindow::showReplaceDialog()
{
    bool ok;
    QString findText = QInputDialog::getText(this, tr("Replace"), tr("Find what:"), QLineEdit::Normal, lastFindText, &ok);
    if (!ok || findText.isEmpty())
        return;
    QString replaceText = QInputDialog::getText(this, tr("Replace"), tr("Replace with:"), QLineEdit::Normal, "", &ok);
    if (!ok)
        return;
    lastFindText = findText;
    lastFindPos = 0;
    QString content = plainTextEdit->toPlainText();
    int idx = content.indexOf(findText, lastFindPos, Qt::CaseSensitive);
    if (idx != -1) {
        QTextCursor cursor = plainTextEdit->textCursor();
        cursor.setPosition(idx);
        cursor.setPosition(idx + findText.length(), QTextCursor::KeepAnchor);
        plainTextEdit->setTextCursor(cursor);
        cursor.insertText(replaceText);
        lastFindPos = idx + replaceText.length();
    } else {
        QMessageBox::information(this, tr("Replace"), tr("Text not found."));
        lastFindPos = 0;
    }
}

void MainWindow::increaseFontSize()
{
    int size = editorFont.pointSize();
    editorFont.setPointSize(size + 1);
    plainTextEdit->setFont(editorFont);
}

void MainWindow::decreaseFontSize()
{
    int size = editorFont.pointSize();
    if (size > 1) {
        editorFont.setPointSize(size - 1);
        plainTextEdit->setFont(editorFont);
    }
}

void MainWindow::resetFontSize()
{
    editorFont.setPointSize(plainTextEdit->font().defaultFamily() == "Monospace" ? 10 : 12);
    plainTextEdit->setFont(editorFont);
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, tr("About"), tr("Simple Qt Text Editor\nFind/Replace, Font Size, About Menu Example."));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F) {
        showFindBar();
        return;
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S) {
        quickSave();
        return;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::showFindBar()
{
    int x = (width() - findBar->width()) / 2;
    int y = menuBar()->height() + 8;
    findBar->move(x, y);
    findBar->show();
    findBar->raise();
    findBar->lineEdit->setFocus();
    findBar->lineEdit->selectAll();
    findAllMatches(findBar->lineEdit->text());

    // FindBar yüksekliği kadar padding ekle
    int padding = findBar->height() + 8;
    plainTextEdit->setStyleSheet(QString("QPlainTextEdit { padding: 5px; padding-top: %1px; }").arg(padding));
}

// Pencere yeniden boyutlaninca FindBar'ı tekrar ortala
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (findBar && findBar->isVisible()) {
        int x = (width() - findBar->width()) / 2;
        int y = menuBar()->height() + 8;
        findBar->move(x, y);
    }
}

void MainWindow::findBarHide()
{
    findBar->hide();
    plainTextEdit->setFocus();
    // Padding'i eski haline döndür
    plainTextEdit->setStyleSheet("QPlainTextEdit { padding: 5px; }");
}

void MainWindow::findAllMatches(const QString &text)
{
    matchPositions.clear();
    currentMatchIndex = -1;
    QString content = plainTextEdit->toPlainText();
    if (text.isEmpty()) {
        findBar->countLabel->setText("");
        return;
    }
    int pos = 0;
    while ((pos = content.indexOf(text, pos, Qt::CaseSensitive)) != -1) {
        matchPositions.append(pos);
        pos += text.length();
    }
    if (!matchPositions.isEmpty()) {
        findBar->countLabel->setText(QString("%1/%2").arg(1).arg(matchPositions.size()));
        currentMatchIndex = 0;
        // Highlight first matchetText(QString("%1/%2").arg(currentMatchIndex + 1).arg(matchPositions.size()));
        QTextCursor cursor = plainTextEdit->textCursor();
        cursor.setPosition(matchPositions[0]);
        cursor.setPosition(matchPositions[0] + text.length(), QTextCursor::KeepAnchor);
        plainTextEdit->setTextCursor(cursor);
    } else {
        findBar->countLabel->setText("0/0");
    }
}

void MainWindow::findBarFindNext()
{
    if (matchPositions.isEmpty()) return;
    QString text = findBar->lineEdit->text();
    if (currentMatchIndex < 0) currentMatchIndex = 0;
    else currentMatchIndex = (currentMatchIndex + 1) % matchPositions.size();
    findBar->countLabel->setText(QString("%1/%2").arg(currentMatchIndex + 1).arg(matchPositions.size()));
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.setPosition(matchPositions[currentMatchIndex]);
    cursor.setPosition(matchPositions[currentMatchIndex] + text.length(), QTextCursor::KeepAnchor);
    plainTextEdit->setTextCursor(cursor);
}

void MainWindow::findBarFindPrev()
{
    if (matchPositions.isEmpty()) return;
    QString text = findBar->lineEdit->text();
    if (currentMatchIndex < 0) currentMatchIndex = 0;
    else currentMatchIndex = (currentMatchIndex - 1 + matchPositions.size()) % matchPositions.size();
    findBar->countLabel->setText(QString("%1/%2").arg(currentMatchIndex + 1).arg(matchPositions.size()));
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.setPosition(matchPositions[currentMatchIndex]);
    cursor.setPosition(matchPositions[currentMatchIndex] + text.length(), QTextCursor::KeepAnchor);
    plainTextEdit->setTextCursor(cursor);
}

void MainWindow::showFontDialog()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, editorFont, this, tr("Select Font"));
    if (ok) {
        editorFont = font;
        plainTextEdit->setFont(editorFont);
    }
}

void MainWindow::newFile()
{
    plainTextEdit->clear();
    currentFilePath.clear();
    setWindowTitle("Untitled - Qt Text Editor");
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            plainTextEdit->setPlainText(in.readAll());
            file.close();
            currentFilePath = fileName;
            setWindowTitle(QFileInfo(fileName).fileName() + " - Qt Text Editor");
        }
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), currentFilePath.isEmpty() ? "untitled.txt" : currentFilePath, tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << plainTextEdit->toPlainText();
            file.close();
            currentFilePath = fileName;
            setWindowTitle(QFileInfo(fileName).fileName() + " - Qt Text Editor");
        }
    }
}

void MainWindow::quickSave()
{
    if (!currentFilePath.isEmpty() && QFile::exists(currentFilePath)) {
        QFile file(currentFilePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << plainTextEdit->toPlainText();
            file.close();
            setWindowTitle(QFileInfo(currentFilePath).fileName() + " - Qt Text Editor");
        }
    } else {
        saveFile();
    }
}

MainWindow::~MainWindow() {}
