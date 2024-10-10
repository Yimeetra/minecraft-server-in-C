# Minecraft server implementation written in C.

## Current functionality:
Basically, it can do nothing.... Yet :) \
The only functionality now is recieving packets and printing their data.

### TODO:
  - [x] create client struct (socket, gameState, nickname, etc.).
  - [ ] write basic world loader or something like that.
  - [ ] make possible for player to join world.
  - [x] make possible multiple connections.
  - [x] handle multiple packits in one recv.
  - [x] make server pingable.
  - [x] create packetizer (method to create packets).
  - [x] add operations with string for bytearray