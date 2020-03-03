# Introduction

This is a tiny PacMan inspired ASCII terminal game with a small Caesar Crypto Application. My intention for making this game is to show my C++ skills as I apply for a job as a programmer. Besides that I had a lot of fun while developing and playing!

# Requirement

The only library I use beside the standard libraries is *ncurses*. Just make sure to provide it. I run this program on MacOS and Linux Debian and use typical package manager:

        apt-get install libncurses-dev
        brew install ncurses

In both cases I use the *g++* compiler with the command:

        g++ -o cyberGame ./source/*.cpp -lncurses -std=c++11

Run the game with

        ./cyberGame
