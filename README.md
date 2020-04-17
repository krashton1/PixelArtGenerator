PixelArtGenerator

Symbols sometimes dont load. In godot, fix lib linking issues.
Make sure all gdns point to real lib
In scene->sub resource->script, link debug dll

To package for release, need to change the linked debug dll in godot to the release dll, built by scons


# PixelArtGenerator

Pixel Art Generator is a program developed with C++ and Godot 3.1.2, with C++ bindings built. 

The program is a biome panning simulation. Biomes are procedurally generated, and interpolated between. 

All Assets are procedurally generated from a few robust generation algorithms, and a lot of input variables.

## Usage

Extract contents of 'PAG.zip'
Run PixelArtGenerator.exe

On Map Screen, click to add path markers, space to start
On Generated Screen, press right to pan faster, press left to pan slower.

## License
[MIT](https://choosealicense.com/licenses/mit/)
