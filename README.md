PixelArtGenerator

Symbols sometimes dont load. In godot, fix lib linking issues.
Make sure all gdns point to real lib
In scene->sub resource->script, link debug dll

To package for release, need to change the linked debug dll in godot to the release dll, built by scons