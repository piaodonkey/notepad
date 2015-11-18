#include "mainwindow.h"
#include <QtPrintSupport>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //setWindowTitle("未命名[*]");
    initMenu();
    //statusAction->setCheckable(true);
    textEdit=new QTextEdit(this);
    setCentralWidget(textEdit);
    statusBar()->showMessage("Ready");
    fDialog=new QFileDialog(this);
    resize(600,600);
    setCunrrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
    conn();
    textEdit->installEventFilter(this);
    this->setAcceptDrops(false);
    textEdit->setAcceptDrops(false);
    this->setWindowIcon(QIcon(":/1.ico"));


}

void MainWindow::setTex(const QString &temp)
{
   // QMessageBox::about(this,temp,temp);
    tempText=temp;
    if(!tempText.isEmpty())
        openFile();
   // tempText.clear();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==textEdit)
    {

            if(textEdit->textCursor().hasSelection())
            {
                delAction->setEnabled(true);
                //qDebug()<<"selected";
                return true;

            }
            else
            {
                delAction->setDisabled(true);
                return false;
            }


    }
    else
        return QMainWindow::eventFilter(obj,e);
}

void MainWindow::conn()
{
    connect(printAction,&QAction::triggered,this,[=](){
            QPrinter printer;
            QPrintDialog *dialog = new QPrintDialog(&printer, this);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
           // dialog->setWindowTitle(tr("Print Document"));
            if (textEdit->textCursor().hasSelection())
                dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
            if (dialog->exec() == QDialog::Accepted)
                textEdit->print(&printer);
    });
    connect(helpAction,&QAction::triggered,this,[=](){
        QMessageBox::information(this,"帮助","如有疑问请联系QQ:2850125339");
    });
    connect(aboutAction,&QAction::triggered,this,[=](){
        QMessageBox::about(this,"关于",static_cast<QString>(QSysInfo::windowsVersion()));
    });
    connect(statusAction,&QAction::toggled,this,&MainWindow::setSB);//设置状态栏
    connect(autoLAction,&QAction::toggled,this,[=](){
              textEdit->setWordWrapMode(static_cast<QTextOption::WrapMode>(autoLAction->isChecked()));

    });
    connect(exitAction,&QAction::triggered,this,&MainWindow::close);
    connect(newAction,&QAction::triggered,this,&MainWindow::newFile);
    connect(openAction,&QAction::triggered,this,&MainWindow::openFile);

    connect(saveAsAction,&QAction::triggered,this,&MainWindow::saveAs);
    connect(saveAction,&QAction::triggered,this,&MainWindow::save);
    connect(textEdit,&QTextEdit::textChanged,this,[=](){setWindowModified(textEdit->document()->isModified());});
    connect(printpdfAction,&QAction::triggered,this,&MainWindow::print);
    connect(textEdit,SIGNAL(undoAvailable(bool)),undoAction,SLOT(setEnabled(bool)));
    connect(undoAction,&QAction::triggered,textEdit,&QTextEdit::undo);
    connect(textEdit,SIGNAL(copyAvailable(bool)),cutAction,SLOT(setEnabled(bool)));
    connect(textEdit,SIGNAL(copyAvailable(bool)),copyAction,SLOT(setEnabled(bool)));

    connect(cutAction,&QAction::triggered,textEdit,&QTextEdit::cut);
    connect(copyAction,&QAction::triggered,textEdit,&QTextEdit::copy);
    connect(patseAction,&QAction::triggered,textEdit,&QTextEdit::paste);
    connect(delAction,&QAction::triggered,textEdit,[=](){textEdit->textCursor().removeSelectedText();});
    connect(findAction,&QAction::triggered,this,&MainWindow::find);
    connect(findNextAction,&QAction::triggered,this,[=](){
        textEdit->find(findString);
    });
    connect(replaceAction,&QAction::triggered,textEdit,[=](){
        QDialog *inputDia=new QDialog(this);
        inputDia->setWindowModality(Qt::ApplicationModal);
        inputDia->setAttribute(Qt::WA_DeleteOnClose);
        inputDia->setWindowTitle(tr("替换对话框"));
        QLabel *label=new QLabel(tr("查找内容:"),inputDia);
        QLabel *label1=new QLabel(tr("替换内容:"),inputDia);
        QLineEdit *line=new QLineEdit(inputDia);
        QLineEdit *line1=new QLineEdit(inputDia);
        QPushButton *ok=new QPushButton(tr("替换"),inputDia);
        QPushButton *cancel=new QPushButton(tr("取消"),inputDia);
        QHBoxLayout *layout=new QHBoxLayout;
        QHBoxLayout *layout3=new QHBoxLayout;
        layout->addWidget(label);
        layout3->addWidget(label1);
        layout->addWidget(line);
        layout3->addWidget(line1);
        QHBoxLayout *layout1=new QHBoxLayout;
       // layout1->addLayout(layout);
        layout1->addWidget(ok);
        layout1->addWidget(cancel);
        QVBoxLayout *layout2=new QVBoxLayout;
        layout2->addLayout(layout);
        layout2->addLayout(layout3);
        layout2->addLayout(layout1);
        inputDia->setLayout(layout2);
        inputDia->show();
        connect(ok,&QPushButton::clicked,textEdit,[=](){
            if(textEdit->find(line->text()))
            {
                textEdit->insertPlainText(line1->text());
            }
            ;});
        connect(cancel,&QPushButton::clicked,inputDia,&QDialog::close);
    });
    connect(selAllAction,&QAction::triggered,textEdit,[=](){
        textEdit->selectAll();
    });
    connect(dateTAction,&QAction::triggered,textEdit,[=](){
        //textEdit->append(QTime::currentTime().toString());
        textEdit->append(QDateTime::currentDateTime().toString());

    });
    connect(fontAction,&QAction::triggered,this,[=](){
        bool ok;
        QFont font = QFontDialog::getFont(&ok, this);
        if (ok) {
            // font is set to the font the user selected
            textEdit->setFont(font);
        }
    });
}
void MainWindow::find()
{
    //QString findString;
    QDialog *inputDia=new QDialog(this);
    inputDia->setWindowModality(Qt::ApplicationModal);
    inputDia->setAttribute(Qt::WA_DeleteOnClose);
    inputDia->setWindowTitle(tr("查找对话框"));
    QLabel *label=new QLabel(tr("查询内容:"),inputDia);
    QLineEdit *line=new QLineEdit(inputDia);
    QPushButton *ok=new QPushButton(tr("查询下一个"),inputDia);
    QPushButton *cancel=new QPushButton(tr("取消"),inputDia);
    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(label);
    layout->addWidget(line);
    QHBoxLayout *layout1=new QHBoxLayout;
   // layout1->addLayout(layout);
    layout1->addWidget(ok);
    layout1->addWidget(cancel);
    QVBoxLayout *layout2=new QVBoxLayout;
    layout2->addLayout(layout);
    layout2->addLayout(layout1);
    inputDia->setLayout(layout2);
    inputDia->show();
    connect(ok,&QPushButton::clicked,textEdit,[=](){
        findString=line->text();
    textEdit->find(line->text());});
    connect(cancel,&QPushButton::clicked,inputDia,&QDialog::close);
}

