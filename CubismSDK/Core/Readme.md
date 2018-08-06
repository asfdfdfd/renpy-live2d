# Cubism 3.0 SDK Core

This folder contains header and platform-specific library files for developing native applictions.
What's the Core anyway? It's a small library that allows you to load Cubism models in your application.


## Getting Started

You probably want to play back animations and want to display your models on screen, too.
[Go here](https://github.com/Live2D/CubismNativeComponents/tree/early-access) for further information.

## File Structure

```
CORE
├─include                    ; Header file
└─lib                        ; Library files
    ├─android                 ; For Android
    │  ├─arm64-v8a             ; For ARM64 architecture
    │  ├─armeabi-v7a           ; For ARM architecture
    │  └─x86                   ; For x86 architecture
    ├─experimental            ; For experimental
    │  ├─emscripten            ; For Emscripten
    │  └─rpi                   ; For Raspberry Pi
    ├─ios                     ; For iOS
    │  ├─xxx-iphoneos          ; For Device SDK
    │  └─xxx-iphonesimulator   ; For Simulator SDK
    ├─linux                   ; For Linux
    │  └─x86_64                ; For x86-64 architecture
    ├─macos                   ; For macOS
    └─windows                 ; For Windows
       ├─x86                  ; For x86 architecture
       │  ├─120                ; Built from VS2013
       │  └─140                ; Built from VS2015
       └─x86_64               ; For x86-64 architecture
          ├─120                ; Built from VS2013
          └─140                ; Built from VS2015
```

Note: For Windows static library, the suffix (e.g. `_MD` `_MDd` `_MT` `_MTd`) refers to the "Use Run-Time Library" option used to compile the library.
