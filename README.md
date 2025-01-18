# PixelMatrix

![1730395261623 2040x1536](https://github.com/user-attachments/assets/baf4c97d-f4ce-478e-8573-d94cbe39a0a4)

 [Matrix IDE](https://matrix.tim-arnold.de)


## Project Overview
PixelMatrix is a project that uses an LED matrix to display various animations and games. The project leverages an ESP8266 for controlling the LED matrix, allowing for dynamic and interactive displays. The system is designed to be extensible, making it easy to add new animations and applications.

## Connecting to the LEDMatrix

### Using the Hotspot
1. **Power up your LED Matrix**: Ensure your device is powered on and ready.
2. **Connect to the Hotspot**: On your Wi-Fi-enabled device, search for and connect to the Wi-Fi hotspot named `PixelMatrix` followed by the PixelCode displayed on the bottom right.
3. **Start Playing**: After the connection is established a browser window with the controls should pop up. If not navigate to `matrix.local` or `matrix.de`

## Creating and Registering an Application

### Steps to Create an Application
1. **Create a new Project**: Create a new Project at https://matrix.tim-arnold.de
2. **Select the new template**


### Connect to the led matrix
After a wifi connection as described in **Using the hotspot** is established, you can reload the connection in the Matrix Connection box.

## Using Matrix functions
You can check the cheatsheet in the help section or start with an example to see all commands and functions available.

## Using OTA (Over-the-Air) Updates

Download firmware here: [firmware.bin](https://github.com/WRI-Obernburg/PixelMatrix/releases)

### Steps for OTA Updates
1. **Connect to the Device**: Ensure your device is connected to the LED Matrix.
2. **Navigate to the Update Page**: Open a web browser and navigate to `http://matrix.local/update`.
3. **Upload Firmware**: Follow the on-screen instructions to upload your new firmware.

