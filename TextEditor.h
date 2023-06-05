#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>

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
    void slotIncreaseImage();
    void slotDecreaseImage();
    void slotInsertTable();
    void slotDarkMode();
    void slotLightMode();
    void closeEvent(QCloseEvent *event);
    bool hasUnsavedChanges();

    //Palette buttons
    void onRedColorButtonClicked();
    void onOrangeColorButtonClicked();
    void onYellowColorButtonClicked();
    void onGreenColorButtonClicked();
    void onAzureColorButtonClicked();
    void onBlueColorButtonClicked();
    void onPurpleColorButtonClicked();
    void onBlackColorButtonClicked();

private:
    Ui::TextEditor *uiPtr;
    QString file_path = "";
    QToolBar *toolbar();
    QMenu *menuConfig();
    QMenu *editMenu();
    QMenu *formatMenu();
    QMenu *insertMenu();
    QMenu *viewMenu();

    //Palette
    QWidget *window = NULL;
    QGroupBox *gridGroupBox;
    QGridLayout *colorPalette;
    QPushButton *redColorButton;
    QPushButton *orangeColorButton;
    QPushButton *yellowColorButton;
    QPushButton *greenColorButton;
    QPushButton *azureColorButton;
    QPushButton *blueColorButton;
    QPushButton *purpleColorButton;
    QPushButton *blackColorButton;

    void setPaletteColors();
    void createColorPalette(qint32 x ,qint32 y , qint32 width=200 , qint32 height=100);
    void hidePalette(QWidget *window);
    void showPalette(QWidget *window);

};
#endif // TEXTEDITOR_H
