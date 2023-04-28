#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QMenu>

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
    void slotFileOpen();
    void slotFileSave();
    void slotFileSaveAs();

private:
    Ui::TextEditor *uiPtr;
    QString file_path;

    QMenu *menuConfig();
};
#endif // TEXTEDITOR_H
