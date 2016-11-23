# nonogram_solver
A program to solve the Japanese puzzles called nonograms. See nonograms.org for examples of the puzzle. 
In the rows and columns, clues are listed and provides information about how many, how long, and of which color the colorized adjacent blocks should have in that row or column.

At the current state, the program is able to solve a very small nonogram. Some of the logic is done but there are still some implementations left for the program to be able to solve the larger nonograms.

What is possible now is to fill a row which has to be completely full. (1B means that there should be 1 block of blue color)
      | 1B 2B 1B
_________________
1B    | 
2B 1R |             < - in the the second row, every block has to be colorized, and they will be filled with blue blocks
           ^- same goes for the second column

The result is

        1B 2B 1B
_________________
1B    |     B
2B 1R | B   B  R      

What is at the top of the TODO-list is to let the clues in the first row, first column, and third column deduce that their blocks have been colorized. That is not useful in this case, but is a must for solving larger nonograms.

Next thing on the TODO-list is to implement the tactic of exploring extreme positions.

For example
    2B 2B 2B 2B 2B 2B 2B
________________________
4B | 
4B | 

Here we could colorize like this:
     ?  ?  ?  ?  ?  ? 
 ____________________
4B |       B  B




