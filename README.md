# Transmission Line Parameter & Power Analysis Calculator

A comprehensive C-based simulation tool to calculate transmission line parameters (R, L, C, ABCD) and perform advanced power, efficiency, and series compensation analysis for 154kV and 400kV high-voltage networks.

This project was developed as part of the "Electric Power Transmission" course at Kocaeli University, Department of Electrical Engineering (Group 6). I developed the software architecture and wrote the entire C code, while the theoretical calculations and mathematical modeling were prepared collaboratively with my teammates.

## Project Structure

The repository is divided into two progressive stages:

* **`iletim1.c` (Phase 1):** Calculates the core electrical parameters and circuit models for various conductor and tower configurations.
* **`iletim2.c` (Phase 2):** Builds upon Phase 1 to perform dynamic power flow, line profiling, P-V curve generation, and series compensation analysis.

## Features

### Phase 2: Advanced Power & Compensation Analysis (New in `iletim2.c`)
Building on the foundational parameters, the Phase 2 module simulates real-world grid scenarios:
* **Power Flow & Efficiency:** Calculates sending-end voltage, current, active/reactive power, voltage regulation, and line efficiency based on user-defined receiving-end loads (Surge Impedance Loading - SIL) and power factors.
* **Distance-Based Line Profiling:** Generates a 10-point mathematical profile of voltage, active power, and reactive power along the entire length of the transmission line. 
* **P-V Curve Generation:** Simulates grid stability by scaling the load from 10% to 150% (k = 0.1 to 1.5) and calculating the required sending-end voltage.
* **Series Compensation & Overload:** Evaluates the grid's survivability under extreme overloads (10x Nominal Load) and tests the restorative effects of 30% and 50% series capacitive compensation using the Equivalent Pi Circuit Model.

### Phase 1: Core Parameters (`iletim1.c`)
Calculates the following baseline parameters based on coordinate geometry:
* **R (Ω/km):** Alternating Current (AC) resistance
* **L (mH/km):** Inductance (based on GMD and GMR)
* **C (nF/km):** Capacitance
* **Z (Ω/km) & Y (S/km):** Series impedance and shunt admittance
* **A, B, C, D Parameters:** Long transmission line circuit model using complex hyperbolic functions.

## Supported Configurations

### Conductor Types
The project database includes 8 standard ACSR conductor types:
1.  795 MCM Drake
2.  795 MCM Tern
3.  954 MCM RAIL
4.  1192.5 MCM BUNTING
5.  477 MCM Hawk
6.  336.4 MCM Linnet
7.  954 MCM Cardinal
8.  1272 MCM Pheasant

### Tower Configurations
The program analyzes 4 different coordinate-based tower geometries:
* **Type 1:** Single Circuit (No Bundle - PA Suspension)
* **Type 2:** Single Circuit (3-Bundle - Horizontal Layout, 3PA1)
* **Type 3:** Double Circuit (No Bundle - Vertical Layout, TA)
* **Type 4:** Double Circuit (2-Bundle, 2A)
