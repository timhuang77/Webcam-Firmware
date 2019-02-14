# Webcam-Firmware

This is the final version of the firmware developed for the WiFi webcam embedded system for a course that I took at my university. In this course, I designed a WiFi webcam from start-to-finish, accomplishing tasks such as:
* Component selection
* Component soldering
* Firmware design
* Web interface design
* Shell-enclosure 3D-printing

This repository includes the firmware for the webcam that enables the Atmel MCU (SAM4S8B) that we have chosen to fetch image frames from the camera sensor and deliver the images to the desktop client via WebSocket protocol. 


This firmware performs the following functions:
* Initialize microcontroller (MCU) i.e. configure clock, board, etc.
* Configure Zentri WiFi's USART protocol i.e. which ports are used for USART, allows USART to interrupt the MCU, peripheral clock, etc.
* Initialize communication interrupt with Zentri WiFi (parsing Zentri WiFi commands)
* manually initialize the Zentri WiFi module using its GPIO pins
* Initialize camera sensor i.e. configure I/O, clock, vsync, TWI communication (otherwise known as I2C)
* Configure camera sensor i.e. set the capture format (JPEG, YUV422 color encoding, image dimensions)
* Setup WiFi communication: configure Zentri WiFi to connect to WiFi network
* Check for WebSocket connections
* Capture image and process image buffer
* Export saved image via Websocket

## Deployment

Compiled using Atmel Studio 7 and flashed using Atmel Studio's built-in MCU firmware utility

## Built With

* [Atmel Studio 7](https://www.microchip.com/mplab/avr-support/atmel-studio-7) - The IDE used

## Acknowledgments

* Course professor for facilitating the development of this project
* Course partner
