# DateTimeLib++

```
 _____         __          _______ __                  _____   __ __             __     __   
|     \.---.-.|  |_.-----.|_     _|__|.--------.-----.|     |_|__|  |--. .----._|  |_ _|  |_ 
|  --  |  _  ||   _|  -__|  |   | |  ||        |  -__||       |  |  _  | |  __|_    _|_    _|
|_____/|___._||____|_____|  |___| |__||__|__|__|_____||_______|__|_____| |____| |__|   |__|  
                                                                                             
```
*Note: only tested with c++20, clang++ on osx intel/arm and linux.*

## What it does

* helper methods to get timestamps in seconds (unix timestamp) and milliseconds
* provides a wait timer to syncronize start events to the second
* converts timestamp to iso date string
* parese an iso date string to minutes

## cmake

* builds both a static (libdatetime.a) and shared (libdatetime.so or .dll) library.
* exports an installable CMake package for easy linking in other projects (find_package(datetimelib)).
* installs headers to include/ and libraries to lib/.

## API

_still working on it..._

## Unit Tests

* tested on osx x_86 and arm86
* tested on ubuntu linux
* need more tests...

## Install

* brew install spdlog
* sudo apt install libspdlog-dev -y

## CMake Example

* include as a sub project
* build and install locally
* header only version?

## Use Cases

_comming..._

###### 2025.02.18 | dpw
