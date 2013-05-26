#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    bool wasSaved;
    QString filename;
    QString deffolder;
    int numberCount;
    QSettings *settings;

private slots:
    void loadSrtFile();
    void loadTextFile();
    void saveTextFile();
    void saveAsTextFile();
    void about();

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
