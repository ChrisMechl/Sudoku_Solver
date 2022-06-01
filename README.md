# Sudoku_Solver

A program to solve given sudoku puzzles stored in .txt files. The puzzle file should consist of 9x9 characters where a '.' indicates an empty slot (see Puzzles directory for examples).

The program can be asked to show steps as well as print the time it took to solve the puzzle using -s and -t flags respectively. The Makefile has useful macros to help you run different puzzles and show steps or time the solution. You will need to change the PUZZLES variable to the path of the puzzles as stored on your machine. To run without utilizing the Makefile compile the source and use the command "./main <puzzle location> <optional flag>".
  
This program uses a 2D array of shorts to store the possible values for each index of the 9x9 puzzle. If the value of that index is known, the short of that index is zero. If it is not known, the short of that index uses the 9 most least significant bits to store a 1 if that number could possibly fit, and a 0 if that number could not fit. 
ie. If the index could possibly be the number 9, 5, 2, or 1, the short at that index would be 0000 0001 0001 0011 to represent that in the smallest data structure possible. 
Each index is then chronologically checked by comparing the known numbers in the row, column, and 3x3 square to remove numbers (bits) from the possibility short that represents it. If an index contains only one possibility, record that possibility as part of the solution and remove that number from the possibilities of all indexes in the row, column, and 3x3 square.
  
Some puzzles will require the solver to make a guess (usually because two indexes have the same possibilities and the rest of the puzzle does not remove a possibility from either of them). This program currently can only make one guess at a time, and if the puzzle becomes unsolvable after making the guess, can revert the guess, remove the guessed number from the possibility short, and make a new guess. What this program cannot yet do is handle making multiple guesses in a row and reverting each guess one at a time. This means it can't solve all the hard puzzles yet and will hang or crash.
