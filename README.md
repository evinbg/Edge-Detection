# Edge-Detection
C++ program that takes a raw image and performs edge detection

## What it does
This program does matrix operations on the image using both 3x3 and 5x5 sobel operators to generate a gradient magnitude image.<br>
This gradient magnitude image is then used to do perform an algorithm that detects the edges of the image within a certain threshold.

## Execution
Before executing the program, you must make sure that you have a raw image file named "image.raw" in <br>
the same directory as the main.cpp file. You must also make sure that you change the variables of <br>
<code>HEIGHT</code> and <code>WIDTH</code> in the main.cpp file to the correct height and width of the image. <br>
The <code>THRESHOLD</code> variable can be changed to adjust the edge detection sensitivity.
