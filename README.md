# MaiBo

MaiBo is yet another open source C++ game engine. Although it can be compiled on
Windows and Linux (and quite possibly soon for iOS and Android), it's main
target is the browser via Emscripten.

## Features

* Build via CMake for Emscripten, Windows, Unix/Linux
* OpenGL based (the intersection of OpenGL features working in WebGL and OpenGL ES 2)
* Uses SDL (and sdl events for input), SDL Image, SDL Mixer

## Build instructions

There are submodules in this repo. Don't forget to call `git submodule init` and
`git submodule update`

*More coming soon*

## Copyright

The source code outside of the `third_party` directory is distributed under the
MIT Software License. See LICENSE.txt for further details or copy
[here](http://opensource.org/licenses/MIT).

Copyright &copy; 2015 Borislav Stanimirov.

### Thanks to

The author would like to thank [Chobolabs](http://www.chobolabs.com/) for their
support and some pieces of the CMake code which were initially developed at Chobolabs and
are used here
