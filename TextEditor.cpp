#include "TextEditor.h"
#include "ui_TextEditor.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <QFontDialog>
#include <QColorDialog>
#include <QColor>
#include <QImage>
#include <QIcon>
#include <QToolBar>
#include <QAction>


TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent), uiPtr(new Ui::TextEditor)
{
    uiPtr->setupUi(this);
    uiPtr->menubar->addMenu(menuConfig());
    uiPtr->menubar->setFont(tr("Corbel"));
    uiPtr->menubar->addMenu(editMenu());
    uiPtr->menubar->addMenu(formatMenu());
    uiPtr->menubar->addMenu(insertMenu());
    uiPtr->menubar->addMenu(viewMenu());
    uiPtr->toolBar->addWidget(toolbar());
    slotRenameTitle("");
    setWindowIcon(QIcon(":/res/Icons/file"));
}

TextEditor::~TextEditor()
{
    delete uiPtr;
}


void TextEditor::slotRenameTitle(QString newName)
{
    if (newName == "")
        setWindowTitle(tr("Team.3 Text Editor"));
    else
        setWindowTitle(newName + tr(" - Team.3 Text Editor"));
}

void TextEditor::slotFileNew()
{

}

void TextEditor::slotFileOpen()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(file_name);
    file_path = file_name;
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open the file");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    QFileInfo fileInfo(file_path);
    QString titleName = fileInfo.fileName();
    slotRenameTitle(titleName);
    file.close();
}

void TextEditor::slotFileSave()
{
    QFile file(file_path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save the file");
        return;
    }
    QTextStream out(&file);
    QString text = uiPtr->textBrowser->toPlainText();
    out << text;
    file.flush();
    file.close();
}

void TextEditor::slotFileSaveAs()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Save the file");
    QFile file(file_name);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save the file");
        return;
    }
    file_path = file_name;
    QTextStream out(&file);
    QString text = uiPtr->textBrowser->toPlainText();
    out << text;
    file.flush();
    file.close();
}

void TextEditor::slotPrintFile()
{
    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle("Print");
    if (dlg.exec() != QDialog::Accepted)
    {
        return;
    }
    uiPtr->textBrowser->print(&printer);
}

void TextEditor::slotExitFile()
{

}

void TextEditor::slotUndo()
{

}

void TextEditor::slotRedo()
{

}

void TextEditor::slotCopy()
{

}

void TextEditor::slotCut()
{

}

void TextEditor::slotPaste()
{

}

void TextEditor::slotSelectAll()
{

}

void TextEditor::slotBold()
{

}

void TextEditor::slotItalic()
{

}

void TextEditor::slotUnderlined()
{

}

void TextEditor::slotCrossedOut()
{

}

void TextEditor::slotFontStyle()
{

}

void TextEditor::slotFontColor()
{

}

void TextEditor::slotInsertImage()
{

}

void TextEditor::slotInsertTable()
{

}

void TextEditor::slotDarkMode()
{

}

void TextEditor::slotLightMode()
{

}

QMenu *TextEditor::menuConfig()
{
    QMenu *menuFilePtr = new QMenu(this);
    menuFilePtr->setFont(tr("Corbel"));
    menuFilePtr->setTitle(tr("File"));
    menuFilePtr->addAction(tr("New"), QKeySequence::New, this, &TextEditor::slotFileNew)->setIcon(QIcon(":/res/Icons/file")); //Ctrl+N
    menuFilePtr->addAction(tr("Open"), QKeySequence::Open, this, &TextEditor::slotFileOpen)->setIcon(QIcon(":/res/Icons/folder")); //Ctrl+O
    menuFilePtr->addAction(tr("Save"), QKeySequence::Save, this, &TextEditor::slotFileSave)->setIcon(QIcon(":/res/Icons/diskette"));   //Ctrl+S
    menuFilePtr->addAction(tr("Save as"), QKeySequence::SaveAs, this, &TextEditor::slotFileSaveAs)->setIcon(QIcon(":/res/Icons/save-as"));//Ctrl+Shift+S
    menuFilePtr->addAction(tr("Print"), QKeySequence::Print, this, &TextEditor::slotPrintFile)->setIcon(QIcon(":/res/Icons/printer")); //Ctrl+P
    menuFilePtr->addSeparator();
    menuFilePtr->addAction(tr("Exit"), this, &TextEditor::slotExitFile)->setIcon(QIcon(":/res/Icons/logout"));
    return menuFilePtr;
}

