simpleLogger
============

A simple logger based on Boost.Log v2. It shows how to create a custom global logger, 
add a sink with two backends and a filter, add attributes and formats the log message.
It also provides some convenience macros for easy usage. 

app.cpp demonstrates how simpleLogger can be used: just include simpleLogger.h, 
link the simpleLogger library and use the LOG_* macros to write the logs.

Its tested and builds on Linux + GCC 5.4.0 and on Windows7 + Visual Studio 2013+2015.

see also http://gernotklingler.com/blog/simple-customized-logger-based-boost-log-v2/
