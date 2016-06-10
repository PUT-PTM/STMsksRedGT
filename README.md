# sksRedGT

sksRedGT is a smartphone controlled toy car mostly powered by batteries. 
Car sksRedGT is also faithful copy of the original Alfa Romeo 6969, which didn't actually exist
(I just make up to fill description cuz one paragraph would look poor; in english every word feels like a shooting star. Make a wish!)

## Components

A lot of people keep asking us why don't we put a lists of components of car. We don't know. 
Here and now, the list of components

- STM32F4 Discovery Board
- HC-05 bluetooth
- Ultrasonic ranging module HC-SR04
- Lego Technic Engine
- Servo FUTABA S3003
- Qoltec PowerBank
- Commutator
- Batteries
- Car housing made from lego


#### HC-SR04 outputs
                    
- Vcc: 5V
- Gnd: GND
- Trig: PC1
- Echo: PD0

#### Bluetooth outputs

- Vcc: 3V
- Gnd: GND
- TXD: PC11
- RXD: PC10

#### Lego Technic Engine

- alternately to AO & AO2 of commutator

#### Servo FUTABA S3003

- Vcc: 5V
- Gnd: GND
- Signal: PC11

#### Commutator

- Gnd: GND
- PWMA: PB6
- AIN2: PA9
- AIN1: PA7
- STBY: 5V


## How to use

The code is arleady downloaded to flash of stm32f4 board. You can review or download the whole project using Github.
To drive sksRedGT you have to download App from Github and install on your smartphone.


## Application

It's simple Android App which allows you take control over sksRedGT by bluetooth.
Catalog STMsksRedGT/Application contains all the necessary files.

## Future plans

We're thinking about having T-shirts with logo of sksRedGT. Stay tuned.

# We are

## S  Szymon Kołodziejczak
## K  Kuba Krawczyk
## S  Szymon Kaszuba

Special credits to our friend Maciek


The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.

Supervisor: Tomasz Mańkowski








