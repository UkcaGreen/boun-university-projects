#include "memorycard.h"
#include "memorygameboard.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QTimer>
#include <QtWidgets/QGraphicsSceneMouseEvent>

MemoryCard::MemoryCard(MemoryGameBoard *scene) :
    QObject(nullptr),
    QGraphicsPixmapItem(nullptr),
    m_isFace(false)
{
    scene->addItem(this);
}

MemoryCard::MemoryCard(const QPixmap &face, const QPixmap &back, MemoryGameBoard *scene, int id) :
    QObject(nullptr),
    QGraphicsPixmapItem(back, nullptr),
    m_face(face),
    m_back(back),
    m_isFace(false),
    m_id(id)
{
    scene->addItem(this);
}


void MemoryCard::flip(const char *slotName)
{   
    
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "pos", this);
    posAnimation->setStartValue(pos());

    posAnimation->setEndValue(pos());


    group->addAnimation(posAnimation);

    QTimer::singleShot(100, this, slotName);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MemoryCard::flipToBack()
{
    flip(SLOT(setToBack()));
}

void MemoryCard::flipToFace()
{
    flip(SLOT(setToFace()));
}

void MemoryCard::setToBack()
{
    m_isFace = false;
    setPixmap(m_back);
}

void MemoryCard::setToFace()
{
    m_isFace = true;
    setPixmap(m_face);
}

//returns MemoryCard open or not 
bool MemoryCard::isFace() const
{
    return m_isFace;
}

//specifies action for mouse press to select a card 
void MemoryCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MemoryGameBoard *board = static_cast<MemoryGameBoard*>(scene());

    if (!m_isFace && board->canReveal())
    {
        flipToFace();

        if (board->lastRevealed())
        {
            MemoryCard *other = board->lastRevealed();
            board->setLastRevealed(nullptr);

            if (this->m_id == other->m_id)
            {
                QTimer::singleShot(500, this, SLOT(flyOut()));
                QTimer::singleShot(500, other, SLOT(flyOut()));
                QTimer::singleShot(900, board, SLOT(enableReveal()));
            }
            else
            {
                QTimer::singleShot(1000, this, SLOT(flipToBack()));
                QTimer::singleShot(1000, other, SLOT(flipToBack()));
                QTimer::singleShot(1000, board, SLOT(enableReveal()));
            }
        }
        else
        {
            board->setLastRevealed(this);
        }
    }

    event->accept();
}


//removes MemoryCard from the game
void MemoryCard::flyOut()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos", this);
    animation->setEndValue(QPoint(- 10000 , - 10000 ));
    connect(animation, &QAbstractAnimation::finished, this, &MemoryCard::matched);
    connect(animation, &QAbstractAnimation::finished, this, &QObject::deleteLater);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


