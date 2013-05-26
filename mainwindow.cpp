#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QSettings("SubToText","SubToText");
    deffolder = settings->value("Default Folder","").toString();
    settings->setValue("Default Folder",deffolder);
    settings->sync();

    ui->mainToolBar->hide();
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,150);
    //ui->tableWidget->setColumnWidth(2,width()-ui->tableWidget->columnWidth(0)-ui->tableWidget->columnWidth(1)-20);
    ui->tableWidget->setColumnWidth(2,width()-275);
    wasSaved = true;
    connect(ui->aboutButton,SIGNAL(clicked()),this,SLOT(about()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionOpenSub,SIGNAL(triggered()),this,SLOT(loadSrtFile()));
    connect(ui->openSrtButton,SIGNAL(clicked()),this,SLOT(loadSrtFile()));
    connect(ui->actionOpenText,SIGNAL(triggered()),this,SLOT(loadTextFile()));
    connect(ui->openTextButton,SIGNAL(clicked()),this,SLOT(loadTextFile()));
    connect(ui->actionSaveText,SIGNAL(triggered()),this,SLOT(saveTextFile()));
    connect(ui->saveTextButton,SIGNAL(clicked()),this,SLOT(saveTextFile()));
    connect(ui->actionSaveAsText,SIGNAL(triggered()),this,SLOT(saveAsTextFile()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(close()));
    ui->tableWidget->setWordWrap(true);
    ui->tableWidget->setTextElideMode(Qt::ElideNone);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent *)
{
   // ui->tableWidget->setColumnWidth(2,width()-ui->tableWidget->columnWidth(0)-ui->tableWidget->columnWidth(1)-20);
    ui->tableWidget->setColumnWidth(2,width()-275);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!wasSaved)
    {
        int r = QMessageBox::warning(this,tr("Text was edited"),
                                     tr("Do you want to save changes?"),
                                     QMessageBox::Yes | QMessageBox::Default,
                                     QMessageBox::No,
                                     QMessageBox::Cancel | QMessageBox::Escape);

        if (r == QMessageBox::No)
        {
            event->accept();
            return;
        }
        if (r == QMessageBox::Cancel | QMessageBox::Escape)
        {
            event->ignore();
            return;
        }
        if (r == QMessageBox::Yes | QMessageBox::Default)
        {
            if (filename.isEmpty())
                saveAsTextFile();
            else
                saveTextFile();
            event->accept();
        }
    }
    else
        event->accept();
}

void MainWindow::loadSrtFile()
{
    QFileDialog dialog;
    QString name = dialog.getOpenFileName(this,tr("Open subtitles file"),
                                                deffolder,"All text files (*.srt)");
    if (!name.isEmpty())
    {
        filename = name;
        dialog.selectFile(name);
        deffolder = dialog.directory().path();
        settings->setValue("Default Folder",deffolder);
        settings->sync();

        setWindowTitle("Sub2Text 0.1 - "+filename);

        numberCount = -1;
        bool started = false;
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            while (!out.atEnd())
            {
                QString str = out.readLine();
                if (str.isEmpty()) started = false;
                else
                {
                    bool newline = false;
                    for (int i=0;i<str.size()-2;i++)
                        if (str[i] == '-' && str[i+1] == '-' && str[i+2] == '>')
                            newline = true;
                    if (newline)
                    {
                        started = true;
                        numberCount++;
                        QString begintime = str.left(8);
                        ui->tableWidget->setRowCount(numberCount+1);
                        ui->tableWidget->setItem(numberCount,0, new QTableWidgetItem(begintime));
                        ui->tableWidget->setItem(numberCount,1, new QTableWidgetItem(""));
                        ui->tableWidget->setItem(numberCount,2, new QTableWidgetItem(""));
                    }
                    else
                    {
                        if (started)
                        {
                            QString text = ui->tableWidget->item(numberCount,2)->text() + str + " ";
                            ui->tableWidget->item(numberCount,2)->setText(text);
                        }
                    }
                }
            }
        }
        filename.clear();
        wasSaved = false;
        ui->tableWidget->setColumnWidth(2,width()-320);
    }
}

