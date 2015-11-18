#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets>
#include <QMainWindow>
#include <QPrinter>
#include <QtPrintSupport>
//#include <QFileDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
      void setTex(const QString &temp);
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:


      bool eventFilter(QObject *, QEvent *);
private:

    QString tempText;
    QAction *printpdfAction,*newAction,*openAction,*saveAction,*saveAsAction,*printAction,*exitAction,
            *undoAction,*cutAction,*copyAction,*patseAction,*delAction,*findAction,*findNextAction,
            *replaceAction,*selAllAction,*dateTAction,*autoLAction,*fontAction,*statusAction,*helpAction,*aboutAction,*jump2Action;
     QFileDialog *fDialog;
    QTextEdit *textEdit;
    void initAction();
    void initMenu();
    void find();
    QString findString;
    QList<QAction*> fileL,editL,styleL,viewL,helpL,statusL;
    QMenu *fileM,*editM,*styleM,*viewM,*helpM;
    void init();
    void conn();
    QString curFile;
    void setCunrrentFile(const QString &);
    void closeEvent(QCloseEvent *e);
    bool saveFile(const QString &);
    void print();
private slots:
    bool saveAs();

    void openFile();
    void newFile();
    bool maybeSave();
    void setSB();
    bool save();

    //void textModifyed();
};

#endif // MAINWINDOW_H
