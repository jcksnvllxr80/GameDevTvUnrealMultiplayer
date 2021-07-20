# GameDevTvUnrealMultiplayer (https://www.udemy.com/course/unrealmultiplayer/)
Make Local & Online Multiplayer Games. Improve Your Coding Programming Skills. Made In Collaboration With Epic Games.
^^ course description

These are my notes and code from taking this course as described above

# NOTES

## Launch a server:

Using Command Prompt (CMD)...

- "C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" -game

- "C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" -game -log

- "C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" /Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap -game -log

---- 

If you are having issues spawning servers, try the following:

Does your command line for the server look like this? "C:\\Program Files\\Epic Games\\UE_4.24\\Engine\\Binaries\\Win64\\UE4Editor.exe" "C:\\Path\\To\\My\\Project.uproject" /Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen -server -log Including ?listen and the map.

Does the client command look like this? "C:\\Program Files\\Epic Games\\UE_4.24\\Engine\\Binaries\\Win64\\UE4Editor.exe" "C:\\Path\\To\\My\\Project.uproject" 127.0.0.1 -game -log Pro Tip: 127.0.0.1 should work on any machine.

Is Steam VR or Oculus stopping multiple games from loading?

----

## Launch a client to connect to the server:

Again, using command prompt...

- "C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" 192.168.1.116 -game -log

- "C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" 192.168.1.116:7777 -game -log

----

## Launch a server to the LOBBY:

Using Powershell...

- & "C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" /Game/PuzzlePlatforms/Maps/Lobby -game
- implement some UFUNCTION(Exec) c++ code in the GameInstance class to handle console "Host" and "Join" commands
#### Launch a game to connect to the server in the same way as shown above in the "Launch a client to connect to the server" section 

----

## Play with friends over the internet without port forwarding

1. go to www.vpn.net
2. download hamachi
3. sign up for an account
4. create a new network (free accounts only get 5 computers on their network)
5. another user will join the created network with network name and password
6. get game server IP by right-clicking the computer of the game server and copying the IPV4 address
7. now a friend can connect to my machine as if on LAN by connecting to my computer


----

# Cool Websites

1. www.everytimezone.com 
  shows timezones and how they related to one another
2. www.sharedrop.io
  allows users to drag and drop files to one another
