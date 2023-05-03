# EndlessCPP

Basic endless runnner game made in Unreal 5.1.1, utilizing both C++ and Blueprints.

Download playable build from the Releases page.

## Good To Know:

- Hitting a stone obstacle reduces your speed
- Health pickups only spawn when you're hurt
- Split screen multiplayer on the same keyboard (Player 1 uses WASD, Player 2 uses Arrow Keys)
- **Every 5 blocks you dodge you get a reward**

## Features Implemented

| Feature                                                                                                                          | Implemented | Comment                                                                                                            |
| -------------------------------------------------------------------------------------------------------------------------------- | ----------- | ------------------------------------------------------------------------------------------------------------------ |
| Spawning platforms over a pre-defined area of the screen which scroll past the player at a particular rate                       | ✅          |                                                                                                                    |
| Spawning projectiles/enemies which spawn over a pre-defined area of the screen which scroll past the player at a particular rate | ✅          | Obstacles, collectibles and enemies will appear                                                                    |
| Increasing difficulty over time                                                                                                  | ✅          | It gets faster and faster                                                                                          |
| Accumulated points which increase as time progresses and is saved in a high score list shown at startup                          | ✅          | Top 3 scores shown (amount can be changed in the MainMenu blueprint)                                               |
| 3 lives in each game                                                                                                             | ✅          | Amount can be changed                                                                                              |
| Projectile to player collision                                                                                                   | ✅          | The alien enemies will fire at you                                                                                 |
| Basic keyboard controls                                                                                                          | ✅          | Use the arrow keys                                                                                                 |
| **Multiplayer**                                                                                                                  | ✅          | Split screen, same keyboard                                                                                        |
| **Reward for dodging obstacles**                                                                                                 | ✅          | Every time you dodge 5 blocks in a row (5x combo) you get bonus points and a 20% chance that all obstacles despawn |

Lots of parameters of the game can be modified:

In the World Mover actor you can modify:

- If you want to switch out the grass, replace the _Ground Piece Blueprint_
- If you want to add more obstacles, add them to the _Obstacle Blueprints_ array
- If you want to add more collectibles (coins), add them to the _Collectible Blueprints_ array
- If you want to add more/different health pickups, add them to the _Health Blueprints_ array
- If you want to add more enemies, add them to the _Enemy Blueprints_ array
- Parameters for:
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
- Reward parameters (how much bonus points, how many blocks you need to dodge)

## Some Implementation Details

I like Blueprints, and Unreal likes Blueprints, so a lot of stuff is done through that.

While I understand that this assignment is for a C++ class, I argue that Unreal wants you to use Blueprints as much as possible, so I would argue that my approach of learning and using both Blueprints and C++ is more powerful and useful in reality.

So I use **C++ for stuff that is performance critical** and happens often: spawning the obstacles/enemies, the alien "AI", manipulating the world, keeping track of score and other stats. The Game Mode and Player State cores are also made in C++. My interfaces are also done in C++ because as far as I can tell, interfaces made in C++ can be used in both code and blueprints, while interfaces done in Blueprints can only be used in Blueprints.

Blueprints does most of the other things and things that don't happen very often. It sets up the HUD, writes the save file for the high scores, populates the main menu, etc. It also does the player inputs, which might be controversial but I argue you don't do that many inputs in this game. If this was a game where you would constantly be making inputs (like holding a stick forward) I would do it through code instead. But for occasionally pressing up/down/left/right, I think Blueprints does it good enough.

---

While it looks like the player is running, he is actually still. The world moves instead. Actually, it doesn't really move either. In reality, the grassy ground has no collision. There is however and invisible platform the player is always standing on.

The ground pieces are **pooled**. It initially spawns a bunch of platforms, but when there are enough it starts moving the platform that is furthest back to the front.
Further improvement would be to also pool the obstacles, collectibles and projectiles.

Each player has their own World Mover, and each World Mover keeps track of what obstacles/enemies/collectibles it spawns in **separate TArrays**. The World Mover has a function that moves all of those objects (arrays) towards the player. Another reason why I keep them in separate TArray's is so that I can despawn all "bad things" for example, when the player dodges a bunch of blocks in succession.

I use **interfaces** for dealing damage, receiving heal and picking up coins. Using interfaces is nice because I could add a damage interface to the glass boxes to make them be able to break from alien projectiles. It's nice to be able to reuse code and logic.

I have events for:

- Score changed
- Speed changed
- Combo changed
- Health changed

With these events, I can set up the HUD to listen for them and to update the HUD when needed.

The game is also **framerate independent** as far as I can tell. Originally it wasn't because I did a lot of things inside Tick and assumed manually setting a tick interval would solve it. But it didn't, so I've moved the stuff that is framerate sensitive to be "ticked" in Timers instead. I've tried the game at 30, 60 and a variable 100-150 FPS and it seems to work consistently.

# Part 2

My biggest issue when reworking the code for 2 players was that I had initially done a lot of things in the Game Mode, like keeping track of score and speed. I had to move most of the Game Mode logic into Player States instead. This was a bit disheartening (as the huge amount of red, deleted lines in the commit will tell you) and frustrating but it was a good exercise because now I know what should be put in Game Mode and what should be put into Player State.

My second struggle was realizing it wasn't possible to use the same keyboard with 2 player controllers. My solution for this ended up being getting input inside the first player controller (which resides in the first player character) and then forward that to the second players character. Not an elegant solution but main point of this exercise wasn't dealing with input systems. I did atleast switch to using the new Enhanced Input System, IA's and IMC's.

After that was figured out, I was able to use Unreals built-in support for split screen and local multiplayer pretty seamlessly and learned a lot about player controllers in the process.

My world mover/spawning logic was already good for multiplayer support. Only thing I changed was to make it utilize Unreals built-in concept of "Instigators". This way, I can easily keep track of whose enemies are whose (I had a funny bug where the enemies would only fire at player 1 initially, because I used `GetFirstLocalPlayerController()` in the alien scripts to find their targets). I also had to move them to be placed under wherever each player was, instead of assuming (0,0,0).

As for rewarding the player for succesful play, I decided to implement a combo system. Every n blocks (5 by default) you get a reward and there is a chance all obstacles will despawn. For detecting if the player dodges, I add invisible boxes to the open slots and they increment the combo counter if triggered (health and coin pickups also increment the combo counter).

## Notable Commits

| Commit                                                                                           | Comment                                                                                                                                                                                                                                                                  |
| ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | 
| [d369a15](https://github.com/lambdan/EndlessCPP/commit/d369a158f908078d5e2853f799ef1b247fac3046) | Here I stripped out a lot of code from the Game Mode and moved it over to a new Player State class, so that the players got separate scores and such. (Game Mode was re-implemented in Blueprints, but rewritten to C++ in the next commit.)                             |     |
| [e2cc629](https://github.com/lambdan/EndlessCPP/commit/e2cc629ab93e275627f64b48948f23d90ec77d1e) | Game Mode rewritten to C++                                                                                                                                                                                                                                               |
| [13f29b5](https://github.com/lambdan/EndlessCPP/commit/13f29b57af8b0e2111e2280ce6c61192a33f4937) | Here I implemented the combo/reward system. One interesting change I did here was to separate collectibles/enemies/obstacles into separate arrays so I could only despawn enemies and made a `MoveActors` function to move them all separately, but in the same fashion. |
| [cfb0892](https://github.com/lambdan/EndlessCPP/commit/cfb08924355b9a52eb81b447ff15717fbe7973fc) | Parameters added for the combo system.                                                                                                                                                                                                                                   |
