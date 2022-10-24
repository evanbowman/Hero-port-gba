# Hero Port GBA

I'm porting Hero Core, a gamemaker game I used to play when I was in highschool, to Gameboy Advance in my spare time.
Game by Daniel Remar, music by BrotherAndroid.

## Implementation status:

### Flip Hero:
- [x] Movement
- [x] Jetpack trail
- [x] weapon
- [x] blade
- [ ] burst
- [ ] warp
- [ ] Level up
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
- [ ] Eyespy


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
- [ ] Star Splitter
- [x] Elite
- [x] Eliminator
- [ ] Cruiser Tetron

- [ ] Boss death animations

Note: boss death animations involve tons of code and I haven't finished them yet. Sometimes there's as much code in the boss death sequence as in the boss itself.


### Enemy Projectiles
- [x] EShot
- [x] ESupershot
- [x] EMegashot
- [x] EBomb
- [ ] Eliteshot
- [x] Missile
- [x] Gigashot
- [x] Laser
- [ ] Tetron Laser


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
- [ ] Saving
- [x] Music
- [ ] Sound effects


### Menus
- [ ] Title screen
- [ ] World map
- [ ] Pause menu
- [ ] Intro cutscene
- [ ] Intro help text


# Building

Devkitpro gba toolchain required. Then, cd into the build folder and run ./set-gameboyadvance-toolchain. Then run make.
