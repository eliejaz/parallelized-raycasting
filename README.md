# The project has several phases:

1. Use OpenMP to improve the frame count per second. The computations related tothe floor, ceiling, wall, and sprite casting should each be parallelized (individually, i.e.,ensuring parallel computation of each of the computations, not making the computationsof those objects each on a different thread).

2. Create a thread to parallelize the reception and the update of the positions of the other players.

3. Parallelize the position sending by creating a new thread, in which the position is sent continuously.

4. In the previous phase, the position of the player is sent continuously, even when theplayer is not moving. In this phase, to avoid sending the position unnecessarily, send itonly when the player has moved. In order to achieve this, use a condition variable that is notified by the main thread when the player has moved.