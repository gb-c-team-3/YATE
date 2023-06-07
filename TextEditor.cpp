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
#include <QCloseEvent>
#include <QTextBlock>
#include <QPushButton>


TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent), uiPtr(new Ui::TextEditor)
{
    QFont font("Corbel", 10);
    uiPtr->setupUi(this);
    uiPtr->menubar->setFont(font);
    uiPtr->menubar->setStyleSheet("color:#242424}");
    uiPtr->menubar->addMenu(menuConfig());
    uiPtr->menubar->addMenu(editMenu());
    uiPtr->menubar->addMenu(formatMenu());
    uiPtr->menubar->addMenu(insertMenu());
    uiPtr->menubar->addMenu(viewMenu());
    uiPtr->toolBar->addWidget(toolbar());
    slotRenameTitle("");
    setWindowIcon(QIcon(":/res/Icons/file_1"));

    uiPtr->centralwidget->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);

    readConfig();
    applyConfig();
}

TextEditor::~TextEditor()
{
    createConfig();

    delete uiPtr;
}

/*************************************
 *
 *          Configuration
 *
 *************************************/

void TextEditor::readConfig(){
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/config.txt");

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open config file");
        return;
    }

    QTextStream in(&file);
    configTXT = in.readAll();
    file.close();

}

bool TextEditor::createConfig(){

    // "C:/Users/<USER>/AppData/Local/Team3Editor"
    QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    QFile file((QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/config.txt"));

    if(!file.open(QFile::WriteOnly ))
    {
        QMessageBox::warning(this, "Warning", "Cannot save config file");
    }

    QTextStream out(&file);
    configTXT = QString::number(uiPtr->centralwidget->height()) + "_" + QString::number(uiPtr->centralwidget->width()) + "_";
    out << configTXT;
    file.flush();
    file.close();
    return true;
}


void TextEditor::applyConfig(){
    QString buffer="";

    for (int i = 0, separatorCount =0; i < configTXT.size(); ++i) {

        if((separatorCount == 0) && (configTXT[i] != separator)){
            buffer +=configTXT[i];
        }else if((separatorCount == 0) && (configTXT[i] == separator)){
            separatorCount +=1;
            textEditorHeight = buffer.toInt();

            i++;
            buffer="";
        }

        if((separatorCount == 1) && (configTXT[i] != separator)){
            buffer +=configTXT[i];
        }else if ((separatorCount == 1) && (configTXT[i] == separator)){
            separatorCount +=1;
            textEditorWidth = buffer.toInt();

            i++;
            buffer="";
        }
    }

    this->resize(textEditorWidth,textEditorHeight);
}

/*
 * end configuration
 */

void TextEditor::slotRenameTitle(QString newName)
{
    if (newName == "")
        setWindowTitle(tr("Team.3 Text Editor"));
    else
        setWindowTitle(newName + tr(" - Team.3 Text Editor"));
}

void TextEditor::slotFileNew()
{
    if (hasUnsavedChanges()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Unsaved changes", "You have unsaved changes. Do you want to save them?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            slotFileSaveAs();               // if we made new file and choose to save changes - we save file, clear text area, path, file name in the header
            uiPtr->textEdit->clear();
            file_path.clear();
            QFileInfo fileInfo(file_path);
            QString titleName = fileInfo.fileName();
            slotRenameTitle(titleName);
        }
        else
        {     
            uiPtr->textEdit->clear();    // if we choose not to save changes - we just clear  all (text area, path, name)
            file_path.clear();
            QFileInfo fileInfo(file_path);
            QString titleName = fileInfo.fileName();
            slotRenameTitle(titleName);
        }
    }
    else
    {
        uiPtr->textEdit->clear();        // if we opened existing file but want to make new file without any changes - we just clear all
        file_path.clear();
        QFileInfo fileInfo(file_path);
        QString titleName = fileInfo.fileName();
        slotRenameTitle(titleName);
    }
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
    uiPtr->textEdit->setText(text);      // we show the content of file in the textBrowser
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
        slotFileSaveAs();                   // if file doesnt exist yet we save it by "save as" fucntion, if already exists just save changes
    }
    QTextStream out(&file);
    QString text = uiPtr->textEdit->toHtml(); // to save formating and images we change "toPlainText" into "toHtml"
    out << text;
    file.flush();
    file.close();
}

void TextEditor::slotFileSaveAs()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Save the file", "", "Text Files (*.txt)");  // saves to txt format
    QFile file(file_name);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save the file");
        return;
    }
    file_path = file_name;
    QTextStream out(&file);
    QString text = uiPtr->textEdit->toHtml(); // to save formating and images we change "toPlainText" into "toHtml"
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
    uiPtr->textEdit->print(&printer);
}

