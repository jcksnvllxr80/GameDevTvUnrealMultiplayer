# GameDevTvUnrealMultiplayer (https://www.udemy.com/course/unrealmultiplayer/)
Make Local & Online Multiplayer Games. Improve Your Coding Programming Skills. Made In Collaboration With Epic Games.
^^ course description

These are my notes and code from taking this course as described above

# NOTES

## Launch a server:

Using Command Prompt (CMD)...

- "C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" -game

- C:\Users\A-A-Ron>"C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" -game -log

- C:\Users\A-A-Ron>"C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" /Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap -game -log

---- 

If you are having issues spawning servers, try the following:

Does your command line for the server look like this? "C:\\Program Files\\Epic Games\\UE_4.24\\Engine\\Binaries\\Win64\\UE4Editor.exe" "C:\\Path\\To\\My\\Project.uproject" /Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen -server -log Including ?listen and the map.

Does the client command look like this? "C:\\Program Files\\Epic Games\\UE_4.24\\Engine\\Binaries\\Win64\\UE4Editor.exe" "C:\\Path\\To\\My\\Project.uproject" 127.0.0.1 -game -log Pro Tip: 127.0.0.1 should work on any machine.

Is Steam VR or Oculus stopping multiple games from loading?

----

## Launch a client to connect to the server:

Again, using command prompt...

- C:\Users\A-A-Ron>"C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" 192.168.1.116 -game -log

- C:\Users\A-A-Ron>"C:\Program Files\UE_4.26\Engine\Binaries\Win64\UE4Editor.exe" "C:\Users\A-A-Ron\git\GameDevTvUnrealMultiplayer\PuzzlePlatforms\PuzzlePlatforms.uproject" 192.168.1.116:7777 -game -log
