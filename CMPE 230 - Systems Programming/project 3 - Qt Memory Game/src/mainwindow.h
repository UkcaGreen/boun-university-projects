#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    unsigned m_elapsedSteps;
    unsigned m_pairsLabel;
    Ui::MainWindow *ui;

protected:
    virtual void showEvent(QShowEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void startGame();
    void onElapsedStepsChanged(unsigned n);
    void onPairsLabelChanged(unsigned n);
};

#endif // MAINWINDOW_H
