##  Smart Pet Feeder

This project implements a **smart pet feeder** designed to automatically feed pets based on their presence and the current food level in the bowl.

The system uses an **ultrasonic sensor** to detect when the pet approaches the feeder. Once the pet is detected, a **weight sensor (load cell)** measures the amount of food currently in the bowl. If the measured weight is below a predefined threshold, the feeder automatically opens its door to dispense food until the desired amount is reached.

To ensure proper operation at night, a **photoresistor (LDR)** monitors ambient light levels. When low-light conditions are detected and the pet approaches, an **LED light** is turned on to provide visibility for the pet.

This project ensures that pets are fed **consistently and safely**, regardless of the time of day, while minimizing food waste and manual intervention.

### Features

* Automatic pet detection using an ultrasonic sensor
* Food level monitoring with a weight sensor
* Automatic food dispensing when the bowl is not sufficiently filled
* Nighttime detection using a photoresistor
* LED illumination in low-light conditions
