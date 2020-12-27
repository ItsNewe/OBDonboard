# OBDOnBoard - An OBD2 based digital dash oriented towards K-Line equiped vehicles

OOB is mean to be used on a microcontroller or microcomputer such as Arduinos or Raspberry Pis. This project is actively developed based on a Raspberry pi 3 B+
This software, while working for all OBD standards, isn't optimized for vehicles equipped with a CAN Bus, as it uses standard OBD commands instead of sniffing the CAN bus

## Installation

# Dependencies
- ncurses
- fmt

# Compilation

```bash
> mkdir build && cd build
> cmake ..
> make
```

Then execute the `OBDOnBoard` binary created into the `build/` directory
The planned physical build is as follows

![diagram](https://i.ibb.co/Bs13ZwP/Untitled-Diagram-1.png)