void MainWindow::loadTextFile()
{
    QFileDialog dialog;
    QString name = dialog.getOpenFileName(this,tr("Open text file"),
                                                deffolder,"All text files (*.txt)");
    if (!name.isEmpty())
    {
        filename = name;
        dialog.selectFile(name);
        deffolder = dialog.directory().path();
        settings->setValue("Default Folder",deffolder);
        settings->sync();

        setWindowTitle("Sub2Text 0.1 - "+filename);

        numberCount = -1;
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            while (!out.atEnd())
            {
                QString str = out.readLine();
                if (str[0] == ' ')
                {
                    QString text = ui->tableWidget->item(numberCount,2)->text() + str.right(str.size()-(8+2+12+2));
                    ui->tableWidget->item(numberCount,2)->setText(text);
                }
                else
                {
                    numberCount++;
                    QString begintime = str.left(8);
                    str = str.right(str.size()-10);
                    QString person = str.left(12);
                    str = str.right(str.size()-14);
                    ui->tableWidget->setRowCount(numberCount+1);
                    ui->tableWidget->setItem(numberCount,0, new QTableWidgetItem(begintime));
                    ui->tableWidget->setItem(numberCount,1, new QTableWidgetItem(person));
                    ui->tableWidget->setItem(numberCount,2, new QTableWidgetItem(str));
                }
            }
        }
        filename.clear();
        wasSaved = false;
        ui->tableWidget->setColumnWidth(2,width()-320);
    }
}

const int sizeline = 40;
void MainWindow::saveTextFile()
{
    if (!filename.isEmpty())
    {
        numberCount = ui->tableWidget->rowCount();
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            for (int i=0;i<numberCount;i++)
            {
                out << ui->tableWidget->item(i,0)->text() << "  ";
                QString person = ui->tableWidget->item(i,1)->text();
                if (person.size()<=12)
                {
                    out << ui->tableWidget->item(i,1)->text();
                    for (int i=0;i<12-person.size();i++) out << " ";
                    out << "\t";
                }
                else
                {
                    out << ui->tableWidget->item(i,1)->text().left(12) << "\t";
                }
                if (ui->tableWidget->item(i,2)->text().size() <= sizeline)
                    out << ui->tableWidget->item(i,2)->text() << "\n";
                else
                {
                    QString str = ui->tableWidget->item(i,2)->text();
                    while (str.size()>sizeline)
                    {
                        int pos_space = 0;
                        for (int i=0;i<sizeline;i++) if (str[i] == ' ') pos_space = i+1;
                        out << str.left(pos_space) << "\n";
                        for (int i=0;i<8;i++) out << " ";
                        out << "\t";
                        for (int i=0;i<12;i++) out << " ";
                        out << "\t";
                        str = str.right(str.size()-pos_space);
                    }
                    out << str << "\n";
                }
            }
        }
        file.close();
        wasSaved = true;
    }
    else
    {
        saveAsTextFile();
    }
}

void MainWindow::saveAsTextFile()
{
    QFileDialog dialog;
    QString name = dialog.getSaveFileName(this,tr("Save text file"),
                                                deffolder,"All text files (*.txt)");
    if (!name.isEmpty())
    {
        filename = name;
        dialog.selectFile(name);
        deffolder = dialog.directory().path();
        settings->setValue("Default Folder",deffolder);
        settings->sync();

        saveTextFile();
    }
}

void MainWindow::about()
{
    QMessageBox::about(this,tr("About"),
                       tr("Sub2Text 0.1 beta\n"
                          "Application's author is Bodnya Alexey (abodnya)\n"
                          "If you have find a bug or have deal for me, contact with me via email fermerasb@gmail.com"));
}
