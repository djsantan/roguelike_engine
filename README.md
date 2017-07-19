# roguelike_engine

-TODO: Write the README.md.
-TODO: Actually do the initial commit.
-TODO: Refactor the code to be less spaghetti.

## What is this?
This is a side project I was working on to implement a simple roguelike game similar to rogue, nethack, and dungeon crawl stone soup. There
is hardly anything in it right now, so don't expect much.

## Goals
- Have some sort of netcode wherein players can move around together in the same dungeon. I don't think I want this to be real time but 
something along the lines of having a move timer if there is more then one player on a floor and if a player needs to, they can request
an extended turn time.
- Implement some sort of system where entities may possess an arbitrary configuration of body parts with various properties.

## Building
All you should need is ncurses. Just run make from the base directory of the repo.
