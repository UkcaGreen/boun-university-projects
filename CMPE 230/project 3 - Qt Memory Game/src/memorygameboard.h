#ifndef MEMORYGAMEBOARD_H
#define MEMORYGAMEBOARD_H

#include <QtWidgets/QGraphicsScene>
#include "memorycard.h"

class MemoryGameBoard : public QGraphicsScene
{
    Q_OBJECT
    int m_rows, m_columns, m_margin, m_elapsedSteps;
    MemoryCard *m_lastRevealed;
    bool m_canReveal;
    QList<MemoryCard*> m_items;

public:
    explicit MemoryGameBoard(QObject *parent = nullptr);
    MemoryCard *lastRevealed() const;
    void setLastRevealed(MemoryCard *card);
    bool canReveal() const;


    static QPixmap paintCard(char c, QColor bg, QColor fg, int width, int height);
    static QList<char> generateChars(int n);

signals:
    void elapsedStepsChanged(int n);
    void pairsLabelChanged(int n);

public slots:
    void cardMatched();
    void startGame();
    void enableReveal();

};

#endif // MEMORYGAMEBOARD_H
