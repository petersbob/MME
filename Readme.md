#Musical Melee

##Creating music with Super Smash Brothers Melee

##What does it do?

This project allows you to customize the sounds coming out of Super Smash Brothers Melee. Information about what is happening on screen is pulled out from memory.
This data includes:
+ Player X position
+ Player Y position
+ Player stock
+ Player percent
+ Current stage
+ Player jumps left
+ Facing direction
+ Whether a player is charging
+ Whether a player is invulnerable
And much more.

###How was it built?

It is built using C++ and [ChucK](http://chuck.cs.princeton.edu), a programming language created for "real-time sound synthesis and music creation." 

The two languages communicate over the [Open Sound Control (OSC)] (http://opensoundcontrol.org/) protocol.

It was built off of the work done by github user altf4 on his [SmashBot] (https://github.com/altf4/SmashBot) AI project.

###How what do I need to use it?

1. A mac or linux computer. We hope to be able to expand to windows sometime in the future.
2. Install chuck.
Mac: you can install it with [homebrew](http://brew.sh/)
    ```brew install chuck
    ```
Linux: your package manager may have a copy
    + fedora
    ```dnf install chuck
    ```
    +ubuntu
    ```apt-get isntall chuck
    ```

3. A copy of the [Dolphin](dolphin-emu.org) wii/gamecube emulator, version 5.0 or later.
4. A copy of Melee v1.02 NTSC.
5. Apply the Melee Netplay COmmunity Settings Gecko Code. Right click on the iso -> properties -> Gecko Codes -> Netplay Community Settings.
6. Enable cheats in DOlphin. Config -> General -> Enable Cheats
7. Lastly, you may want a gamecube controller and an adapter for use on your computer.

###I have that stuff, now what?

1. Download or clone the project to your computer.
2. Inside of the project folder, run
```make
```
3. Then, run the MME executable
```./mme
```
4. And then the ChucK program
```chuck main.ck
```
5. And start up Melee and you should start hearing the new sound effects

### I can't get it to work!
Musical Melee is still a young project and will have issues.
