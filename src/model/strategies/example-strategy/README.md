# Example Strategy

This strategy serves as an example on how to create a new strategy.
The *src* folder contains the cpp files, while the *include* folder contains the header files within a folder named according to the strategy name.

The *ExampleStrategy.cpp* files contains an *apply* function within the namespace of the strategy.
This function is called by the framework once every simulation time step, if the strategy is listed in the *sequence.conf* file in the *strategies* folder.
If you would like to add more cpp files, they have to be added to the STRATEGY_SOURCES field in the *CMakeLists.txt*.

This strategy also contains examples for the logging and alert function of the framework.
It enables to write messages to the OSI log, that can be visualized live in many simulation tools.