mkdir build-windows
cd build-windows

set %BOOST_ROOT% C:\BOOST\boost_1_57_0
cmake -G "Visual Studio 12 2013 Win64" ../

cd ..
