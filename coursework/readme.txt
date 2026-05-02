================================================================================================================

                                    ULTIMATE CONNECT 4- READ ME

================================================================================================================

PLATFORM:

    The game runs only on Windows System as the program uses Windows API for console supported colors, timer
    system and Sleep functionality. The game was built and tested on Windows 11. So it if suggested to use a
    Windows 10 or later versions. It will not compile or run on MAC or Linux.

PREREQUISITES:

    - Windows 10 or later versions
    - MinGW-64(c++ compiler): it must be installed and added to your environment variables

FILE STRUCTURE:

    Please ensure these files are present in your directory:
        -globals.h
        -filehandler.cpp
        -game_board.cpp
        -game_replay.cpp
        -players.cpp
        -scoreboard.cpp
        -win_chacks.cpp
        -ultimateConnect4.cpp
        -game.exe

COMPILATION:

    Open a terminal (Commant prompt or powershell) and navigate to the directory which includes all the
    files mentioned above. Then run this command:
        g++ ultimateConnect4.cpp filehandler.cpp game_board.cpp game_replay.cpp players.cpp scoreboard.cpp win_checks.cpp -o game

    If this command shows errors please add -std=c++11 at the end to explicitly state the version of the compiler. 

EXECUTION:

    After successful compiling, run this command:
        Command prompt: game.exe
        powershell: ./game or ./game.exe
    NOTE: please make sure your terminal is in the same directory as game.exe before running it.