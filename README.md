# **Serial Communication Protocol Simulator**
*A C-based simulation of UART communication between embedded devices, demonstrating full-duplex communication with thread-safe circular buffers.*

---

## **📌 Project Description**

This project simulates **UART (Universal Asynchronous Receiver/Transmitter)** communication between two embedded devices using **plain C**. It demonstrates:
- **Full-duplex communication** (simultaneous transmission and reception).
- **Thread-safe circular buffers** for data exchange.
- **Real-time synchronization** using `pthread` (mutexes and condition variables).
- **Blocking receive** and **non-blocking send** operations.
- **Simulated delays** to mimic real-world UART communication.

### **Why This Matters**
UART is a fundamental protocol in **IoT and industrial automation**, used for serial communication between microcontrollers, sensors, and peripherals. This simulator helps engineers and students:
- Understand **UART’s asynchronous nature**.
- Practice **thread-safe data handling**.
- Debug and optimize **serial communication protocols**.

---

## **🔧 Features**
   Feature | Description |
 |---------|-------------|
 | **Full-Duplex Communication** | Simulates simultaneous TX (transmission) and RX (reception). |
 | **Thread-Safe Circular Buffers** | Prevents race conditions during data exchange. |
 | **Blocking/Non-Blocking I/O** | Demonstrates real-world UART behavior (e.g., waiting for data). |
 | **Simulated Delays** | Mimics transmission/reception delays for realism. |
 | **Modular Design** | Easy to extend for other protocols (e.g., SPI, I2C). |

---

## **🛠 Technical Details**

### **Key Components**
1. **Circular Buffer**
   - Thread-safe data structure for storing bytes.
   - Uses `pthread_mutex` and `pthread_cond` for synchronization.
   - Implements **producer-consumer pattern** (TX fills buffer, RX empties it).

2. **UART Device Simulation**
   - Each device has **TX and RX buffers**.
   - Simulates **random data generation** for RX.
   - Logs TX/RX events for debugging.

3. **Thread Management**
   - Dedicated threads for TX/RX operations.
   - Simulates **asynchronous UART behavior**.

### **File Structure**
