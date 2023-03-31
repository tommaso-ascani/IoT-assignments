# IoT assignment 3
Smart Garden system composed of 5 subsystem:
  - Garden Sensor Board (ESP) ---> Set of sensor that sends data to the Garden Service
  - Garden Mobile App (Android) ---> Mobile app for manual control of the irrigation system and lighting
  - Garden Service (PC - backend) ---> Main control unit developed in java that manage all the garden, interacting as follow:
    - Serial line with Arduino
    - Serial line with Android
    - HTTP with Dashboard
    - MQTT with Garden Sensor Board
  - Garden Dashboard (PC - frontend) ---> Web page that visualize the state of the garden
  - Garden Controller (Arduino) ---> Board for irrigation and lighting control
