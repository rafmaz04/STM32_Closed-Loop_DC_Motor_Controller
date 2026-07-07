# STM32_Closed-Loop_DC_Motor_Controller
This project implements closed-loop speed control system for a 6V DC geared motor (FIT0450) using an STM32G431RB microcontroller. By employing a discrete PI (Proportional-Integral) control algorithm, the system maintains precise motor velocity despite varying load conditions.


As a 4th-semester Electromobility student, I designed this architecture to bypass standard polling methods in favor of a synchronized, interrupt-driven control loop based on exact timing intervals of 10 milliseconds.

Hardware Architecture:
- Microcontroller: STM32G431RB Nucleo
- Actuator: FIT0450 6V DC Gearmotor with 120 to 1 reduction
- Driver: L298N Bipolar H-Bridge
- User Interface: 2 hardware debounced push-buttons for state shifting, 20 kilo-ohm potentiometer for speed reference
- Status Indicators: 3 LEDs for Park, Reverse, and Drive states with 330 ohm current-limiting resistors
- Power Supply: 12V external supply safely scaled to motor limits via Software PWM Saturation

Core Software Features:
- Zero-Latency Control Loop: The 100Hz control loop is entirely decoupled from the CPU. Timer 6 generates a hardware TRGO event that triggers an Injected ADC
- Conversion. The CPU only interrupts when the ADC data is ready, guaranteeing a mathematically perfect sampling interval.
- Modular C Architecture: The codebase is strictly abstracted into isolated libraries including pi_controller, car_control, speed_sensor, filter, and ramp.
- Feedback: Magnetic Quadrature Incremental Encoder decoded in STM32 4X TI12 Mode yielding 3840 ticks per revolution
- Anti-Windup PI Controller: Implements mathematical clamping through back-calculation to prevent integral windup during aggressive reference steps or physical motor stalling.
- Signal Filtering: An Exponential Moving Average low-pass filter eliminates quantization noise from the discrete encoder ticks.
- Ramp Generation: A soft-start ramp generator prevents violent voltage spikes when the setpoint changes instantly.
- Deadband Injection: Software deadbands prevent high-frequency driver whining at near-zero speeds.

State Machine:
The vehicle utilizes a strict state machine to ensure safe operation:
1) PARK: Motor power is cut, integrals are zeroed, and target speed is forced to zero. Indicated by a red LED.
2) DRIVE: Forward closed-loop regulation. Indicated by a green LED.
3) REVERSE: Bipolar reverse closed-loop regulation. Indicated by a blue LED.

## Hardware Wiring & Pinout

### Motor Driver (L298N)
| L298N Pin | Connection / STM32 Pin | Function |
| :--- | :--- | :--- |
| **12V** | External 12V Supply (+) | Main power for the motor |
| **GND** | External GND & STM32 GND | **Critical:** Must share a common ground with the Nucleo |
| **5V** | Unconnected | (Assuming STM32 is powered via USB) |
| **ENA** | **PC6** (TIM8_CH1) | 100Hz PWM speed control signal |
| **IN1** | **PCx** (`MOTOR_IN1_Pin`) | Direction control (H-Bridge) |
| **IN2** | **PCx** (`MOTOR_IN2_Pin`) | Direction control (H-Bridge) |
| **OUT1** | Motor Power Terminal 1 | Power delivered to FIT0450 motor |
| **OUT2** | Motor Power Terminal 2 | Power delivered to FIT0450 motor |

### Encoder (FIT0450 Magnetic Encoder)
| Encoder Pin | STM32 Nucleo Pin | Function |
| :--- | :--- | :--- |
| **VCC** | 3.3V (from Nucleo) | Sensor power |
| **GND** | GND | Sensor ground |
| **Channel A** | **PA0** (TIM2_CH1) | Quadrature Input A |
| **Channel B** | **PA1** (TIM2_CH2) | Quadrature Input B |

### User Interface (Potentiometer & Buttons)
| Component | STM32 Nucleo Pin | Function |
| :--- | :--- | :--- |
| **Potentiometer Wiper** | **PA6** (ADC2_IN3) | 0-3.3V analog speed reference |
| **Potentiometer VCC** | 3.3V | Voltage reference high |
| **Potentiometer GND** | GND | Voltage reference low |
| **UP Button** | **PCx** (`BTN_UP_Pin`) | State trigger (Internal Pull-up to GND) |
| **DOWN Button** | **PCx** (`BTN_DOWN_Pin`) | State trigger (Internal Pull-up to GND) |

### Status Indicators (LEDs)
*Note: All LEDs are connected in series with a 330Ω current-limiting resistor to GND.*

| LED Color | STM32 Nucleo Pin | System State |
| :--- | :--- | :--- |
| **Red** | **PBx** (`PARK_LED_Pin`) | PARK (Motor disabled, Integrals reset) |
| **Green** | **PBx** (`DRIVE_LED_Pin`) | DRIVE (Forward closed-loop regulation) |
| **Blue** | **PBx** (`REVERSE_LED_Pin`) | REVERSE (Backward closed-loop regulation) |