QMenu *TextEditor::editMenu()
{
    QMenu *menuEditPtr = new QMenu(this);
    menuEditPtr->setFont(tr("Corbel"));
    menuEditPtr->setTitle(tr("Edit"));
    menuEditPtr->addAction(tr("Undo"), QKeySequence::Undo, this, &TextEditor::slotUndo)->setIcon(QIcon(":/res/Icons/turn-left")); //Ctrl+Z
    menuEditPtr->addAction(tr("Redo"), QKeySequence::Redo, this, &TextEditor::slotRedo)->setIcon(QIcon(":/res/Icons/forward")); //Ctrl+Y
    menuEditPtr->addAction(tr("Copy"), QKeySequence::Copy, this, &TextEditor::slotCopy)->setIcon(QIcon(":/res/Icons/copy")); //Ctrl+C
    menuEditPtr->addAction(tr("Cut"),QKeySequence::Cut, this, &TextEditor::slotCut)->setIcon(QIcon(":/res/Icons/scissors")); //Ctrl+X
    menuEditPtr->addAction(tr("Paste"), QKeySequence::Paste, this, &TextEditor::slotPaste)->setIcon(QIcon(":/res/Icons/paste")); //Ctrl+V
    menuEditPtr->addSeparator();
    menuEditPtr->addAction(tr("Select All"), QKeySequence::SelectAll, this, &TextEditor::slotSelectAll)->setIcon(QIcon(":/res/Icons/checkbox")); //Ctrl+A
    return menuEditPtr;
}

QMenu *TextEditor::formatMenu()
{
    QMenu *menuFormatPtr = new QMenu(this);
    menuFormatPtr->setFont(tr("Corbel"));
    menuFormatPtr->setTitle(tr("Format"));
    menuFormatPtr->addAction(tr("Bold"), QKeySequence::Bold, this, &TextEditor::slotBold)->setIcon(QIcon(":/res/Icons/bold")); //Ctrl+B
    menuFormatPtr->addAction(tr("Italic"), QKeySequence::Italic, this, &TextEditor::slotItalic)->setIcon(QIcon(":/res/Icons/italic")); //Ctrl+I
    menuFormatPtr->addAction(tr("Underline"), QKeySequence::Underline, this, &TextEditor::slotUnderlined)->setIcon(QIcon(":/res/Icons/underline")); //Ctrl+U
    menuFormatPtr->addAction(tr("Crossed"),this, &TextEditor::slotCrossedOut)->setIcon(QIcon(":/res/Icons/cross-out"));
    menuFormatPtr->addSeparator();
    menuFormatPtr->addAction(tr("Font style"), this, &TextEditor::slotFontStyle)->setIcon(QIcon(":/res/Icons/font-adjustment"));
    menuFormatPtr->addAction(tr("Font color"), this, &TextEditor::slotFontColor)->setIcon(QIcon(":/res/Icons/color-text"));
    return menuFormatPtr;
}

QMenu *TextEditor::insertMenu()
{
    QMenu *menuInsertPtr = new QMenu(this);
    menuInsertPtr->setFont(tr("Corbel"));
    menuInsertPtr->setTitle(tr("Insert"));
    menuInsertPtr->addAction(tr("Image"), this, &TextEditor::slotInsertImage)->setIcon(QIcon(":/res/Icons/insert-picture-icon"));
    menuInsertPtr->addAction(tr("Table"), this, &TextEditor::slotInsertTable)->setIcon(QIcon(":/res/Icons/tablet"));
    return menuInsertPtr;
}

