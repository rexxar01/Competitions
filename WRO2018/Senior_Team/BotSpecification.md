| Wheel Number | Position | DIR | PWM | Reversed |
| :-----: | :----: | :-----: | :-------: | :----: |
| 1 | Top Right    | 53 | 10 | False |
| 2 | Top Left     | 49 | 7 | False |
| 3 | Bottom Left  | 47 | 8 | False |
| 4 | Bottom Right | 51 | 9 | True  |
# Base

## Base motors
**Motor Drivers**: Cytron MDD10A REV2.0<br>
**Motor Driver convention logic**: A is LOW, B is HIGH
### MOTOR DRIVER 1
#### TOP RIGHT
A : -ve convention<br>
B : +ve convection<br>

| Item | Pin number |
| :----: | :----: |
| PWM 1 | PIN 10 |
| DIR 1 | PIN 53 |

#### BOTTOM RIGHT
A : +ve convention<br>
B : -ve convention<br>
*reversed connection*

| Item | Pin number |
| :----: | :----: |
| PWM 2 | PIN 9 |
| DIR 2 | PIN 51 |

### MOTOR DRIVER 2
#### BOTTOM LEFT
A : -ve convention<br>
B : +ve convention<br>

| Item | Pin number |
| :----: | :----: |
| PWM 1 | PIN 8 |
| DIR 1 | PIN 47 |

#### TOP LEFT
A : -ve convention<br>
B : +ve convention<br>

| Item | Pin number |
| :----: | :----: |
| PWM 2 | PIN 7 |
| DIR 2 | PIN 49 |
