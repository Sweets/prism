<div align="center">
<h1>
<img src="prism.png" width="100px">
</h1>
</div>

prism is a group-based psuedo-tiling window manager for the X windowing system.

<div align="center">
<h4>What the fuck does that mean?</h4>
</div>

In a nutshell, prism puts windows into groups. Within these groups,
each window is tiled. The gimmick is that while the windows are actually
tiled, the top-level groups follow a floating paradigm.

<div align="center">
<h4>For why?</h4>
</div>

Tiling in groups allows for windows to be easily organized.
This can be useful for windows that take up less screen space, such as
terminal emulators. If a window needs to take up more space, having it be the
only window in a group will make it act like a regular floating window.

<div align="center">
<h4>hurry the fuck up</h4>
</div>

prism will be done when I finish it.
For what it's worth, prism is my current focal point for
personal projects.

- [ ] hell froze over
    - [ ] Groups
        - [ ] Sub groups
        - [ ] Mapping and unmapping
        - [x] Move and resize

    - [ ] Floating
        - [x] Windows are moved as a whole part of a group
        - [x] Windows are resized and keep relative position without overflow
        - [ ] Snapping
            - [ ] Snaps to window sides
            - [ ] Follows gap setting
        - [ ] Windows don't go into dead zones (off screen)

    - [ ] Tiling
        - [x] Users specified directional splitting
        - [x] Windows properly reclaim space in a group if another is closed
        - [ ] Follows gap setting

    - [ ] Window rules
        - [ ] Group by regex matched name or class
        - [ ] Sub groups for multi-window processes (see: gimp)

    - [ ] Multimonitor
        - [ ] Windows don't go off the visible view port
        - [ ] Dead zones as specified by the user

    - [ ] IPC
        - [x] Quit
        - [x] Specify tile direction
        - [ ] Map and unmap top-most focused group
        - [ ] Reload configuration

    - [ ] Configuration
        - [ ] Window decorations
        - [ ] Specify window rules
        - [ ] Dead zone size

    - [ ] Conveniences
        - [ ] Gaps in tiled groups
        - [ ] Window decorations
            - [ ] Simple borders (single, double, triple)
            - [ ] Complex (handles, close buttons)
        - [ ] Window swallowing
