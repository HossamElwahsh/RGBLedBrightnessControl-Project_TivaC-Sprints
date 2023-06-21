# LED Control V2.0
#### ARM
#### Author: Team 1 - Sub team A
- #### Members:
    - Alaa Hisham
    - Hossam Elwahsh

## Brief
> Develop the Timer Driver and use it to control the RGB LED brightness on the Tiva-C board based on the push button press.

## Quick Links
- `Video` 👉 [Youtube](https://youtu.be/7yPKnvcMhTQ)
- `Project Hex` 👉 [LED_Control.hex](RGB-BRIGHTNESS/Objects/LED_Control.hex)
- `Documentation` 👉 [Google Docs](https://docs.google.com/document/d/11W93RY6OmI8ad2eAT02Lx71EFzYXv_iCN_bzJ6KNIy0/edit?usp=sharing) 👉 [PDF](Documents/RGB%20LED%20Control%20V2.0%20Design.pdf)
- `Team Backlog / Test Protocol` 👉 [Google Sheets](https://docs.google.com/spreadsheets/d/1NoLEc5Sk5aW1h5KqDCW50kSdbQppFpclcKfD8z8wE_4/edit?usp=sharing) 👉 [Excel](Documents/RGB%20Control%20V2.0%20-%20Team%201%20-%20Sub%20Team%20A.xlsx) 

----
## Requirements
### Hardware Requirements
- Use the TivaC board
- Use SW1 as an input button
- Use the RGB LED
### Software Requirements
- **The RGB LED is OFF initially**
- The PWM signal has a 500ms duration
- The system has four states:
- SW1 - First press
  - The Green LED will be on with a 30% duty cycle
- SW1 - Second press
  - The Green LED will be on with a 60% duty cycle
- SW1 -Third press
  - The Green LED will be on with a 90% duty cycle
- SW1 - Fourth press will be off
  - The Green LED will be off
- On the fifth press, system state will return to state 1

### Implement your drivers
- Implement the General Purpose Timer (GPT) driver
  - GPT Driver shall support APIs like:( Initialization, starting a timer in microseconds,  milliseconds, and seconds, stopping the timer, getting elapsed time, getting remaining time, enabling interrupts, disable interrupts, and a callback.)