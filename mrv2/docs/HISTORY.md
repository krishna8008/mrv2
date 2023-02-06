v0.3.2
------

- Updated the build to rely on media-autobuild_suite exclusively on Windows.
- Fixed menu bar hiding not showing in the view menu properly
  (it was always on).
- Fixed Spherical environment mapping (not using a shader anymore).
- Added Cubic environment maps with the OpenEXR distribution.
- Fixed repositioning of text input field when clicking inside the text input.
- Fixed locating libintl.h on Windows.
- Added this HISTORY.md file to docs/ directory in distribution.
- Improved build instructions.
- Fixed mouse rotation of environment maps.
- Fixed middle mouse button click starting playback, like left mouse button.


v0.3.1
------

*******************************************************************************
- Linux Binary releases that work on Rocky Linux 8, RedHat 8 and Ubuntu 20.04.
*******************************************************************************

*******************************************************************************
- Added a Dockerfile for easy building and disting on all Linux distros.
  The base distro it builds on is Rocky Linux 8.
*******************************************************************************

*******************************************************************************
- Updated manual building documentation for Rocky Linux, Ubuntu, macOS and
  Windows separately to make it clearer.
*******************************************************************************

- The main executable is mrv2.exe (Windows) or mrv2.sh (Linux / macOS ).
- Fixed the build system to use mrv2 everywhere instead of mrv2 or mrViewer2.
- Added getting all .so dependencies in CMake to distribute the executable
  appropiately.
- Changed hard-coded file extensions to use Darby's IO plugin system.
- Fixed crash on Linux GNOME when using native file requester.
- Added tooltips to Read Ahead/Read Behind caches to clarify they are in
  seconds.
- Added single click playback and stop on the view window, like RV.
- Added Doxygen documentation (very incomplete).
- Added displaying of spherical environment maps in a virtual sphere
  ( courtesy of an open source OpenRV shader from The Mill ).
- Updated building documentation for Rocky Linux, Ubuntu, macOS and Windows.
- Added pen size change thru hotkeys.
- Fixed thumbnail creation on Windows.
- Removed memory leak of thumbnail creation.

v0.3.0
------

- Improved UI: menus, status bar, functionality.
- Moved status bar and status tool to bottom of the screen.
- Added preferences and menu toggle for status bar.
- Added a Panel menu to hold all dockable panels/windows.
- Added a One Panel Only toggle to show one panel at a time instead of packing
  all panels one after the other.  Floating windows are not effecte by this
  setting.
- Fixed video layer (channels) displayed when switching from one clip version
  to the next.
- Added a gamma switch to switch between 1 and the previous value.
- Added CONTRIBUTORS.md list.
- Automated version bumps in C++ code by looking at cmake/version.cmake.
- Fixed a refresh bug in FPS display when selecting Default FPS.
- I finally fixed a horrible FLTK crashing bug on thumbnail on timeline slider.
- Fixed a  crash when setting loop mode with no media loaded.
- Fixed playback of clips where fps did not match tbr.
- Fixed autoplayback when setting is set in the preferences.


v0.2.0
------

- Added support for multipart OpenEXR files.
- Fixed crashes on Windows due to time slider thumbnail.
- Made time slider thumbnail appear.
- Improved redrawing of thumbnails.
- Fixed crash on too long attributes when displayed in the HUD.
- Moved all tools into their own library (mrvTools).
- Fixed cursor drawing and slow performance of drawing tools.
- Fixed default gamma keyboard shortcuts not working.
- Added a rather rudimentary OTIO Playlist.  You select clips in the file
  window, change their in/out points and add them to the Playlist.
  When the playlist is done, you click OT Playlist and the clips are
  assembled in an otio file that is saved in $TEMP.
  Currently, you cannot nest OTIO files within another OTIO file.
- Added menu entry for Presentation mode.
- Added menu entries for deleting an annotation and all annotations
  from the movie.
- Made annotation menus appear as soon as a drawing is made.