#include "memorygameboard.h"
#include <QtGui/QPainter>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QTime>


MemoryGameBoard::MemoryGameBoard(QObject *parent) :
    QGraphicsScene(parent),
    m_rows(4),
    m_columns(6),
    m_margin(1),
    m_elapsedSteps(0),
    m_lastRevealed(nullptr),
    m_canReveal(true)
{
}


MemoryCard *MemoryGameBoard::lastRevealed() const
{
    return m_lastRevealed;
}


void MemoryGameBoard::setLastRevealed(MemoryCard *card)
{
    if (m_canReveal)
    {
        if (!card)
        {
            m_canReveal = false;
            emit elapsedStepsChanged(++m_elapsedSteps);
        }
        m_lastRevealed = card;
    }
}

bool MemoryGameBoard::canReveal() const
{
    return m_canReveal;
}

void MemoryGameBoard::enableReveal()
{
    m_canReveal = true;
}


//Formats size and shape of the cards
QPixmap MemoryGameBoard::paintCard(char c, QColor bg, QColor fg, int width, int height)
{

    //Assigns font size
    QFont font;
    font.setPixelSize(height - 15);

    QPixmap pix(width, height);
    pix.fill(Qt::transparent);
    
    //Paints rectangular cards with given dimensions and colors
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setFont(font);
    painter.setPen(bg);
    painter.setBrush(bg);
    painter.drawRoundedRect(0, 0, width, height, 10, 10, Qt::RelativeSize);
    painter.setPen(fg);
    painter.drawText(0, 0, width, height, Qt::AlignCenter, QString(c));
    painter.end();
    return pix;
}


/*
Calculates MemoryCards' sizes
Formats MemoryCards
Generates MemoryCards 
Locates MemoryCards
*/
void MemoryGameBoard::startGame()
{
    //size calculation
    qreal ww = static_cast<qreal>(sceneRect().width()) / static_cast<qreal>(m_columns);
    qreal hh = static_cast<qreal>(sceneRect().height()) / static_cast<qreal>(m_rows);
    int h = static_cast<int>(hh);
    int w = static_cast<int>(ww);

    //formats MemoryCards' Colors and Shapes
    QPixmap backPixmap = paintCard(
        'X', 
        QColor(50, 50, 50, 255), 
        QColor(255, 255, 255, 255), 
        w - m_margin, 
        h - m_margin);


    //Generates MemoryCards
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    QList<char> chars = generateChars(m_rows * m_columns);

    //randomize letters' location 
    qsrand(static_cast<quint32>(QTime::currentTime().msec()));

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            

            //Select letter from chars randomly
            char ch = chars[qrand() % chars.count()];
            chars.removeOne(ch);

            //Creates MemoryCard object 
            MemoryCard *card = new MemoryCard(paintCard(ch, QColor(230, 230, 230, 255), QColor(20, 20, 20, 255), w - m_margin, h - m_margin),
                                              backPixmap,
                                              this,
                                              ch);
       

            //Connects card object to MemoryGameBoard
            m_items.append(card);
            connect(card, &MemoryCard::matched, this, &MemoryGameBoard::cardMatched);
            card->show();
            
            //Locates MemoryCard object at MemoryGameBoard
            QPropertyAnimation *animation = new QPropertyAnimation(card, "pos", this);
            animation->setStartValue(QPointF(j * w + m_margin, -h));
            animation->setEndValue(QPointF(j * w + m_margin, i * h + m_margin));
            group->addAnimation(animation);
        }
    }

    //Notifies other objects in program 
    emit elapsedStepsChanged(0);
    emit pairsLabelChanged(0);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

//Removes MemoryCard when selected pair is true
void MemoryGameBoard::cardMatched()
{
    auto *card = qobject_cast<MemoryCard*>(sender());
    m_items.removeOne(card);
    int restCard = m_items.length();
    emit pairsLabelChanged(((m_columns * m_rows)-restCard)/2);
}


// generates cards' letters 
QList<char> MemoryGameBoard::generateChars(int n)
{
    QList<char> chars;
    if (n % 2)
        n--;
    int i = 0;
    
    for (char c = 'A'; i < n && c <= 'Z'; c+=1, i+=2)
    {
        chars.push_back(c);
        chars.push_back(c);
    }
    return chars;
}


