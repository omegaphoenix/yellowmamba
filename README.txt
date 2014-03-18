David Qu and Justin Leong
CS 2 Assignment 9/10

David's Contribution: Wrote initial heuristic, recursive minmax, and worked on improving the heuristic to account for mobility, frontier discs, corners, adjacency to corners and other factors. Changed player-valgrind.sh to work with yellowmamba so memory leaks could be fixed. Helped fix memory leaks in person.     

Justin's Contribution:


Improvements:
We implemented recursive min-max with alpha-beta pruning 
to search to a good depth (7-8). Our heuristic was based on a paper online that
accounted for piece difference, corners, squares adjacent to corners, mobility, frontier discs and a static evaluation of the squares on the board. We added
a high weight on the edges not adjacent to corners to emphasize a strategy of 
claiming the edge in anticipation of grabbing the corner. We checked timing of
our program to make sure it didn't time out.
