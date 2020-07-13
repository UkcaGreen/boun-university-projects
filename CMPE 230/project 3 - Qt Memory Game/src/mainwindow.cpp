#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "memorygameboard.h"
#include <QShowEvent>

#if defined(HAVE_OPENGL)

#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_elapsedSteps(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    scene->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
    ui->graphicsView->setScene(scene);

}

//destructor for MainWindow object 
MainWindow::~MainWindow()
{
    delete ui;
}


//runs when w.show() is called in main.cpp
void MainWindow::showEvent(QShowEvent *event)
{
    startGame();
    event->accept();
}


// starts game
void MainWindow::startGame()
{


    if (ui->graphicsView->scene())
        ui->graphicsView->scene()->deleteLater();

    MemoryGameBoard *board = new MemoryGameBoard(this);
    ui->graphicsView->setScene(board);
    connect(
        board, 
        &MemoryGameBoard::elapsedStepsChanged, 
        this, 
        &MainWindow::onElapsedStepsChanged);
    connect(
        board, 
        &MemoryGameBoard::pairsLabelChanged, 
        this, 
        &MainWindow::onPairsLabelChanged);
    board->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
    board->setSceneRect(ui->graphicsView->rect());
    board->startGame();
}

//prints number of tries selection
void MainWindow::onElapsedStepsChanged(unsigned n)
{
    m_elapsedSteps = n;
    QString text = tr("Tries: %1").arg(QString::number(n));
    ui->stepsLabel->setText(text);
}


//prints successful pair selection
void MainWindow::onPairsLabelChanged(unsigned n)
{
    m_pairsLabel = n;
    QString text = tr("Pairs: %1").arg(QString::number(n));
    ui->pairsLabel->setText(text);
}
