# microprocessor_project
a simple space invader game coded in C for Arduino Uno and displayed on multiple RGB matrices. the project is simulated using proteus8.
the C code and simulation files are available in this repository.

## code
the fastLED library is used to work with RGB matrix.

## simulation
<img width="1093" height="779" alt="Screenshot 2025-08-10 143025" src="https://github.com/user-attachments/assets/fe085b46-8f7c-499f-b1ee-483ca8470959" />

### game play
the matrices display a pink laser canon controlled by the player which shoots white laser balls when the fire button is pressed. there are several blue alien ships that shoot yellow bullets. if the laser cannon hit any of the aliens or their bullets the game would be over, the whole screen turns red and a new scene would be displayed(the game restarts). if the player's laser ball hit any alien, the alien would be removed from the screen. after defeating all the aliens, the whole screen would turn green(player wins the game) and a new scene would be displayed.
### main controls
the laser cannon is controled by 4 buttons to move in the four main directions and 1 button to fire. the buttons are connected to the Arduino Uno and when pressed the interrupts are handled correspondingly.


