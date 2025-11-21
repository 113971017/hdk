This example demonstrates how to get real values for temperature,
pressure, battery voltage and acceleration.

WARNING!    Please select a correct profile `PROFILE` in the `Makefile` which
            matches the device.

Add the following expression to C-Debugger's Watches to get real values
for the temperature (in deg.C), pressure (in kPa) and voltage (in V)        
    processed_t / 256.0 - 52
    processed_p / 65536.0
    processed_v / 100.0
    processed_accel_x / 10.0
    processed_accel_z / 10.0

To measure power consumption:

1) In order to set 1V per 1 mA gain of current monitor with 10 Ohm shunt
   configure the EVB91804 board Rev 2.5 as below:
   JP1=Opened, SW3=(1-2), SW4=(1-3), check output signal on pin 2 of the CON2
   JP2 can be opened temporarily just to get better picture of the current consumption dynamics

2) Measure output signal by oscilloscope connected to CON2 connector

    Toggling of GPIO0 pin indicates the start/stop time of the measurement procedure

    On the scope:
    - press Math and select Integration function: Sdt
    - press Settings
        Source: select channel connected to CON2
        Scale: adjust to fit the integrated waveform into the function boundaries (indicated by GPIO0)
    - press Cursor
        Source: Math
        X: width of GPIO (start/stop of the procedure)
        Y: put to the points where integrated waveform intersect function start/stop points (X cursor)

    - read measurement dY(M)
