#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QTextCharFormat>
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QColorDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>

#include "paintarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Window and UI setup
    ui->setupUi(this);
    this->setCentralWidget(ui->centralwidget);

    createActions();
    createMenus();

    QFont myFont("MS Shell Dlg 2",8,50,0);
    ui->textEdit->setFont(myFont);

    connect(ui->italics, SIGNAL(toggled(bool)),
        this, SLOT(makeItalic(bool)));
    connect(ui->underline, SIGNAL(toggled(bool)),
        this, SLOT(makeUnderline(bool)));
    connect(ui->bold, SIGNAL(toggled(bool)),
        this, SLOT(makeBold(bool)));
    connect(ui->fontSize, SIGNAL(valueChanged(int)),
        this, SLOT(changeFontSize(int)));

    connect(ui->paintBox, SIGNAL(toggled(bool)),
        this, SLOT(togglePaint(bool) ));
    connect(ui->brushBox, SIGNAL(clicked()),
        this, SLOT(changeBrushWidth()));
    connect(ui->colorBox, SIGNAL(clicked()),
        this, SLOT(changeBrushColor()));

}
//Font Property Methods
void MainWindow::makeBold(bool value)
{
    QTextCharFormat format;
    if (value)
    format.setFontWeight(99);
    else
    format.setFontWeight(50);


    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);

    ui->textEdit->setFocus();
}

void MainWindow::makeItalic(bool value){
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    ui->textEdit->setFontItalic(value);
    ui->textEdit->setFocus();
}

void MainWindow::makeUnderline(bool value){
    ui->textEdit->setFontUnderline(value);
    ui->textEdit->setFocus();
}
void MainWindow::changeFontSize(int value){
    QTextCharFormat format;
    format.setFontPointSize(value);

    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);

    ui->textEdit->setFocus();
    ui->textEdit->setFocus();
}
//Brush Menu Methods
void MainWindow::changeBrushColor()
{
    if (PaintArea)
    {
    const QColor newColor = QColorDialog::getColor(PaintArea->brushColor());
    if (newColor.isValid())
        PaintArea->setBrushColor(newColor);
    }
    else
    QMessageBox::warning(this,"Error","Paint Area not active");
}

void MainWindow::changeBrushWidth()
{
    if (PaintArea)
    {
    bool ok;
    const int newWidth = QInputDialog::getInt(this, tr("Plug & Paint"),
                                              tr("Select brush width:"),
                                              PaintArea->brushWidth(),
                                              1, 50, 1, &ok);
    if (ok)
        PaintArea->setBrushWidth(newWidth);
    }
    else
    QMessageBox::warning(this,"Error","Paint Area not active");
}
//Enable Painting
void MainWindow::togglePaint(bool value){
    if (value)
    {


        PaintArea = new class PaintArea;
        scrollArea = new QScrollArea;
        ui->centralwidget->layout()->addWidget(scrollArea);

        scrollArea->setWidget(PaintArea);
        scrollArea->setAttribute(Qt::WA_NoSystemBackground,true);

        PaintArea->resize(ui->textEdit->size());
        scrollArea->show();

    }
    else
    {
        delete PaintArea;
        delete scrollArea;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
//File IO Methods
void MainWindow::newFile()
{
    currentFile = ""; //The File that is currently opened. Will be used in other functions.
    ui->textEdit->setText("");
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Select a File","","Text Files(*.txt);;Other()");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;
    QTextStream fileText(&file);
    QString text = fileText.readAll();
    ui->textEdit->setText(text);
    file.close();
    changeTitle();
}

void MainWindow::saveText()
{
    QString fileName;

    if (currentFile == "")//Check if a file is already open, if not find a file to save to
    fileName = QFileDialog::getSaveFileName(this,"Select a File","","Text Files(*.txt);;Other()");
    else
    fileName = currentFile;

    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;
    QTextStream fileTextOut(&file);
    QString text = ui->textEdit->toPlainText();
    fileTextOut << text;
    file.flush();
    file.close();
}

void MainWindow::saveTextAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Select a File","","Text Files(*.txt);;Other()");
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;
    QTextStream fileTextOut(&file);
    QString text = ui->textEdit->toPlainText();
    fileTextOut << text;
    file.flush();
    file.close();
    currentFile = fileName;
}

bool MainWindow::saveImage()
{
    QString fileName;
    if (PaintArea)
    {
    fileName = QFileDialog::getSaveFileName(this,"Save As","","Image Files(*.png);");
    if (fileName.isEmpty())
        return false;
    }
    else
    {
        QMessageBox::warning(this,"Paint Area not active","Error");
        return false;
    }

    return PaintArea->saveImage(fileName, "png");
}
//Text Edit Methods
void MainWindow::copy()
{
    ui->textEdit->copy();
}

void MainWindow::cut()
{
    ui->textEdit->cut();
}

void MainWindow::paste()
{
    ui->textEdit->paste();
}

void MainWindow::redo()
{
    ui->textEdit->redo();
}


void MainWindow::undo()
{
    ui->textEdit->undo();
}


//Extras
void MainWindow::changeTitle(){
    QString pathEnd = currentFile.section("/",-1); //finds filename from the path provided
    QString title = "Notes n Stuff// " + pathEnd;

    setWindowTitle(title); //Changes title to concatenation
}


// Menus and Actions
void MainWindow::createActions()
{
    //File
    newAct = new QAction(tr("&New..."), this);
    newAct->setShortcuts(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveText);

    saveAsAct = new QAction(tr("&Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveTextAs);

    saveImageAct = new QAction(tr("&Save Image..."), this);
    connect(saveImageAct, &QAction::triggered, this, &MainWindow::saveImage);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    //Edit
    copyAct = new QAction(tr("&Copy..."), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    cutAct = new QAction(tr("&Cut..."), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    pasteAct = new QAction(tr("&Paste..."), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

    redoAct = new QAction(tr("&Redo..."), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    undoAct = new QAction(tr("&Undo..."), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    //Brush
    brushColorAct = new QAction(tr("&Brush Color..."), this);
    connect(brushColorAct, &QAction::triggered, this, &MainWindow::changeBrushColor);

    brushWidthAct = new QAction(tr("&Brush Width..."), this);
    connect(brushWidthAct, &QAction::triggered, this, &MainWindow::changeBrushWidth);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveImageAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(redoAct);
    editMenu->addAction(undoAct);

    brushMenu = menuBar()->addMenu(tr("&Brush"));
    brushMenu->addAction(brushWidthAct);
    brushMenu->addAction(brushColorAct);
}