void MainWindow::print()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出PDF文件"), QString(), "*.pdf");
        if (!fileName.isEmpty()) {
            // 如果文件后缀为空，则默认使用.pdf
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(".pdf");
            QPrinter printer;
            // 指定输出格式为pdf
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
            textEdit->print(&printer);


        }

}
void MainWindow::setCunrrentFile(const QString &fileName)
{
     curFile=fileName;
     textEdit->document()->setModified(false);
     this->setWindowModified(false);
     QString showName=curFile;
     if(curFile.isEmpty())
     {
         showName="未命名";
     }
     setWindowFilePath(showName);
}
bool MainWindow::saveFile(const QString &fileName)
{

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this,fileName,"写入错误",QMessageBox::Ok);
        return false;
    }
    QTextStream out(&file);
    //textEdit->clear();
#ifndef QT_NO_CURSOR
QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
statusBar()->showMessage("保存文件...");
//while(!in.atEnd())
//{
//textEdit->append( in.readLine());
  // if(fDialog->selectedNameFilter())
    out<<textEdit->toPlainText();
   // out<<textEdit->toHtml();
//}
statusBar()->showMessage("保存完毕",2000);
this->setWindowModified(false);
textEdit->document()->setModified(false);
#ifndef QT_NO_CURSOR
QApplication::restoreOverrideCursor();
#endif

