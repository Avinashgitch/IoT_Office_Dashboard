# IoT Office Dashboard

This project is an **IoT-based smart dashboard** that displays real-time office activity using an **ESP32 microcontroller**, a **0.96" OLED display**, and an **HW-478 RGB LED module**. The goal is to present attendance and team performance data visually in a compact, ambient format for quick daily insight in an office setting.  

Data is entered through a web interface and a Flask-based Python backend collects and stores inputs like:  
- Number of absentees
- Remaining projects
- Best performer of the week


## Features
- Displays absentees, pending projects, and best performer as **scrolling text** on an OLED screen.
- RGB LED module changes colour based on number of absentees:
  - Green: (≤ 4)
  - Yellow: (5–10)
  - Orange: (11–20)
  - Red: (> 20)
- Powered by ESP32 which communicates with local Flask backend over Wi-Fi.
- Web UI for updating data easily.
- An **internal API** (`/api/data`) allows the ESP32 to fetch updates and react to them automatically. The backend also provides a browser-accessible form to enter or update the data.


## Use Case
- Can be mounted near the entrance of a team room or manager’s desk.
- Easily editable by the team leader via web interface.
- Displays critical info passively.
- Although currently designed to display attendance, project status, and best performer, the system can easily be extended to display any custom data relevant to the workspace — simply by modifying the web form and backend JSON fields.



## Hardware Used
- ESP32 DevKit v1 (ESP32-WROOM-32, 30-pin)  
- SSD1306 0.96" I2C OLED display  
- RGB LED Module (HW478) – *Common Cathode type*
- Breadboard and jumper wires  
- Power source (USB/Adapter)


## ESP32 Pin Connections

| Component       | ESP32 GPIO Pin | Purpose                   |
|-----------------|----------------|---------------------------|
| RGB LED - Red   | GPIO14         | PWM output (Red channel)  |
| RGB LED - Green | GPIO12         | PWM output (Green channel)|
| RGB LED - Blue  | GPIO13         | PWM output (Blue channel) |
| RGB LED - (–)   | GND            | Common cathode (ground)   |
| OLED - SDA      | GPIO21         | I2C data                  |
| OLED - SCL      | GPIO22         | I2C clock                 |
| OLED - VCC      | 3.3V           | Power                     |
| OLED - GND      | GND            | Ground                    |


## Backend Tech

- **Python 3** + **Flask**
- JSON-based storage (`data.json`)
- Web form interface for data entry
- REST API for ESP32 device to fetch latest data


## Setup Instructions

### Backend (Flask API)

1. **Install Python dependencies**  
   Make sure Python 3 is installed. Then install Flask:
   ```bash
   pip install flask
   

2. **Run the Flask server**  
   In the project folder containing app.py, run:
   ```bash
   python app.py
   ```
    
   Access the server at:  
   `http://<your-ip>:5000`
   

4. **Use the Web Interface**  
   Open in browser:  
   `http://<your-ip>:5000`

   Use the form to update:  
   - Absentee count  
   - Project Count  
   - Best Performer
    

5. **Check the API Endpoint**  
   Open in browser:  
   `http://<your-ip>:5000/api/data`


### ESP32 (Arduino IDE)

1. **Install Required Libraries**  
   - Adafruit SSD1306
   - Adafruit GFX Library
   - ArduinoJson
   - WiFi.h


2. **Upload Code**
   - Open the .ino file in Arduino IDE  
   - Select board: ESP32 Dev Module  
   - Choose correct COM port  
   - Click Upload


3. **Connect to Wi-Fi**  
   Ensure the ESP32 and the device running Flask are on the same Wi-Fi network.  


4. **Wiring**  
   Wire the OLED display and RGB LED to the ESP32 as per the pin connection table.


5. **Power the ESP32**  
   Use USB (from your computer) or a 5V adapter.


## Folder Structure

IoT_Office_Dashboard/  
├── arduino_ide  
│ └── arduino_ide.ino  
├── templates  
│ └── index.html  
├── app.py  
├── data.json  
├── README.md  
├── requirements.txt  



## API Documentation
This API provides real-time data to an ESP32-based hardware display system that shows office absentees status, pending projects, and the best performer of the week. It also determines the RGB LED color to reflect absentee levels.

**Base URL:**  
`http://<your-server-ip>:5000`

**Endpoint:**  
GET /api/data

**Description:**:   
Returns the current office statistics and a corresponding RGB color for an LED based on the number of absentees.

**Response Format:**   
Content-Type: application/json


**Success Response Example (HTTP 200):**

{  
  "absentees": 2,  
  "best_performer": "John",  
  "led": {  
    "b": 0,  
    "g": 255,  
    "r": 0  
  },  
  "projects": 1  
}


**Error Response (HTTP 500):**

{  
  "error": "<error_message>"  
}


**Response Fields:**

| Field           | Type    | Description                             |
|-----------------|---------|-----------------------------------------|
| absentees       | Integer | No of absentees on current day          |
| projects        | Integer | No of remaining projects                |
| best\_performer | String  | Name of the weeks top performer         |
| led.r           | Integer | Red component of RGB LED color(0-255)   |
| led.g           | Integer | Green component of RGB LED color(0-255) |
| led.b           | Integer | Blue component of RGB LED color(0-255)  |


**Status Codes:**

| Code  | Status                                     |
|-------|--------------------------------------------|
| 200   | Data received successfully                 |
| 500   | Internal server error- could not read data |


**Colour Logic:**

| Absentees Range | LED Colour | RGB Value   |
|-----------------|------------|-------------|
| 0 - 4           | Green      | (0,255,0)   |
| 5 - 10          | Yellow     | (255,200,0) |
| 11 - 20         | Orange     | (255,40,0)  |
| > 20            | Red        | (255,0,0)   |


## Authors

Authors: Avinash Vinod, Adhityan AS  
Date: 13 June 2025  

