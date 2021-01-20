////////////////////////////////////////////////////////////
// Names : Shufen Situ & Nicholas Rebstock  	          //
// CMPUT 275, Winter 2020                                 //
// Assignment 2 part 2 draw_route.cpp             	  //
// Collaboration level: Full	                          //
// Source: none						  //
////////////////////////////////////////////////////////////
#include "draw_route.h"
#include "map_drawing.h"

extern shared_vars shared;

void draw_route() {
	/* This function uses the array shared.num_waypoints to draw the shortest route
	between the two selected points on the map of Edmonton.
	Args: none;
	Returns: none;
	*/

	if ((shared.num_waypoints != 0)||(shared.num_waypoints != 500)) {
		int32_t waypointLat1, waypointLon1, waypointLat2, waypointLon2;
		for (int i = 0; (i < shared.num_waypoints + 1); i++) {
			waypointLat1 = latitude_to_y(shared.map_number, shared.waypoints[i].lat) - shared.map_coords.y;
			waypointLon1 = longitude_to_x(shared.map_number, shared.waypoints[i].lon) - shared.map_coords.x;
			waypointLat2 = latitude_to_y(shared.map_number, shared.waypoints[i+1].lat) - shared.map_coords.y;
			waypointLon2 = longitude_to_x(shared.map_number, shared.waypoints[i+1].lon) - shared.map_coords.x;
			shared.tft -> drawLine(waypointLon1, waypointLat1, waypointLon2, waypointLat2, TFT_BLACK);
		}
		waypointLat1 = latitude_to_y(shared.map_number, shared.waypoints[shared.num_waypoints].lat) - shared.map_coords.y;
		waypointLon1 = longitude_to_x(shared.map_number, shared.waypoints[shared.num_waypoints].lon) - shared.map_coords.x;
		waypointLat2 = latitude_to_y(shared.map_number, shared.waypoints[shared.num_waypoints+1].lat) - shared.map_coords.y;
		waypointLon2 = longitude_to_x(shared.map_number, shared.waypoints[shared.num_waypoints+1].lon) - shared.map_coords.x;
		shared.tft -> drawLine(waypointLon1, waypointLat1, waypointLon2, waypointLat2, TFT_BLACK);
	}

}
