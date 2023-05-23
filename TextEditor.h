#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();

private slots:
    void slotRenameTitle(QString newName);
    void slotFileNew();
    void slotFileOpen();
    void slotFileSave();
    void slotFileSaveAs();
    void slotPrintFile();
    void slotExitFile();
    void slotUndo();
    void slotRedo();
    void slotCopy();
    void slotCut();
    void slotPaste();
    void slotSelectAll();
    void slotBold();
    void slotItalic();
    void slotUnderlined();
    void slotCrossedOut();
    void slotFontStyle();
    void slotFontColor();
    void slotInsertImage();
    void slotInsertTable();
    void slotDarkMode();
    void slotLightMode();

private:
    Ui::TextEditor *uiPtr;
    QString file_path = "";
    QToolBar *toolbar();
    QMenu *menuConfig();
    QMenu *editMenu();
    QMenu *formatMenu();
    QMenu *insertMenu();
    QMenu *viewMenu();
};
#endif // TEXTEDITOR_H
