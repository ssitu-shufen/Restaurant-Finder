////////////////////////////////////////////////////////////
// Names : Shufen Situ & Nicholas Rebstock                //
// CMPUT 275, Winter 2020                                 //
// Assignment 2 part 2 serial_handling.cpp                //
// Source: none                                           //
////////////////////////////////////////////////////////////


#include "serial_handling.h"

extern shared_vars shared;

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
    /*This function will communicate with the server and send the request to the server.
    Then, receive the number of waypoints and their latitude and longitude from the server
    and store them in the array shared.waypoints[].
    Args: 
        start: starting point;
        end: ending point;
    Returns: 1 if it is successful.
    */
    Serial.print("R ");
    Serial.print(start.lat);
    Serial.print(" ");
    Serial.print(start.lon);
    Serial.print(" ");
    Serial.print(end.lat);
    Serial.print(" ");
    Serial.println(end.lon);
    shared.num_waypoints = 0;
    bool noPath = false;
    int num = 0; 
    int count = 1;
    String line;
    String command;
    while (true) {
        
        while (Serial.available()) {
            line = Serial.readString();
            Serial.setTimeout(10);
            command = line.substring(0,1);
            if (command == "N") {
                // timeoutTime = millis() + 1000;
                String num1 = line.substring(2);
                num = num1.toInt();
                shared.num_waypoints = num;
                if ((num == 0)||(num > 500)) {
                    noPath = true;
                    Serial.println("No path found");
                    break;
                }
                shared.waypoints[0].lat = start.lat;
                shared.waypoints[0].lon = start.lon;
                Serial.print("Client got: ");
                Serial.println(line);
                Serial.println('A');
                continue;
            } else if (command == "W") {
                Serial.print("Client got: ");
                Serial.println(line);
                Serial.println('A');
                String latitudeS = line.substring(2,9);
                const char* cLat = latitudeS.c_str();
                int32_t lat = (int32_t) atol(cLat);
                
                shared.waypoints[count].lat = lat;

                String longitudeS = line.substring(11);
                const char* cString = longitudeS.c_str();
                int32_t lon = (int32_t) atol(cString);
                
                shared.waypoints[count].lon = lon*(-1);
                count++;
                if (count == num+1) {
                    shared.waypoints[count].lat = end.lat;
                    shared.waypoints[count].lon = end.lon;
                }
                continue;
            } else if (command == "E") {
                break;
            }
        }
        if ((command == "E")||(noPath== true)) {
            noPath = false;
            break;
        }
    }

    // 1 indicates a successful exchange, otherwise, return 0 if the communication failed.
    return 1;
}