void TextEditor::slotExitFile()
{
	QApplication::exit();
}

void TextEditor::slotUndo()
{
	uiPtr->textEdit->undo();
}

void TextEditor::slotRedo()
{
	uiPtr->textEdit->redo();
}

void TextEditor::slotCopy()
{
	uiPtr->textEdit->copy();
}

void TextEditor::slotCut()
{
	uiPtr->textEdit->cut();
}

void TextEditor::slotPaste()
{
	uiPtr->textEdit->paste();
}

void TextEditor::slotSelectAll()
{
	uiPtr->textEdit->selectAll();
}

void TextEditor::slotBold()
{
    if( uiPtr->textEdit->fontWeight() == QFont::Normal) {
        uiPtr->textEdit->setFontWeight(QFont::Bold);
    }
    else{
        uiPtr->textEdit->setFontWeight(QFont::Normal);
    }
}

void TextEditor::slotItalic()
{
    if(uiPtr->textEdit->fontItalic() == true) {
        uiPtr->textEdit->setFontItalic(false);
    }
    else{
        uiPtr->textEdit->setFontItalic(true);
    }
}

void TextEditor::slotUnderlined()
{
    if( uiPtr->textEdit->fontUnderline() == true) {
        uiPtr->textEdit->setFontUnderline(false);
    }
    else{
        uiPtr->textEdit->setFontUnderline(true);
    }
}

void TextEditor::slotCrossedOut()
{
    QFont font_ = uiPtr->textEdit->currentFont();

    if( uiPtr->textEdit->currentFont().strikeOut() == true) {
        font_.setStrikeOut(false);
        uiPtr->textEdit->setCurrentFont(font_);
    }
    else{
        font_.setStrikeOut(true);
        uiPtr->textEdit->setCurrentFont(font_);
    }

}

void TextEditor::slotFontStyle()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
    {
        uiPtr->textEdit->setCurrentFont(font);
    }
    else return;
}

void TextEditor::slotFontColor()
{

    QPoint Pos =mapFromGlobal(QCursor::pos());
    createColorPalette(Pos.x() , Pos.y()-(uiPtr->toolBar->height()));

}

void TextEditor::slotInsertImage()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Open the file");
    if (file_path.isEmpty())
    {
        return;             // if user will want to cancel inserting image
    }

    QTextImageFormat img_fmt;
    img_fmt.setName(file_path);
    img_fmt.setHeight(30);  // made images a bit bigger for easier formating
    img_fmt.setWidth(30);
    uiPtr->textEdit->textCursor().insertImage(img_fmt);
}

void TextEditor::slotIncreaseImage()
{
    QTextBlock currentBlock = uiPtr->textEdit->textCursor().block();
    QTextBlock::iterator it;
    static const double INCREASE_STEP = 10.0;
    for (it = currentBlock.begin(); !(it.atEnd()); ++it)
    {
        QTextFragment fragment = it.fragment();
        if (fragment.isValid())
        {
            if(fragment.charFormat().isImageFormat())
            {
                 QTextImageFormat newImageFormat = fragment.charFormat().toImageFormat();
                 newImageFormat.setWidth(newImageFormat.width()+ INCREASE_STEP);
                 newImageFormat.setHeight(newImageFormat.height()+ INCREASE_STEP);

                 QTextCursor coursorPtr = uiPtr->textEdit->textCursor();
                 coursorPtr.setPosition(uiPtr->textEdit->textCursor().selectionStart());
                 coursorPtr.setPosition(uiPtr->textEdit->textCursor().selectionEnd(), QTextCursor::KeepAnchor);
                 coursorPtr.setCharFormat(newImageFormat);
             }
         }
    }
}

