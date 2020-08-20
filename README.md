# MRTN4110_PhaseD

## Setup

In order to get started working on the PhaseD Project. Ensure you first clone the Git to your workstation.

### Poetry

Poetry is a python package manager that we will be using to ensure we all have the same dependencies installed for our python environments, that all package versions match up, and that everything runs smoothly.

All you need to do to get started with poetry is:

1. Download poetry from this [link][1] and follow the Instructions based on your OS.
    - If you are using OSX/Linux then just run `$ curl -sSL https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py | python`

2. Run `$ poetry install` once you've got it all set up, and you're in the PhaseD directory.

3. Get coding!

[1]: https://python-poetry.org/docs/#installation "Poetry install Link"

## File Structure

There are two main directories we'll be working with in this project

### Legacy

Will hold all of the working code from the previous phases, completely unmodified. This is for the purpose of reference.

### src

Will contain the source code for the PhaseD functionality and contain altered versions of these files for phaseD and integration.

## General Useage

There are two components to the regular program functionality. 

1. Map Analysis
   
   - Map analysis takes place on a python script, which processes a pre-prepared image of a map, including the robot, and translates this into a .txt file containing an outline of the map for Path Planning.
  
2. Path Planning and Execution
   
   - Path Planning and Execution is performed using a C++ script run within WeBots. It takes the .txt file created in Map Analysis and implements a Path-Finding algorythm to find the shortest paths for the robot to reach the center.


### Map Analysis Usage

Ensuring you are in the `/src/Combined/` directory, type the command `$python Main.py` to run the Map Analyser. This will prompt you to select either map 1 or map 2 to perform analysis and generate a .txt map, remember your choise as it is required for the next part. Once you've selected the map, the program will display the processes performed on the Map image until the final map .txt is output to the command line and to a file called `./MapFound.txt`, this concludes map Analysis.

### Path Planning and Execution Usage

In order to plan and execute path planning on your chosen map, navigate to the `./src/Combined/worlds/` directory and select the world with the number coresponding to the one you chose for the previous part, either _world_1.wbt_ or _world_2.wbt_.

## Special Features

A Number of special features were created for the system to add to its existing functionality.

### Manual Control

Manual control grants the user the ability to control the movements of the robot according to directions given by the keyPad. In order to give these directions, open the ControlerInput world, click plat on Webots, and click on the simulation screen, this will allow you to take control of the robot.

### Live Mapping

Expanding on the previous feature live mapping allows the user to control the robot within the map and create the text file corresponding to that map as they go. The map will be revealed in the WeBots console as you explore the map using the manual control.

### Live Tracking

Live tracking is a demonstration of how tracking can be performed on the e-puck robot from above, by taking a video and running the tracking script on that video, the robot can be tracked accross the screen.