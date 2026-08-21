#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCharFormat>
#include <QStackedLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
class QActionGroup;
class QMenu;
class QScrollArea;
QT_END_NAMESPACE
class PaintArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changeTitle();
    QFont myFont;

public slots:
    void makeBold(bool value);
    void makeItalic(bool value);
    void makeUnderline(bool value);
    void changeFontSize(int value);
    void togglePaint(bool value);

private slots:
    void copy();
    void cut();
    void paste();
    void redo();
    void undo();

    void newFile();
    void open();
    void saveText();
    void saveTextAs();
    bool saveImage();

    void changeBrushColor();
    void changeBrushWidth();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";

    void createActions();
    void createMenus();
    void populateMenus();

    PaintArea *PaintArea = nullptr;
    QScrollArea *scrollArea = nullptr;

    QMenu *fileMenu = nullptr;
    QAction *newAct = nullptr;
    QAction *openAct = nullptr;
    QAction *saveAct = nullptr;
    QAction *saveAsAct = nullptr;
    QAction *saveImageAct = nullptr;
    QAction *exitAct = nullptr;

    QMenu *editMenu = nullptr;
    QAction *copyAct = nullptr;
    QAction *cutAct = nullptr;
    QAction *pasteAct = nullptr;
    QAction *redoAct = nullptr;
    QAction *undoAct = nullptr;

    QMenu *brushMenu = nullptr;
    QAction *brushWidthAct = nullptr;
    QAction *brushColorAct = nullptr;
};
#endif // MAINWINDOW_H