void TextEditor::slotDecreaseImage()
{
    QTextBlock currentBlock = uiPtr->textEdit->textCursor().block();
    QTextBlock::iterator it;
    static const double DECREASE_STEP = 10.0;
    for (it = currentBlock.begin(); !(it.atEnd()); ++it)
    {
        QTextFragment fragment = it.fragment();
        if (fragment.isValid())
        {
            if(fragment.charFormat().isImageFormat())
            {
                 QTextImageFormat newImageFormat = fragment.charFormat().toImageFormat();
                 if(((newImageFormat.width()-DECREASE_STEP) > 0) && ((newImageFormat.height()-DECREASE_STEP) > 0))
                 {
                    newImageFormat.setWidth(newImageFormat.width()-DECREASE_STEP);
                    newImageFormat.setHeight(newImageFormat.height()-DECREASE_STEP);
                 }

                 QTextCursor coursorPtr = uiPtr->textEdit->textCursor();
                 coursorPtr.setPosition(uiPtr->textEdit->textCursor().selectionStart());
                 coursorPtr.setPosition(uiPtr->textEdit->textCursor().selectionEnd(), QTextCursor::KeepAnchor);
                 coursorPtr.setCharFormat(newImageFormat);
             }
         }
    }
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
    QFont font("Corbel", 10);
    QMenu *menuFilePtr = new QMenu(this);
    menuFilePtr->setFont(font);
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
    QFont font("Corbel", 10);
    QMenu *menuEditPtr = new QMenu(this);
    menuEditPtr->setFont(font);
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
    QFont font("Corbel", 10);
    QMenu *menuFormatPtr = new QMenu(this);
    menuFormatPtr->setFont(font);
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
    QFont font("Corbel", 10);
    QMenu *menuInsertPtr = new QMenu(this);
    menuInsertPtr->setFont(font);
    menuInsertPtr->setTitle(tr("Insert"));
    menuInsertPtr->addAction(tr("Image"), this, &TextEditor::slotInsertImage)->setIcon(QIcon(":/res/Icons/insert-picture-icon"));
    menuInsertPtr->addAction(tr("Table"), this, &TextEditor::slotInsertTable)->setIcon(QIcon(":/res/Icons/tablet"));
    return menuInsertPtr;
}

QMenu *TextEditor::viewMenu()
{
    QFont font("Corbel", 10);
    QMenu *menuViewPtr = new QMenu(this);
    menuViewPtr->setFont(font);
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
    bold->setStatusTip("При выделении текста справа налево текст меняется только один раз при нажатии. "
                   "При выделении слева направо, текст меняется каждый раз.");

    QAction *italic = toolbar->addAction(QIcon(":/res/Icons/italic"), "Italic");
    connect(italic, &QAction::triggered, this, &TextEditor::slotItalic);
    italic->setStatusTip("При выделении текста справа налево текст меняется только один раз при нажатии. "
                   "При выделении слева направо, текст меняется каждый раз.");

    QAction *underlined = toolbar->addAction(QIcon(":/res/Icons/underline"), "Underlined");
    connect(underlined, &QAction::triggered, this, &TextEditor::slotUnderlined);
    underlined->setStatusTip("При выделении текста справа налево текст меняется только один раз при нажатии. "
                   "При выделении слева направо, текст меняется каждый раз.");

    QAction *crossedOut = toolbar->addAction(QIcon(":/res/Icons/cross-out"), "Cross");
    connect(crossedOut, &QAction::triggered, this, &TextEditor::slotCrossedOut);
    crossedOut->setStatusTip("При выделении текста справа налево текст меняется только один раз при нажатии. "
                             "При выделении слева направо, текст меняется каждый раз.");

    toolbar->addSeparator();

    QAction *font_style = toolbar->addAction(QIcon(":/res/Icons/font-adjustment"), "Font style");
    connect(font_style, &QAction::triggered, this, &TextEditor::slotFontStyle);

    QAction *font_color = toolbar->addAction(QIcon(":/res/Icons/color-text"), "Font color");
    connect(font_color, &QAction::triggered, this, &TextEditor::slotFontColor);

    toolbar->addSeparator();

    QAction *iamge = toolbar->addAction(QIcon(":/res/Icons/insert-picture-icon"), "Insert image");
    connect(iamge, &QAction::triggered, this, &TextEditor::slotInsertImage);

    QAction *image_up = toolbar->addAction(QIcon(":/res/Icons/picture-increase.png"), "Increase image");
    connect(image_up, &QAction::triggered, this, &TextEditor::slotIncreaseImage);

    QAction *image_down = toolbar->addAction(QIcon(":/res/Icons/picture-decrease.png"), "Reduce image");
    connect(image_down, &QAction::triggered, this, &TextEditor::slotDecreaseImage);

    QAction *tablet = toolbar->addAction(QIcon(":/res/Icons/tablet"), "Insert table");
    connect(tablet, &QAction::triggered, this, &TextEditor::slotInsertTable);

    return toolbar;
}

void TextEditor::closeEvent(QCloseEvent *event)
{
    if (hasUnsavedChanges()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Unsaved changes", "You have unsaved changes. Do you really want to quit?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) 
	{
            event->ignore();
        } 
	else 
	{
            event->accept();
        }
    } 
    else 
    {
        event->accept();
    }
}

