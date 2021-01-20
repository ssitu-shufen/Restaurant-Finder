////////////////////////////////////////////////////////////
// Names : Shufen Situ & Nicholas Rebstock                //
// CMPUT 275, Winter 2020                                 //
// Assignment 2 part 2 server.cpp                         //
// Collaboration level: Full                              //
// Source: The part 1 solution from eclass                //
////////////////////////////////////////////////////////////
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include "wdigraph.h"
#include "dijkstra.h"
#include "serialport.h"

#include <vector>

using namespace std;

struct Point {
    /* This struct is used to store the latitude and
    longitude of Point.
    */
    long long lat, lon;
};

long long manhattan(const Point& pt1, const Point& pt2) {
    /*This function will use the latitude and longtitude of two points
    to calculate the manhattan distance.
    Args:
    pt1: first given point
    pt2: second given point
    Returns:
    abs(dLat)+abs(dLon): distance between two points
    */
    long long dLat = pt1.lat - pt2.lat, dLon = pt1.lon - pt2.lon;
    return abs(dLat) + abs(dLon);
}

int findClosest(const Point& pt, const unordered_map<int, Point>& points) {
    /*This function will find the vertex closest to the given start point and the endpoint.
    Args: 
    pt: a mapping between vertex identifiers and coordiates;
    points: given point
    Returns:
    best.first: vertex closest to the given point;
    */
    pair<int, Point> best = *points.begin();

    for (const auto& check : points) {
        if (manhattan(pt, check.second) < manhattan(pt, best.second)) {
            best = check;
        }
    }
    return best.first;
}

void readGraph(const string& filename, WDigraph& g, unordered_map<int, Point>& points) {
    /*This function reads Edmonton map data from the file and load it into the WDigraph
    object. Then, it will store vertex coordinates in the struct Point and map each vertex
    to the corresponding Point struct.
    Args:
    filename: name of the file describing a road network;
    graph: an instance of the weighted directed graph;
    points: a mapping between vertex identifiers and coordiates;
    Returns:
    none;
    */
    ifstream fin(filename);
    string line;

    while (getline(fin, line)) {
        string p[4];
        int at = 0;
        for (auto c : line) {
            if (c == ',') {
                ++at;
            }
            else {
                p[at] += c;
            }
        }

        if (at != 3) {
            break;
        }

        if (p[0] == "V") {
            int id = stoi(p[1]);
            assert(id == stoll(p[1]));
            points[id].lat = static_cast<long long>(stod(p[2])*100000);
            points[id].lon = static_cast<long long>(stod(p[3])*100000);
            g.addVertex(id);
        }
        else {
            int u = stoi(p[1]), v = stoi(p[2]);
            g.addEdge(u, v, manhattan(points[u], points[v]));
        }
    }
}


int main() {
    /*This function will communicate with the Arduino and 
    call readGraph to read the data from the given file
    and send the shortest path found using Dijkstra to the client.
    Args: none;
    Returns: none;
    */
    WDigraph graph;
    unordered_map<int, Point> points;
    readGraph("edmonton-roads-2.0.1.txt", graph, points);
    string line;
    list<int> path;
    SerialPort Serial("/dev/ttyACM0");

    while (true) {
        line = Serial.readline(1000);
        string command = line.substr(0,1);
        if (command == "R") {
            cout << "Request: " << line << endl;
            Point sPoint, ePoint;
            sPoint.lat = stoi(line.substr(2,8));
            sPoint.lon = stoi(line.substr(10,18));
            ePoint.lat = stoi(line.substr(20,26));
            ePoint.lon = stoi(line.substr(28,37));
            int start = findClosest(sPoint, points), end = findClosest(ePoint, points);
            unordered_map<int, PIL> tree;
            dijkstra(graph, start, tree);
            if (tree.find(end) == tree.end()) {
                cout << "N 0" << endl;
                Serial.writeline("N 0\n");
                string noPathText = Serial.readline();
                cout << noPathText << endl;
            }
            else {
                while (end != start) {
                  path.push_front(end);
                  end = tree[end].first;
                }
                path.push_front(start);
                string num_points = to_string(path.size());
                assert(Serial.writeline("N "+ num_points+ "\n"));
                cout << "Server Sending: " << "N " << num_points << endl;
                string clientText = Serial.readline();
                cout << clientText << endl;
            }
        } else if (command== "A"){
            cout << "Ack from client: " << line << endl;
            if (path.empty()) {
              assert(Serial.writeline("E \n"));
              cout << "Sending: E" << endl;
            } else {
              int point = *(path.begin());
              int32_t lat = points[point].lat; 
              int32_t lon = points[point].lon; 
              string latitude = to_string(lat);
              string longitude = to_string(lon);
              assert(Serial.writeline("W "+latitude +" "+ longitude + "\n"));
              cout << "Server Sending: W " << lat << " " << lon << endl;
              path.pop_front();
              string received = Serial.readline();
              cout << received << endl;
            }
        }    
    }

  return 0;
}
