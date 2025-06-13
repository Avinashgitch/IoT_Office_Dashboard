# IoT Office Dashboard
An IoT-based smart dashboard that visually displays real-time office data such as the number of absentees, pending projects, and best performer using an **ESP32**, an **OLED display**, and an **RGB LED module**. Data is entered through a web interface and retrieved via a local Flask API.


## Features
- Displays absentees, pending projects, and best performer on an OLED screen.
- RGB LED module changes colour based on number of absentees:
  - Green: (≤ 4)
  - Yellow: (5–10)
  - Orange: (11–20)
  - Red: (> 20)
- Powered by ESP32, communicates with local Flask backend over Wi-Fi.
- Web UI for updating data easily.


## Hardware Used
- ESP32 development board  
- SSD1306 0.96" I2C OLED display  
- RGB LED Module (HW478)  
- Breadboard and jumper wires  
- Power source (USB/Adapter)


## Backend Tech

- **Python 3** + **Flask**
- JSON-based storage (`data.json`)
- Web form interface for data entry
- REST API for ESP32 device to fetch latest data


## API Documentation
This API provides real-time data to an ESP32-based hardware display system that shows office absentees status, pending projects, and the best performer of the week. It also determines the RGB LED color to reflect absentee levels.

**Base URL:**  
http://\<your-server-ip\>:5000

**Endpoint:**  
GET /api/data

**Description:**:   
Returns the current office statistics and a corresponding RGB color for an LED based on the number of absentees.

**Response Format:**   
Content-Type: application/json

**Success Response Example (HTTP 200):**

{  
  "absentees": 2,  
  "best\_performer": "John",  
  "led": {  
    "b": 0,  
    "g": 255,  
    "r": 0  
  },  
  "projects": 1  
}

**Error Response (HTTP 500):**

{  
  "error": "\<error\_message\>"  
}

**Response Fields:**

| Field           | Type    | Description                             |
| :---:           | :---:   | -----                                   |
| absentees       | Integer | No of absentees on current day          |
| projects        | Integer | No of remaining projects                |
| best\_performer | String  | Name of the weeks top performer         |
| r               | Integer | Red component of RGB LED color(0-255)   |
| g               | Integer | Green component of RGB LED color(0-255) |
| b               | Integer | Blue component of RGB LED color(0-255)  |

**Status Codes:**

| Code  | Status                                     |
| :---: | :---:                                      |
| 200   | Data received successfully                 |
| 500   | Internal server error- could not read data |

**Colour Logic:**

| Absentees Range | LED Colour | RGB Value   |
| :---:           | :---:      | :---:       |
| 0 \- 4          | Green      | (0,255,0)   |
| 5 \- 10         | Yellow     | (255,200,0) |
| 11 \- 20        | Orange     | (255,40,0)  |
| \> 20           | Red        | (255,0,0)   |


Authors: Avinash Vinod, Adhityan AS  
Date: 13 June 2025  