return true;
}
bool MainWindow::save()
{
    if(curFile.isEmpty())
    {
        return saveAs();
    }
    else
    {
        saveFile(curFile);
        return true;
    }

}
bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("文本文档(*.txt)");
    QStringList files;
    if(dialog.exec())
        files=dialog.selectedFiles();
    else
        return false;
    return saveFile(files.at(0));
}
void MainWindow::closeEvent(QCloseEvent *e)
{
   if(maybeSave())
   {
       e->accept();
   }
   else
       e->ignore();
}
void MainWindow::openFile()
{
    if(maybeSave())
    {
        QString text;
        if(tempText.isEmpty())
        {
             text=QFileDialog::getOpenFileName(this,"打开文件",QDir::currentPath(),tr("文本文件 (*.txt);;图像文件 (*.png *.jpg *.bmp *.gif *.jpeg);;所有文件 (*.*)"));
             tempText=text;
        }
        setCunrrentFile(tempText);
       //
        if(!tempText.isEmpty())
        {
            QFile file(tempText);
            if(!file.open(QIODevice::ReadOnly))
            {
                QMessageBox::critical(this,"错误",text.append("打开错误!"),QMessageBox::Ok);
                return;
            }
            QTextStream in(&file);
            textEdit->clear();

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    statusBar()->showMessage("载入文件...");
    while(!in.atEnd())
    {
        textEdit->append( in.readLine());
    }
    statusBar()->showMessage("载入完毕",2000);
    this->setWindowModified(false);
    textEdit->document()->setModified(false);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

        }
    }
     tempText.clear();
}
void MainWindow::newFile()
{
    if(maybeSave())
    {
        textEdit->clear();
        setWindowTitle("未命名[*]");
    }
}
bool MainWindow::maybeSave()
{
    if(textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret=QMessageBox::warning(this,this->windowTitle(),"已修改,是否保存",QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        if(ret==QMessageBox::Save)
        {
            return save();
        }
        else if(ret==QMessageBox::Cancel)
        {
            return false;
        }

    }
    return true;
}
void MainWindow::setSB()
{
    if(statusAction->isChecked())
    {
        statusBar();
        //statusAction->setChecked(true);
        //qDebug()<<statusAction->isCheckable()<<statusAction->isChecked();
    }
    else
    {
        setStatusBar(0);
        //statusAction->setChecked(false);
    }


}
void MainWindow::initMenu()
{

    initAction();

    fileM=menuBar()->addMenu(tr("文件(&F)"));
    fileM->addActions(fileL);

    editM=menuBar()->addMenu(tr("编辑(&E)"));
    editM->addActions(editL);

    styleM=menuBar()->addMenu(tr("格式(O)"));
    styleM->addActions(styleL);

    viewM=menuBar()->addMenu(tr("查看(&V)"));
    viewM->addActions(viewL);


    helpM=menuBar()->addMenu(tr("帮助(&H)"));
    helpM->addActions(helpL);



}
void MainWindow::initAction()
{

    newAction=new QAction(QIcon(":/Action/new"),tr("&新建"),this);
    newAction->setStatusTip(tr("新建文件"));
    newAction->setShortcuts(QKeySequence::New);

    openAction=new QAction(QIcon(":/Action/open"),tr("&打开"),this);
    openAction->setStatusTip(tr("打开文件"));
    openAction->setShortcuts(QKeySequence::Open);

    saveAction=new QAction(QIcon(":/Action/save"),tr("&保存"),this);
    saveAction->setStatusTip(tr("保存文件"));
    saveAction->setShortcuts(QKeySequence::Save);

    saveAsAction=new QAction(QIcon(":/Action/saveAs"),tr("&另存为"),this);
    saveAsAction->setStatusTip(tr("另存文件"));
    saveAsAction->setShortcuts(QKeySequence::SaveAs);

    printAction=new QAction(QIcon(":/Action/print"),tr("&打印"),this);
    printAction->setStatusTip(tr("打印文件"));
    printAction->setShortcuts(QKeySequence::Print);

    printpdfAction=new QAction(QIcon(":/Action/printPdf"),tr("&打印PDF"),this);
    printpdfAction->setStatusTip(tr("打印成PDF文件"));

    exitAction=new QAction(QIcon(":/Action/exit"),tr("&退出"),this);
    exitAction->setStatusTip(tr("退出..."));
    exitAction->setShortcuts(QKeySequence::Quit);

    undoAction=new QAction(QIcon(":/Action/undo"),tr("&撤销"),this);
    undoAction->setStatusTip(tr("撤销"));
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setDisabled(true);
   // undoAction->setSeparator(true);

    cutAction=new QAction(QIcon(":/Action/cut"),tr("&剪切"),this);
    cutAction->setStatusTip(tr("剪切"));
    cutAction->setShortcuts(QKeySequence::Cut);
    cutAction->setDisabled(true);

    copyAction=new QAction(QIcon(":/Action/copy"),tr("&复制"),this);
    copyAction->setStatusTip(tr("复制"));
    copyAction->setShortcuts(QKeySequence::Copy);
    copyAction->setDisabled(true);

    patseAction=new QAction(QIcon(":/Action/patse"),tr("&粘贴"),this);
    patseAction->setStatusTip(tr("粘贴"));
    patseAction->setShortcuts(QKeySequence::Paste);
    //patseAction->setDisabled(true);
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
                    patseAction->setEnabled(md->hasText());

    delAction=new QAction(QIcon(":/Action/del"),tr("&删除"),this);
    delAction->setStatusTip(tr("删除"));
    delAction->setShortcuts(QKeySequence::Delete);
    delAction->setDisabled(true);


    findAction=new QAction(QIcon(":/Action/find"),tr("&查找"),this);
    findAction->setStatusTip(tr("查找"));
    findAction->setShortcuts(QKeySequence::Find);

    findNextAction=new QAction(QIcon(":/Action/findNext"),tr("&查找下一个"),this);
    findNextAction->setStatusTip(tr("查找下一个"));
    findNextAction->setShortcuts(QKeySequence::FindNext);

    replaceAction=new QAction(QIcon(":/Action/replace"),tr("&替换"),this);
    replaceAction->setStatusTip(tr("替换"));
    replaceAction->setShortcuts(QKeySequence::Replace);

    jump2Action=new QAction(QIcon(":/Action/jump2"),tr("&转到"),this);
    jump2Action->setStatusTip(tr("转到"));
    jump2Action->setShortcut(QKeySequence("Ctrl+G"));
    jump2Action->setDisabled(true);

    selAllAction=new QAction(QIcon(":/Action/selAll"),tr("&全选"),this);
    selAllAction->setStatusTip(tr("全选"));
    selAllAction->setShortcuts(QKeySequence::SelectAll);

    dateTAction=new QAction(QIcon(":/Action/dateT"),tr("&时间日期"),this);
    dateTAction->setStatusTip(tr("时间/日期"));
    dateTAction->setShortcut(QKeySequence("Ctrl+T"));

    autoLAction=new QAction(tr("&自动换行"),this);
    autoLAction->setStatusTip(tr("自动换行"));
    autoLAction->setShortcut(QKeySequence("Ctrl+W"));
    autoLAction->setCheckable(true);
    autoLAction->setChecked(true);

    fontAction=new QAction(QIcon(":/Action/font"),tr("&字体"),this);
    fontAction->setStatusTip(tr("字体"));
    fontAction->setShortcut(QKeySequence("Ctrl+F"));

    //statusAction=new QAction(QIcon(":/Action/status"),tr("&状态栏"),this);
    statusAction=new QAction(tr("&状态栏"),this);
    statusAction->setStatusTip(tr("状态栏"));
    statusAction->setShortcut(QKeySequence("S"));
    statusAction->setCheckable(true);
    statusAction->setChecked(true);

    helpAction=new QAction(QIcon(":/Action/help"),tr("&帮助"),this);
    helpAction->setStatusTip(tr("帮助"));
    helpAction->setShortcuts(QKeySequence::HelpContents);

    aboutAction=new QAction(QIcon(":/Action/about"),tr("&关于"),this);
    aboutAction->setStatusTip(tr("关于"));
    aboutAction->setShortcut(QKeySequence("Ctrl+A"));

    //initShortCuts();

    fileL<<newAction<<openAction<<saveAction<<saveAsAction<<menuBar()->addSeparator()<<printAction<<printpdfAction<<menuBar()->addSeparator()<<exitAction;


    editL<<undoAction<<menuBar()->addSeparator()<<cutAction<<copyAction<<patseAction<<delAction<<menuBar()->addSeparator()<<findAction<<findNextAction<<replaceAction<<jump2Action<<menuBar()->addSeparator()<<selAllAction<<dateTAction;
    styleL<<autoLAction<<fontAction;
    viewL<<statusAction;
    helpL<<helpAction<<aboutAction;
    statusL<<statusAction;

}
MainWindow::~MainWindow()
{

}
