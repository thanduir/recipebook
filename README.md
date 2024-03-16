 Recipe book
 ===========

 A Qt based desktop and android app to simplify the management of recipes and the creation of sorted shopping lists.

 Features
 --------

 - Extendable recipe database
 - Export recipes as a formatted pdf recipe book (desktop application only)
 - Plan your menus from these recipes
 - Creates shopping lists sorted by item categories
 - Adjust the order of categories to your local store layout

 Prerequisites
 -------------

- Qt 6.2
- libharu (Optional, for pdf export only)

Build
-----

For the desktop application:

- mkdir build
- cd build
- cmake -DCMAKE_BUILD_TYPE=Release ..
- cmake --build .

For local builds of the android app, it is recommended to rely on the build and deploy tools of Qt Creator.
