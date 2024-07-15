# automated-characterization-of-PV-cell-
In this project, we have designed an arduino UNO based device to automatically measure the key parameters of a solar cell. 
data_collection.ino is the arduino code to take the realtime data from the solar panel by varying the resistance by decoder.
processing_data.py is the python code to process the data and measure the IV characteristics, open circuit voltage, short circuit current and series and parallel resistance of the equivalent circuit of solar panel.
operating_range.m is the matlab code to find out the operating region of the project.
