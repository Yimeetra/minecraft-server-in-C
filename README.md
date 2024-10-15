# Minecraft server implementation written in C.

## Current functionality:
You can enter the world, but currently it is empty :)\

### TODO:
  - [x] create client struct (socket, gameState, nickname, etc.).
  - [ ] write basic world loader or something like that.
  - [x] make possible for player to join world.
  - [x] make possible multiple connections.
  - [x] handle multiple packits in one recv.
  - [x] make server pingable.
  - [x] create packetizer (method to create packets).
  - [x] add operations with string for bytearray.
  - [x] add multithreading.
  - [x] send multiple packets if on cycle.
  - [ ] create NBT structure and methods.
  - [ ] hardcode all registries.