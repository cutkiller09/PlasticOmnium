# README #

### What is this repository for? ###

Aggron is a collision avoidance module of EZ10 security chain.
The module receives Lidar's and velocities datas and activate an emergency stop if an obstacle is detected inside a security zone in front of the vehicle  


### How do I get set up? ###

Compilation process is handled with CMake (http://www.cmake.org/). You can use it to generate IDE project file or GNU Make files.

1. Clone repository.
2. Run CMake with GUI or command line ` cmake /path/to/dev/directory -G"Generator" `
You may find a list of generators here : http://www.cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html

### How is it organized? ###

The project is divided in Thrre parts
	- High Level services : Configuration, Intialization, Scheduler which are not modular and depends on libraries.
	- Middleware : Library and protcol (Canopen, Lidar), all those parts are modular and used on others projects
	- Drivers : drivers (Tcp, Serial, Can), all those parts are modular and used on others projects

### Dependencies ###

Project is developped on Linux system and depends on Linux libraries for Ethernet communication.

### Who do I talk to? ###

Vincent Scesa, Sebastien Loisel 
 

