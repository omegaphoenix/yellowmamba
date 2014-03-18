David Qu and Justin Leong
CS 2 Assignment 9/10

David's Contribution: Wrote initial heuristic, recursive minmax, and worked on 
improving the heuristic to account for mobility, frontier discs, corners, 
adjacency to corners and other factors. Changed player-valgrind.sh to work with 
yellowmamba so memory leaks could be fixed. Helped fix memory leaks in person.     

Justin's Contribution:
Wrote random AI, original non-recursive minimax, alpha-beta pruning.
Fixed memory leaks.
Implemented iterative deepening with move ordering to improve alpha-beta.
Working on transposition table (may or may not be finished by final commit)

Improvements:
We implemented recursive min-max with alpha-beta pruning 
to search to a good depth (6-9). Our heuristic was based on a paper online that
accounted for piece difference, corners, squares adjacent to corners, mobility, 
frontier discs and a static evaluation of the squares on the board. We added
a high weight on the edges not adjacent to corners to emphasize a strategy of 
claiming the edge in anticipation of grabbing the corner. We checked timing of
our program to make sure it didn't time out.  We considered using the Mtd(f)
algorithm but ultimately failed due to time constraints.  We implemented an
iterative deepening search in order to use time most efficiently.  We limited
this to depth 9 but we plan to increase this if we successfully implement
transposition tables.

We believe in our AI because we have a sexy heuristic and we can get decent depth.
We tested our AI against our prior versions to make sure we submitted the sexiest
AI we could implement.
