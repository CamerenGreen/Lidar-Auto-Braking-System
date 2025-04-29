# Lidar Auto-Braking System

![Demo GIF](media/demo.gif)  

A real-time auto-braking system using LiDAR (Light Detection and Ranging) to detect obstacles and automatically apply braking in vehicles. This project can be designed for autonomous vehicles, robotics, or advanced driver-assistance systems (ADAS).

## Features
- **Obstacle Detection**: Uses LiDAR to scan the environment and detect obstacles in real time.
- **Dynamic Braking**: Automatically applies braking based on distance and velocity calculations.
- **Configurable Thresholds**: Adjustable parameters for sensitivity, braking force, and detection range.
- **Real-Time Processing**: Low-latency processing for quick response to obstacles.
- **Simulation & Hardware Support**: Compatible with both simulated environmentsand real LiDAR hardware (e.g., RPLIDAR, Velodyne).

## Prerequisites
- **Hardware**:
  - LiDAR sensor (TFmini-01)
  - Microcontroller/Computer (Raspberry Pi, HiFive RevB01)
  - Motor driver/braking actuator (Servo Motor)
- **Software**:
  - Python 3 and C
  - ROS (optional, for simulation integration)
  - Required libraries: `numpy`, `pyserial`, `matplotlib` (for visualization)

## Installation
1. **Clone the repository**:
   ```bash
   git clone https://github.com/CamerenGreen/Lidar-Auto-Braking-System.git
   cd Lidar-Auto-Braking-System
