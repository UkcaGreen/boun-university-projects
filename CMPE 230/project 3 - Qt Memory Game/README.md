# Memory-Card-Game

## How to Run
```
qmake -project
```
After above command is executed, "**QT += widgets**" string should be added to **qtemil****.pro** file that is newly created.
```
qmake
make
cd release
qtemil.exe
```
Consequently, above commands will be executed to run the program.
***
## Structure
##### Main Window
startGame slots listens whether game is started or not. onEnlapsedStepsChanged and onPairsLabelChanged slots are listens for emissions that are cause change in the tries and pairs labels and updates them respect to these emissions.
* Slots
    * startGame
    * onEnlapsedStepsChanged
    * onPairsLabelChanged
##### Memory Game Board
startGame slots listens whether game is started or not. cardMatched removes matched cards when a match occurs. enableReveal makes a card revealable.
When a change occurs in founded pairs emits a signal to let **Main Window** know about this update. Similarly, after each move enlapsedStepsChanged emits a signal.
* Slots
    * cardMatched
    * startGame
    * enableReveal
* Signals
    * enlapsedStepsChanged
    * pairsLabelChanged
##### Memory Card
flipToFace flips the face-down card on the button click. When I need to flip it back to face-down flipToBack slot is used. flyOut removes the card from the field (used when there is a match). setToFace and setToBack are private slots to set a card face-down or face-up
* Slots
    * setToFace
    * setToBack
    * flyOut
    * flipToFace
    * flipToBack
* Signals
    * matched
***
## Visual Game Guide
#### Start
At the begining there are 24 facedown cards. This screen also greets you when reset button is clicked.

![alt text](https://i.ibb.co/12NB1fR/4.png)

#### Single Card
When a card flipped, it waits another card to proceed.

![alt text](https://i.ibb.co/d49Q6st/1.png)

#### No Match
If the next card is not a match, it flips those two face-down again.

![alt text](https://i.ibb.co/k4zTknT/3.png)

#### Match
In the case of match, match signal is emitted to remove the matched cards.

![alt text](https://i.ibb.co/w7Wyw5Q/2.png)

#### Remove
Removes the matched cards and increases "Pairs" label by one. Game ends when all the cards are removed from the field (in other words "Pairs" label becomes 12).

![alt text](https://i.ibb.co/Hdz5zj4/5.png)

***

