
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include<QScreen>
#include<QtCore>
#include<QtGui>
#include<QString>
#include "windows.h"
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setAttribute( Qt::WA_TranslucentBackground, true );
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::BypassWindowManagerHint| Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

    ui->setupUi(this);

    // Register hotkey
    BOOL success = RegisterHotKey(reinterpret_cast<HWND>(winId()), 1, MOD_CONTROL | MOD_ALT, 'Q');

    if (!success) {
        // Handle error
        ui->search->setText("Fuck");
    }


    int screenwidth;
    int screenheight;

    //Screen Size
    QScreen* screen = QGuiApplication::primaryScreen();
    screenwidth = screen->geometry().width();
    screenheight = screen->geometry().height();

    int searchwidth;
    int searchheight;
    int adjustWidth;

    //search

    adjustWidth = screenwidth / 10;
    searchwidth = (screenwidth / 2) - adjustWidth;
    searchheight = 50;

    this->setGeometry((screenwidth/2) - (searchwidth/2),screenheight /10,searchwidth,searchheight);
    ui->search->setGeometry(0, 0, searchwidth, searchheight);

    connect(ui->search, &QLineEdit::returnPressed, this, &MainWindow::onLineEditReturnPressed);

}

MainWindow::~MainWindow()
{
    UnregisterHotKey(reinterpret_cast<HWND>(winId()), 1);

    delete ui;
}



void MainWindow::onLineEditReturnPressed()
{
    QString prevString;
    prevString = ui->search->text();
    QString url = QString("https://www.google.com/search?q=");
    url.append(prevString);

    QDesktopServices::openUrl(url);

    ui->search->setText("");

}

bool MainWindow::winEvent(MSG *message, long *result)
{
    if (message->message == WM_HOTKEY) {
        int id = static_cast<int>(message->wParam);
        if (id == 1) {
            ui->search->setText("Ctrl+Alt+Q pressed");
            // Show the window if it is minimized
            if (isMinimized()) {
                showNormal();
            }
            // Activate the window if it is not in focus
            if (!isActiveWindow()) {
                activateWindow();
            }
            return true;
        }
    }
    return MainWindow::winEvent(message, result);
}
