#Musical Melee

##Creating music with Super Smash Brothers Melee

##What does it do?

This project allows you to customize the sounds coming out of Super Smash Brothers Melee. Information about what is happening on screen is pulled out from memory.

This data includes:
+ Character X position
+ Character Y position
+ Character stocks remaining
+ Character percent
+ Current stage
+ jumps left
And much more.

###How was it built?

It is built using C++ and [ChucK](http://chuck.cs.princeton.edu)

![ChucK logo] (http://chuck.cs.princeton.edu/doc/images/chuck_logo3.jpg)

The two languages communicate over the [Open Sound Control (OSC)] (http://opensoundcontrol.org/) protocol.

It was built off of the work done by github user altf4 on his [SmashBot] (https://github.com/altf4/SmashBot) AI project.

###What do I need to use it?

+ A mac or linux computer. We hope to be able to expand to Windows sometime in the future.
+ ChucK, a free audio programming language.

Mac: you can install it with [homebrew](http://brew.sh/)
```
brew install chuck
```

Linux: your package manager may have a copy
+ fedora
  ```
  dnf install chuck
  ```
+ ubuntu
  ```
  apt-get install chuck
  ```
  
If you are having trouble installing, check out the ChucK documentation: [http://chuck.cs.princeton.edu/doc/build/] (http://chuck.cs.princeton.edu/doc/build/).

+ A copy of the [Dolphin](https://dolphin-emu.org/) wii/gamecube emulator, version 5.0 or later.
+ A copy of Melee v1.02 NTSC.
+ Apply the Melee Netplay Community Settings Gecko Code. Right click on the iso -> properties -> Gecko Codes -> Netplay Community Settings.
+ Enable cheats in DOlphin. Config -> General -> Enable Cheats
+ Lastly, you may want a gamecube controller and an adapter for use on your computer.

###I have that stuff, now what?

+ Download or clone this project to your computer. 

(the "smashbot" branch is similar to "master" but also includes the smashbot ai)
+ Inside of the project folder, run
```
make
```
+ Then, run the MME executable
```
./mme
```
+ And then the ChucK program
```
chuck main.ck
```
+ And start up Melee and you should start hearing the new sound effects

### I can't get it to work!
Musical Melee is still a young project and will have issues.
