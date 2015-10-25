# MaM9.5_2015
Arduino remote control

Mind a két program Arduino NANO-hoz készült. 
A különbség a verseny alatt használt verzióban, hogy a szervó végállásokat átállítottuk, és a késleltetéseket a tesztelések alatt. Kommunikációs részén nem történt változtatás.

Egyik az adó, másik a vevő.

Mind a kettő a VirtualWire-t használja. További infó róla: http://www.airspayce.com/mikem/arduino/VirtualWire/index.html
Leírás bekötéshez, teszt program: http://www.airspayce.com/mikem/arduino/VirtualWire.pdf
Érdemes lehet átírni erre: http://www.airspayce.com/mikem/arduino/RadioHead/ Elvileg ez többet tud.

A szervo motor mozgatáshoz ezt használtuk, mert a Timer1-et lefoglalta a VirtualWire: http://playground.arduino.cc/ComponentLib/Servo
A SoftwareServo.h file-ban ki kell majd cserélni a #include <WProgram.h> következőre: #include <Arduino.h> ha nem fordul le egyből.
