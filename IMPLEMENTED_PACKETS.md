[x]    C→S: Handshake State=2
[x]    C→S: Login Start
[x]    S → C: Login Success
[x]    C → S: Login Acknowledged
[ ]    C → S: Serverbound Plugin Message (Optional, minecraft:brand with the client's brand)
[ ]    C → S: Client Information (Optional)
[ ]    S → C: Clientbound Plugin Message (Optional, minecraft:brand with the server's brand)
[ ]    S → C: Feature Flags (Optional)
[x]    S → C: Clientbound Known Packs
[ ]    C → S: Serverbound Known Packs
[ ]    S → C: Registry Data (Multiple)
[ ]    S → C: Update Tags (Optional)
[ ]    S → C: Finish Configuration
[ ]    C → S: Acknowledge Finish Configuration
[ ]    S → C: Login (play)
[ ]    S → C: Change Difficulty (Optional)
[ ]    S → C: Player Abilities (Optional)
[ ]    S → C: Set Held Item (Optional)
[ ]    S → C: Update Recipes (Optional)
[ ]    S → C: Entity Event (Optional, for the OP permission level; see Entity statuses#Player)
[ ]    S → C: Commands (Optional)
[ ]    S → C: Update Recipe Book (Optional)
[ ]    S → C: Synchronize Player Position
[ ]    C → S: Confirm Teleportation
[ ]    C → S: Set Player Position and Rotation (Optional, to confirm the spawn position)
[ ]    S → C: Server Data (Optional)
[ ]    S → C: Player Info Update (Add Player action, all players except the one joining (the Notchian server separates these, you don't need to))
[ ]    S → C: Player Info Update (Add Player action, joining player)
[ ]    S → C: Initialize World Border (Optional)
[ ]    S → C: Update Time (Optional)
[ ]    S → C: Set Default Spawn Position (Optional, “home” spawn, not where the client will spawn on login)
[ ]    S → C: Game Event (Start waiting for level chunks event, required for the client to spawn)
[ ]    S → C: Set Ticking State (Optional)
[ ]    S → C: Step Tick (Optional, the Notchian server sends this regardless of ticking state)
[ ]    S → C: Set Center Chunk
[ ]    S → C: Chunk Data and Update Light (One sent for each chunk in a circular area centered on the player's position)
[ ]    S → C: inventory, entities, etc.