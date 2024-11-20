# PixelMatrix
![1730395261623 2040x1536](https://github.com/user-attachments/assets/baf4c97d-f4ce-478e-8573-d94cbe39a0a4)

## Project Overview
PixelMatrix is a project that uses an LED matrix to display various animations and games. The project leverages an ESP8266 for controlling the LED matrix, allowing for dynamic and interactive displays. The system is designed to be extensible, making it easy to add new animations and applications.

## Connecting to the LEDMatrix

### Using the Hotspot
1. **Power up your LED Matrix**: Ensure your device is powered on and ready.
2. **Connect to the Hotspot**: On your Wi-Fi-enabled device, search for and connect to the Wi-Fi hotspot named `PixelMatrix` followed by the PixelCode displayed on the bottom right.
3. **Start Playing**: After the connection is established a browser window with the controls should pop up. If not navigate to `matrix.local` or `matrix.de`

## Creating and Registering an Application

### Steps to Create an Application
1. **Create a Class**: Define a new class that inherits from `Application`.
2. **Implement Required Methods**: Implement the following virtual methods:
   - `init(MatrixManager *mm, ControlManager *cm)`
   - `draw(MatrixManager *mm, ControlManager *cm)`
   - `game_loop(MatrixManager *mm, ControlManager *cm)`
   - `clean_up(MatrixManager *mm)`
   - `on_event(Event e, MatrixManager *mm, ControlManager *cm)`

#### Example:
```c++
#include "system/Application.h"

class MyApp : public Application {
public:
    void init(MatrixManager *mm, ControlManager *cm) override {
        // Initialize application
    }

    void draw(MatrixManager *mm, ControlManager *cm) override {
        // Draw application frame
        // do not allocate new memory here
        // runs at ~30 fps
    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override {
        // Update application logic
        // runs at the specified frequency in MatrixManager->set_tps (Ticks per Second)
    }

    void clean_up(MatrixManager *mm) override {
        // Clean up resources
    }

    void on_event(Event e, MatrixManager *mm, ControlManager *cm) override {
        // Handle events
    }

    static Application* create() {
        return new MyApp();
    }
};
```

### Registering the Application
In the `setup()` function of `main.cpp`, register your application with the `SystemManager`:
```c++
void setup() {
    sm.register_application(MyApp::create, "MyApp", "AuthorName");
}
```

## Using MatrixManager and ControlManager

### MatrixManager
The `MatrixManager` controls the LED matrix, providing methods to set pixels and draw shapes.

#### Example Usage:
```c++
// Set a pixel at (x, y) to a specific color
mm->set(x, y, color);

// Turn off a specific pixel
mm->off(x, y);

// Draw a circle
mm->circle(x, y, radius, color, filled, thickness);

// find more methods in the doxygen documentation
```

### ControlManager
The `ControlManager` manages the controls and status of your application. It also runs animations.

#### Example Usage:
```c++
// Set the status displayed on the webpage
cm->set_status("Running");

// Get the current controls displayed to the user
uint8_t controls = cm->get_controls();

// Run an animation
cm->run_animation(new MyAnimation(), 1000);

// find more methods in the doxygen documentation
```

## Using OTA (Over-the-Air) Updates

Download firmware here: [firmware.bin](https://github.com/WRI-Obernburg/PixelMatrix/releases)

### Steps for OTA Updates
1. **Connect to the Device**: Ensure your device is connected to the LED Matrix.
2. **Navigate to the Update Page**: Open a web browser and navigate to `http://matrix.local/update`.
3. **Upload Firmware**: Follow the on-screen instructions to upload your new firmware.

