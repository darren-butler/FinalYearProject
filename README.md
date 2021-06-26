## About
As part of our 4th year Applied Project & Minor Dissertation we set out to research and explore new technologies to produce an application that adheres to game development industry standards. Our aim was to create a game that incorporated the core features of networked multiplayer, procedural level generation, A.I. controlled enemies, and other minor gameplay systems.

## Screencast Demonstration
A screencast feature overview can be found here - https://web.microsoftstream.com/video/aeff5ed3-9be5-4ce7-bb26-70f898e28bdc
(Note: You may need to be logged into a GMIT Microsoft account to see the video)

## Game Features
- A working Unreal Engine 4 application
- Procedurally generated levels
- A.I. driven enemies with authentic behaviours
- Networked multiplayer matchmaking and session management
- Limited networked gameplay
- Real time combat mechanics with targeted sparring
- Inventory system with stackable consumable items and weapons
- Online scoreboard hosting player scores
- Customisable character options persisted in local storage
- Fully animated character actions

More detailed descriptions of these features and their implementation can be found in our dissertation, also in this repository.
 
## Pre-Requisites for the Project
There are several programs and external plugins required to running the code in this repository:
- Epic Games Launcher / Unreal Engine 4.25.4 - https://www.unrealengine.com/en-US/download
- Steam and Account Login (only to run networking features) - https://store.steampowered.com/
- Advanced Sessions Plugin 4.25.3 - https://forums.unrealengine.com/t/advanced-sessions-plugin/30020
- EPIC Leaderboard Plugin 4.25 - http://epicleaderboard.com/

## How to Run
### Cloning the Repository
1) Clone the repository.
```sh
git clone https://github.com/dnbutler64/FinalYearProject.git
```
Cloning the repository may take some time, an appropriate .gitignore file was used, however Unreal Engine projects tend to be quite large.

### Plugin Setup
2) Next you will need to create a folder "Plugins" inside the newly cloned repository. 

3) Once this folder has been created, copy the three required plugin folders into "Plugins". The three folders needed should be name:
- AdvancedSessions
- AdvancedSteamSessions
- EpicLeaderboard

### Unreal Engine
4) In the "FinalYearProject" folder, there should be a file called: "FinalYearProject.uproject". Right click this file and select "Generate Visual Studio project files".

5) Once this has completed, double click the file. You will be prompted to rebuild missing modules, select yes. This may take some time. 

6) The Unreal Engine editor should have opened our project. You can run the game in editor by clicking the Play button or "alt+p" on the keyboard. (Note: some networking features cannot be tested in editor as is)
