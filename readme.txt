Mephbot Readme 

LAST UPDATE: 9/21/02 

Copyright (c) 2002 by Mike Gogulski 
------------------------------------------------------------------------------ 
READ THIS FIRST 
------------------------------------------------------------------------------ 
Please make sure that your operating system is updated to the 
most current patch level. 

Visit http://windowsupdate.microsoft.com/ and install ALL of the 
critical and recommended updates. Reboot, then try again. 

------------------------------------------------------------------------------ 
Known Bugs (PLEASE DO NOT REPORT) 
------------------------------------------------------------------------------ 

------------------------------------------------------------------------------ 
Installation/Setup 
------------------------------------------------------------------------------ 

Read the installation instructions in mephbot.txt completely. Follow them. 

------------------------------------------------------------------------------ 
Frequently Asked Questions 
------------------------------------------------------------------------------
Q: Where can I get support for Mephbot?

A: Primarily, on the D2 Bots forum at http://www.blizzhackers.com/

Q: I use something other than Onlyer's no-CD crack.  Why am I having problems?

A: I don't know, and I'm not going to troubleshoot them.  Switch to Onlyer's
version.

Q: How do I make it work with my Amazon/Paladin/Assassin/Druid/Necromancer/ 
Barbarian? 

A: You don't. Build a sorceress. 

Q: Diablo won't run anymore. What's wrong? 

A: Did you install to your Diablo II directory? If so, you're out of luck. 
Reinstall D2 and reinstall mephbot to its own directory. Read the directions 
next time. 

Q: When I play D2 normally, some of my hacks are all misconfigured. Why? 

A: Did you install mephbot to your D2Hackit directory? If so, you've lost 
whatever INI files and D2H modules that mephbot overwrote when you unpacked 
it. Tough luck, you should have read the directions. Reinstall mephbot to 
its own separate directory, and figure the rest out yourself. 

Q: Wacky stuff happens all the time when I run Mephbot. This didn't occur 
with the previous version I ran. What's up? 

A: Did you install Mephbot in the same directory as the previous version? If 
so, throw it all away and start over by deleting the mephbot folder and 
unpacking the ZIP file again to a new folder. 

Q: Wierd things happen with the screen. Move to top left corner doesn't work. 
Mephbot crashes. What do I do? 

A: Turn off MoveWindowToTopLeftCorner (set it to zero). 

Q: Okay, still broken. Anything else? 

A: Update Windows to the latest service pack, and install all recommended 
patches from windowsupdate.microsoft.com. Update your video drivers to the 
latest stable release from the manufacturer. 

Q: Still not working! Help!!! 

A: Set Windowed=0 in the INI. 

Q: I have a slow computer and nothing seems to work right. Can I work
around this? 

A: Use the delay settings graciously provided by Maulei: 

DelayStep=10 
DelayGameLoad=150 
DelayExitGame=20 
DelayStartNewGame=80 
DelayCreateGame=100 
DelayAfterDeath=30 
DelayOverheadUpdate=5 
DelayBeforePrecast=20 
DelayPickup=20 
MaxTickRestart=250 
MaxTickWaypoint=400 
MaxTickStairs=600 
MaxTickGame=1800 

dClick=200 
dStartDiablo=24000 
dAccLogin=15000 
dCharLogin=8000 
dHackItStart=3000 
dWaitError=6000 
dCloseDiablo=3000 
dWaitUserClose=15000 
dChannelAction=4000 

Q: As soon as I take the waypoint to Durance 2, I get "YOUR CONNECTION HAS 
BEEN INTERRUPTED". What's happening? 

A: Your character is trying to cast protection spells that she doesn't 
have. Read mephbot.txt again, then go build a character that meets all the 
requirements.  Make sure you've spelled the skill names correctly in the INI

Q: pickit is only grabbing unique rings and amulets. I keep missing uber drops!
The INI I use works just fine for PindleBot. 

A: PindleBot and Mephbot use different versions of pickit. Their INIs are not
fully compatible. Use the INI distributed with Mephbot. You can add things to
it using Pindlebot's INI as a model if you like, just be careful. 

Q: Sometimes when the bot crashes or I exit it and try to load Diablo to 
play the game normally, the game won't load or everything is EXTREMELY 
slow. How do I fix that? 

A: Bring up your Windows Task Manager and kill the old Diablo II process 
which is still running. 

Q: Shortly after game start, Diablo crashes with an error described in the 
Diablo debug log file (in C:\Program Files\Diablo II\D20????.txt) as an 
access violation that traces down through maphack.cpp!DrawAutoMapAct(). 
What's wrong? 

A: Your computer is too slow or doesn't have enough RAM for the default 
timing options. Increase DelayGameLoad in the INI file. Upgrade your 
computer. 

Q: What sorceress build do you recommend? 

A: Spend skill points as follows: 

Max warmth 
Max Frozen Orb 
Max Cold Mastery 
1 Point to each of the prerequisites to these spells 
1 Point in Shiver Armor, Energy Shield, and Thunderstorm at least 
Add +Skill items and spend other points in Static, TS, and Lightning 
Mastery to taste. 

Spend stat points as follows: 

97 Strength (for Skullder's) 
42 Dex (for Ali Baba) 
all the rest split between Vitality and Energy. 

MAX all of your resists. If you're not 75% resist all in hell, don't come 
crying to me that you die too often. 

Q: When is the next version coming out? 

A: Any minute now. Look at this file several times per hour for updates. 


------------------------------------------------------------------------------ 
Future non-features 
------------------------------------------------------------------------------ 

Here is a list of features I will NOT be implementing: 

- Support for non-Sorceress characters 

- Firewall over the moat strategy 

- Support for non-Battle.net gameplay (BNETD, Open B.net, multiplayer, single) 

- Merc healing/resurrection support 

- Support for using Mana/Rejuv potions 

- Fastwp support (unless this bug is not patched when Blizzard releases 1.10) 

- Difficulty selection 

- Running PindleBot and Mephbot side-by side or in sequence 

- Clicking on chests 

- Clicking on shrines 

- Anything specific to D2 classic 

The following features and capabilities are being worked on (or may be worked 
on) by other people. I may host the finished products alongside Mephbot, but 
they will not be part of my development efforts except to facilitate their 
development and support their authors. 

- Something that does what IPBWatch for Pindlebot does 

- Something that switches characters after one character's inventory is full 

- Installation script/configurator 

