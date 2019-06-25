# About
This is an example of how to use [CEF](https://code.google.com/p/chromiumembedded) as an off-screen renderer using ogre3d as platform independent renderer.
The Example renders http://deanm.github.io/pre3d/monster.html to a 1024x768 texture.

# Authors
[Markus Lanner](http://markus-lanner.com)

# Building

# Ubuntu 18.04

```bash
# install ogre3d - https://www.ogre3d.org/
sudo apt install libogre-1.9-dev 
# install cef dependencies
sudo apt install build-essential libgtk2.0-dev libgtkglext1-dev
# get the source
mkdir cef_osr && cd cef_osr
git clone https://github.com/qwertzui11/cef_osr.git
# get cef3
curl -Lo cef.tar.bz2 http://opensource.spotify.com/cefbuilds/cef_binary_75.0.11%2Bgf50b3c2%2Bchromium-75.0.3770.100_linux64.tar.bz2
# extract cef
mkdir cef && cd cef
tar -xf --strip-components=1 ../cef.tar.bz2
cd ..
# build libcef_dll_wrapper debug
mkdir cef_build_dll_wrapper_debug && cd cef_build_dll_wrapper_debug
cmake ../cef -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target libcef_dll_wrapper
cd ..
# build libcef_dll_wrapper release
mkdir cef_build_dll_wrapper_release && cd cef_build_dll_wrapper_release
cmake ../cef -DCMAKE_BUILD_TYPE=Release
cmake --build . --target libcef_dll_wrapper
cd ..
# create build
mkdir build && cd build
cmake ../cef_osr -DCMAKE_MODULE_PATH=/usr/share/OGRE/cmake/modules/ -DCEF_ROOT=../cef
# build
cmake --build .
```

## cef3

Get the latest cef3 build from http://opensource.spotify.com/cefbuilds/index.html

tested versions:
- linux 64bit: `06/20/2019 - CEF 75.0.11+gf50b3c2+chromium-75.0.3770.100 / Chromium 75.0.3770.100`

Use [Cmake](http://cmake.org) and a C++11 compiler.
Tested with ogre3d 1.9 and CEF 3.2295.2034.

# Running the example
Ensure you got the content of the folder cef/Resources/* and the executable chrome-sandbox along with the osr binary.
Ensure you copied the ogre plugins.cfg in your working directory.
