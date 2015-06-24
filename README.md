# ads1015drvr
This is a simple C library with a few example programs for interfacing the rasperry pi with an ADS105 ADC

It behooves one to consult the data sheet for this devic:http://www.ti.com/lit/ds/symlink/ads1014.pdf

This is a very simple library and works without any non-standard dependencies. 

The comments in the code itself and the example programs provided should provide any necessary documentation. 

The function ads1015_read_continuous can be launched as a separate thread using ads1015_launch_continuous. This functionality has not been tested.

Finally, to compile the library yourself, simply hit make. There is a makefile provided in the examples folder as well. This is generic to any C file in that folder. To compile a specific program hit make NAME=program, where 'program' is the name of the program without the .c suffix.
