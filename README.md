# Hero Port GBA

I'm porting Hero Core, a gamemaker game I used to play when I was in highschool, to Gameboy Advance in my spare time.
Game by Daniel Remar, music by BrotherAndroid.

Differences from the Desktop version:
 * This version is incomplete, see feature list below.
 * Certain features have been adjusted or removed, in places where implementing them would not be practical on a gameboy advance. Pixel-perfect collisions, for example, have been approximated with collections of bounding boxes.
 * I need to fix the minimap. The game uses 10x10 pixel tiles for the map, and gba is suited to 8x8 tiles, so I have placeholder graphics in the map until I can find time to render it correctly.


## Implementation status:

### Flip Hero:
- [x] Movement
- [x] Jetpack trail
- [x] weapon
- [x] blade
- [ ] burst
- [x] warp
- [x] Level up
- [x] overheat
- [x] damage animation


### Enemies:
#### Light
- [x] Drone
- [x] Reaver
- [x] Crusher
- [x] Spew
- [x] Turret
- [x] Blomb
- [x] Bolt
- [x] Lurk


#### Heavy
- [x] Spawner
- [x] Power Turret
- [x] Boltaray
- [x] Battle Door
- [x] Chain Snake
- [x] Phase
- [x] Soldier
- [x] Eyespy


#### Elite
- [x] Barrier
- [x] Generator
- [x] Mode
- [x] Hunter
- [x] Dire Viper
- [ ] Mother
- [x] Warp Eidolon
- [x] Annihilator


#### Bosses
- [x] Rock Smasher
- [x] Silencer
- [x] Plasma Hydra
- [x] Reaper Drone
- [ ] Liquid Metal Processor
- [ ] Grand Mother
- [x] Guardian
- [x] Star Splitter
- [x] Elite
- [x] Eliminator
- [x] Cruiser Tetron


### Enemy Projectiles
- [x] EShot
- [x] ESupershot
- [x] EMegashot
- [x] EBomb
- [x] Eliteshot
- [x] Missile
- [x] Gigashot
- [x] Laser
- [x] Tetron Laser


### Walls
- [x] Basic Walls
- [ ] Moving walls
- [ ] Elevators


### Fluids
- [x] Liquid Metal


### Misc/environment
- [ ] Water drips
- [x] Waving grass/eel things
- [x] Stars
- [x] Checkpoints
- [x] Saving
- [x] Music
- [x] Sound effects


### Menus
- [ ] Title screen
- [x] World map
- [ ] Pause menu
- [ ] Intro cutscene
- [ ] Intro help text


# Building

Devkitpro gba toolchain required. Then, cd into the build folder and run ./set-gameboyadvance-toolchain. Then run make.

# License

See LICENSE for terms of the General Public License. Furthermore, please respect the wishes of the game's original creator, Daniel Remar, who does not want the game to be sold.
