# nonogram_solver
A program to solve the Japanese puzzles called nonograms. See nonograms.org for examples of the puzzle. 
In the rows and columns, clues are listed and provides information about how many, how long, and of which color the colorized adjacent blocks should have in that row or column.

At the current state, the program is able to solve a very small nonogram. Some of the logic is done but there are still some implementations left for the program to be able to solve the larger nonograms.

What is possible now is to fill a row which has to be completely full. 

What is at the top of the TODO-list is to let the clues in the first row, first column, and third column deduce that their blocks have been colorized. That is not useful in this case, but is a must for solving larger nonograms.

Next thing on the TODO-list is to implement the tactic of exploring extreme positions.




