# About
This is an example of how to use [CEF](https://code.google.com/p/chromiumembedded) as an off-screen renderer using ogre3d as platform independent renderer.
The Example renders http://deanm.github.io/pre3d/monster.html to a 1024x768 texture.
## Authors
[Markus Lanner](http://markus-lanner.com)

[kenkit](https://github.com/kenkit/cef_osr/commits?author=kenkit)
## Building
Use [Cmake](http://cmake.org) and a C++11 compiler.
Tested with latest Ogre and latest cef binaries as of 1/5/2019.
## Running the example

Download prebuilt cef binaries and extract them somewhere

Make sure you set set(CEF_ROOT XXXX) within the cmakelists

This is what you will get
![cef sample]( https://i.imgur.com/pvziPKd.gif "Cef")

You might need to add an ogre.cfg (Usually found in documents) to the binary dir.

Ensure your resources.cfg/plugins.cfg is setup properly. If not use d3d9 render