QMenu *TextEditor::viewMenu()
{
    QMenu *menuViewPtr = new QMenu(this);
    menuViewPtr->setFont(tr("Corbel"));
    menuViewPtr->setTitle(tr("View"));
    menuViewPtr->addAction(tr("Dark mode"), this, &TextEditor::slotDarkMode)->setIcon(QIcon(":/res/Icons/file"));
    menuViewPtr->addAction(tr("Light mode"), this, &TextEditor::slotLightMode)->setIcon(QIcon(":/res/Icons/file_white"));
    return menuViewPtr;
}

QToolBar *TextEditor::toolbar()
{
    QToolBar *toolbar = new QToolBar(this);

    QAction *newdoc = toolbar->addAction(QIcon(":/res/Icons/file"), "New document");
    connect(newdoc, &QAction::triggered, this, &TextEditor::slotFileNew);

    QAction *open = toolbar->addAction(QIcon(":/res/Icons/folder"), "Open file");
    connect(open, &QAction::triggered, this, &TextEditor::slotFileOpen);

    QAction *save = toolbar->addAction(QIcon(":/res/Icons/diskette"), "Save file");
    connect(save, &QAction::triggered, this, &TextEditor::slotFileSave);

    QAction *save_as = toolbar->addAction(QIcon(":/res/Icons/save-as"), "Save file as");
    connect(save_as, &QAction::triggered, this, &TextEditor::slotFileSaveAs);

    QAction *printdoc = toolbar->addAction(QIcon(":/res/Icons/printer"), "Print document");
    connect(printdoc, &QAction::triggered, this, &TextEditor::slotPrintFile);

    toolbar->addSeparator();

    QAction *undo = toolbar->addAction(QIcon(":/res/Icons/turn-left"), "Undo");
    connect(undo, &QAction::triggered, this, &TextEditor::slotUndo);

    QAction *redo = toolbar->addAction(QIcon(":/res/Icons/forward"), "Redo");
    connect(redo, &QAction::triggered, this, &TextEditor::slotRedo);

    QAction *copy = toolbar->addAction(QIcon(":/res/Icons/copy"), "Copy");
    connect(copy, &QAction::triggered, this, &TextEditor::slotCopy);

    QAction *cut = toolbar->addAction(QIcon(":/res/Icons/scissors"), "Cut");
    connect(cut, &QAction::triggered, this, &TextEditor::slotCut);

    QAction *paste = toolbar->addAction(QIcon(":/res/Icons/paste"), "Paste");
    connect(paste, &QAction::triggered, this, &TextEditor::slotPaste);

    toolbar->addSeparator();

    QAction *bold = toolbar->addAction(QIcon(":/res/Icons/bold"), "Bold");
    connect(bold, &QAction::triggered, this, &TextEditor::slotBold);

    QAction *italic = toolbar->addAction(QIcon(":/res/Icons/italic"), "Italic");
    connect(italic, &QAction::triggered, this, &TextEditor::slotItalic);

    QAction *underlined = toolbar->addAction(QIcon(":/res/Icons/underline"), "Underlined");
    connect(underlined, &QAction::triggered, this, &TextEditor::slotUnderlined);

    toolbar->addSeparator();

    QAction *font_style = toolbar->addAction(QIcon(":/res/Icons/font-adjustment"), "Font style");
    connect(font_style, &QAction::triggered, this, &TextEditor::slotFontStyle);

    QAction *font_color = toolbar->addAction(QIcon(":/res/Icons/color-text"), "Font color");
    connect(font_color, &QAction::triggered, this, &TextEditor::slotFontColor);

    toolbar->addSeparator();

    QAction *iamge = toolbar->addAction(QIcon(":/res/Icons/insert-picture-icon"), "Insert image");
    connect(iamge, &QAction::triggered, this, &TextEditor::slotInsertImage);

    QAction *tablet = toolbar->addAction(QIcon(":/res/Icons/tablet"), "Insert table");
    connect(tablet, &QAction::triggered, this, &TextEditor::slotInsertTable);

    return toolbar;
}