bool TextEditor::hasUnsavedChanges() {
    if(uiPtr->textEdit->toPlainText().length() > 0 && file_path.isEmpty()) {
        return true;
    }


    QFile file(file_path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    QString textContent = uiPtr->textEdit->toPlainText();

    return (textContent != fileContent);
}


void TextEditor::setPaletteColors(){

    redColorButton->setStyleSheet("background:red;");
    orangeColorButton->setStyleSheet("background:orange;");
    yellowColorButton->setStyleSheet("background:yellow;");
    greenColorButton->setStyleSheet("background:green;");
    azureColorButton->setStyleSheet("background:azure;");
    blueColorButton->setStyleSheet("background:blue;");
    purpleColorButton->setStyleSheet("background:purple;");
    blackColorButton->setStyleSheet("background:black;");


}

void TextEditor::onRedColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(Qt::red);
    hidePalette(window);
}


void TextEditor::onOrangeColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(QColorConstants::Svg::orange);
    hidePalette(window);
}


void TextEditor::onYellowColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(Qt::yellow);
    hidePalette(window);
}


void TextEditor::onGreenColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(Qt::green);
    hidePalette(window);
}


void TextEditor::onAzureColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(QColorConstants::Svg::azure);
    hidePalette(window);
}


void TextEditor::onBlueColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(Qt::blue);
    hidePalette(window);
}


void TextEditor::onPurpleColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(QColorConstants::Svg::purple);
    hidePalette(window);
}


void TextEditor::onBlackColorButtonClicked()
{
    uiPtr->textEdit->setTextColor(Qt::black);
    hidePalette(window);
}

void TextEditor::createColorPalette(qint32 x ,qint32 y , qint32 height , qint32 width){

    if(window == NULL)  window = new QWidget(uiPtr->centralwidget);
    window->setMaximumSize(height,width);
    window->setGeometry(QRect(x,y,height,width));

    redColorButton = new QPushButton(this);
    QObject::connect(redColorButton,&QPushButton::clicked, this, &TextEditor::onRedColorButtonClicked);

    orangeColorButton= new QPushButton(this);
    QObject::connect(orangeColorButton,&QPushButton::clicked, this, &TextEditor::onOrangeColorButtonClicked);

    yellowColorButton= new QPushButton(this);
    QObject::connect(yellowColorButton,&QPushButton::clicked, this, &TextEditor::onYellowColorButtonClicked);

    greenColorButton= new QPushButton(this);
    QObject::connect(greenColorButton,&QPushButton::clicked, this, &TextEditor::onGreenColorButtonClicked);

    azureColorButton= new QPushButton(this);
    QObject::connect(azureColorButton,&QPushButton::clicked, this, &TextEditor::onAzureColorButtonClicked);

    blueColorButton= new QPushButton(this);
    QObject::connect(blueColorButton,&QPushButton::clicked, this, &TextEditor::onBlueColorButtonClicked);

    purpleColorButton= new QPushButton(this);
    QObject::connect(purpleColorButton,&QPushButton::clicked, this, &TextEditor::onPurpleColorButtonClicked);

    blackColorButton= new QPushButton(this);
    QObject::connect(blackColorButton,&QPushButton::clicked, this, &TextEditor::onBlackColorButtonClicked);


    gridGroupBox = new QGroupBox(tr("Palette"),window);
    colorPalette = new QGridLayout;
    gridGroupBox->setLayout(colorPalette);

    colorPalette->addWidget(redColorButton,0,0,Qt::AlignCenter);
    colorPalette->addWidget(orangeColorButton,0,1,Qt::AlignCenter);
    colorPalette->addWidget(yellowColorButton,0,2,Qt::AlignCenter);
    colorPalette->addWidget(greenColorButton,0,3,Qt::AlignCenter);
    colorPalette->addWidget(azureColorButton,1,0,Qt::AlignCenter);
    colorPalette->addWidget(blueColorButton,1,1,Qt::AlignCenter);
    colorPalette->addWidget(purpleColorButton,1,2,Qt::AlignCenter);
    colorPalette->addWidget(blackColorButton,1,3,Qt::AlignCenter);

    setPaletteColors();

    if(window->isVisible()) hidePalette(window);
    else showPalette(window);


}

void TextEditor::hidePalette(QWidget *window){
    if(window !=NULL && window->isVisible())  window->hide();
}
void TextEditor::showPalette(QWidget *window){
    if(window !=NULL) window->show();
}
