@mkdir build-win32 >nul
pushd build-win32 >nul
cmake -G "Visual Studio 14" ..
cmake --build . --config Release
popd >nul
