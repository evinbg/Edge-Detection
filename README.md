# Edge-Detection
C++ program that takes a raw image and performs edge detection

## What it does
This program does matrix operations on the image using both 3x3 and 5x5 sobel operators to generate a gradient magnitude image. <br><br>
This gradient magnitude image is then used to perform an algorithm that detects the edges of the image within a certain threshold. <br>
Executing the code will produce 4 raw image files: 3x3 gradient magnitude, 5x5 gradient magnitude, 3x3 detected edges, 5x5 detected edges

## Execution
Before executing the program, you must make sure that you have a raw image file named "image.raw" in <br>
the same directory as the main.cpp file. You must also make sure that you change the variables of <br>
<code>HEIGHT</code> and <code>WIDTH</code> in the main.cpp file to the correct height and width of the image. <br>
The <code>THRESHOLD</code> variable can be changed to adjust the edge detection sensitivity.
