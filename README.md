# EndlessCPP

Basic endless runnner game made in Unreal 5.1.1, utilizing both C++ and Blueprints.

Download playable build from the Releases page.

## Good To Know:

- Speed only increases when your health is full
- Hitting a stone obstacle reduces your speed
- You get more points the faster you're going
- Health pickups only spawn when you're hurt

## Features Implemented

| Feature                                                                                                                          | Implemented | Comment                                                              |
| -------------------------------------------------------------------------------------------------------------------------------- | ----------- | -------------------------------------------------------------------- |
| Spawning platforms over a pre-defined area of the screen which scroll past the player at a particular rate                       | ✅          |                                                                      |
| Spawning projectiles/enemies which spawn over a pre-defined area of the screen which scroll past the player at a particular rate | ✅          | Obstacles, collectibles and enemies will appear                      |
| Increasing difficulty over time                                                                                                  | ✅          | It gets faster and faster                                            |
| Accumulated points which increase as time progresses and is saved in a high score list shown at startup                          | ✅          | Top 3 scores shown (amount can be changed in the MainMenu blueprint) |
| 3 lives in each game                                                                                                             | ✅          | Amount can be changed                                                |
| Projectile to player collision                                                                                                   | ✅          | The alien enemies will fire at you                                   |
| Basic keyboard controls                                                                                                          | ✅          | Use the arrow keys                                                   |

Most, if not all, parameters of the game can be modified:

In the World Manager actor you can modify:

- If you want to switch out the grass, replace the _Ground Piece Blueprint_
- If you want to add more obstacles, add them to the _Obstacle Blueprints_ array
- If you want to add more collectibles (coins), add them to the _Collectible Blueprints_ array
- If you want to add more/different health pickups, add them to the _Health Blueprints_ array
- If you want to add more enemies, add them to the _Enemy Blueprints_ array
- The World Manager actor also has parameters for:
  - Collectible Spawn Probability
    - Determines the chance of a collectible spawning, instead of an obstacle
  - Enemy Spawn Probability
    - Determines the chance of an enemy spawning
  - Blocks Behind Player
    - How many blocks to keep behind player (before the block furthest back is moved in front again)
  - Blocks Ahead Of Player
    - How many blocks to keep spawned in front of the player. Determines how far you can see basically.

In the GameMode you can modify:

- Health parameters (starting health and max health)
- Speed parameters
- Score parameters (how much score you get, how often)

## Some Implementation Details

I like Blueprints, and Unreal likes Blueprints, so a lot of stuff is done through that.

But I use C++ for stuff that happens "often", stuff like moving the world and spawning things.

Main scripts of interest are probably `EndlessGameMode` and `WorldMover`.

The ground pieces are pooled. It initially spawns a bunch of platforms, but when there are enough it starts moving the platform that is furthest back to the front.
Further improvement would be to also pool the obstacles, collectibles and projectiles.

Score is also counted and incremented through C++. Though, the HUD is updated through Blueprints.

# Part 2 (Converting to Two Players)

My biggest issue when reworking the code for 2 players was that I had initially done a lot of things in the Game Mode, like keeping track of score and speed. I moved most of the Game Mode logic into Player States instead.

My second struggle was realizing it wasn't possible to use the same keyboard with 2 player controllers. My solution for this ended up being getting input inside the first player controller (which resides in the first player character) and then forward that to the second players character. Not an elegant solution but main point of this exercise wasn't dealing with input systems. I did atleast switch to using the new Enhanced Input System, IA's and IMC's.
