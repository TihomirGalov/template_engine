# template_engine
Template engine is a simple project given me in Introduction to Programming course in Sofia University.
In the PDF file you can find the whole task combined with examples.

# Running the project
    1.Create two files (.txt):
        One containing the template data in jinja2 sintax and another containing the data 
        which later will be written in the output file
    2. Run the project and in the console window write the path to the both files.
    3. Then in the media/ folder will be created the desired files (.txt format)

# Algorithm
The project works by first reading the first row of the input file, so we can save the names of the variables in memory.
Then we start reading the rows of that input file from row = 2 until the end of the file. For every row, we read we create
a new file with a name matching the row we are at. Then we start iterating through every character on every
row of the template file in a search for special symbols (e.g. '{', '}', '%'). If we find a special symbol, which is a begging of
a variable, we replace it, and then we print that row in the output file. If the special symbol is the beginning of a
FOR loop we iterate through the whole body of the loop and save in memory all the data, and then we place it in the output file,
for every instance of the array.

The project has some issues: For example for now it cannot work with nested loops and doesn't support if statements.
Also, if the variable name in the template file is not found in the input, the program wouldn't throw exception and will just
continue replacing the data after.