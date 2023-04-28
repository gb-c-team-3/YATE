#include "TextEditor.h"
#include "ui_TextEditor.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>


TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent), uiPtr(new Ui::TextEditor)
{
    uiPtr->setupUi(this);
    uiPtr->menubar->addMenu(menuConfig());
    setWindowTitle(tr("Team-3 Text Editor"));
}

TextEditor::~TextEditor()
{
    delete uiPtr;
}

void TextEditor::slotFileOpen()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(file_name);
    file_name = file_name;
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open the file");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    uiPtr->textBrowser->setText(text);
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

QMenu *TextEditor::menuConfig()
{
    QMenu *menuFilePtr = new QMenu(this);
    menuFilePtr->setTitle(tr("Files"));
    menuFilePtr->addAction(tr("Open file"), this, &TextEditor::slotFileOpen, QKeySequence::New);      //Ctrl+N
    menuFilePtr->addAction(tr("Save file"), this, &TextEditor::slotFileSave, QKeySequence::SaveAs);   //Ctrl+S
    menuFilePtr->addAction(tr("Save file as"), this, &TextEditor::slotFileSaveAs, QKeySequence::Save);//Ctrl+Shift+S
    return menuFilePtr;
}

//void TextEditor::on_actionOpen_triggered()
//{

//}


//void TextEditor::on_actionSave_triggered()
//{

//}


//void MainWindow::on_actionSave_as_triggered()
//{

//}
