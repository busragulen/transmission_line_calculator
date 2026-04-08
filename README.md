# transmission_line_calculator
C program to calculate transmission line parameters (R, L, C, ABCD) and circuit models for 154kV and 400kV high voltage networks.

# Transmission Line Parameter Calculator (C)

This project is a computer-aided analysis tool developed in C to calculate the electrical parameters of high-voltage power transmission lines (154 kV and 400 kV). 

It was developed as part of the "Electric Power Transmission" course project at Kocaeli University, Department of Electrical Engineering.
I developed the software and wrote the entire C code. The theoretical calculations and formulas were prepared by my teammates.

## Project Features

The program calculates the following parameters for 4 different transmission tower configurations based on the user-selected conductor type and line length, presenting the results in a comparative table:

* **R (Ω/km):** Alternating Current (AC) resistance
* **L (mH/km):** Inductance
* **C (nF/km):** Capacitance
* **Z (Ω/km):** Series impedance
* **Y (S/km):** Shunt admittance
* **A, B, C, D Parameters:** Long transmission line circuit model (L > 200 km)

## Supported Configurations

### Conductor Types
The project defines 8 different conductor types:
1.  795 MCM Drake
2.  795 MCM Tern
3.  954 MCM RAIL
4.  1192.5 MCM BUNTING
5.  477 MCM Hawk
6.  336.4 MCM Linnet
7.  954 MCM Cardinal
8.  1272 MCM Pheasant

The program analyzes 4 different coordinate-based tower geometries:
* **Type 1:** Single Circuit (No Bundle - PA Suspension)
* **Type 2:** Single Circuit (3-Bundle - Horizontal Layout)
* **Type 3:** Double Circuit (No Bundle - Vertical Layout, TA)
* **Type 4:** Double Circuit (2-Bundle)
