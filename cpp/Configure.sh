mkdir _build > /dev/null
pushd _build > /dev/null
cmake ..
cmake --build . --config Release
popd >/dev/null
