## Building ifm3d from source on Windows

This tutorial details how to compile the ifm3d library and its dependencies on
a Windows platform using Visual Studio.

### How to install and use ifm3d binaries
1. Installation

    Download the ifm3d installer ifm3d_windows_x.x.x.exe.

    This installer provides binaries for Windows OS for Visual Studio 2019 and above. For any other compiler we recommend to build ifm3d from sources.

    Follow the instructions on the installer. It will install binaries at default location as C:/ProgramFiles/ifm3d x.x.x. Make a environment variable with this path.
    ```
    IFM3D_BINARY_DIR = C:/ProgramFiles/ifm3d x.x.x/bin      # Please put the correct install path in case installation is done on other than default path
    ```
2. Usage

    Clone ifm3d-example.

    Add install path (IFM3D_BINARY_DIR) to CMAKE_PREFIX_PATH and build ifm3d-example.

### Dependencies

#### Build tools

* [CMake v3.11.0](http://www.cmake.org) or newer (also available through the
Visual Studio installer)
* [Git for Windows](https://gitforwindows.org) (also available through the
Visual Studio installer)
* [Microsoft Visual Studio](https://www.visualstudio.com)
version 2017 or 2019. The free 'Community' edition is sufficient. Be
sure to select the 'Desktop development with C++' workflow.

#### Binary Dependencies for ifm3d::image and ifm3d::opencv optional modules

##### [PCL](http://www.pointclouds.org)

PCL is available in binary form for Windows platforms via the project's GitHub
releases page. `ifm3d` is tested against version v1.8.1 which can be
downloaded and installed via the following links.

Choose ONE of the following based on your target version of Visual Studio.
 * Visual Studio 2017/2019: [PCL-1.8.1-AllInOne-msvc2017-win64.exe](https://github.com/PointCloudLibrary/pcl/releases/download/pcl-1.8.1/PCL-1.8.1-AllInOne-msvc2017-win64.exe)

**NOTE**: Opt-in to installing the 3rd party dependencies of `PCL`. `ifm3d`
also takes a dependency on the `boost` library. For simplicity/compatibility,
this tutorial builds against the version of `boost` provided along with
`PCL`.

**NOTE**: It is possible to build an ifm3d variant which does not depend on the
`PCL` library. See [Appendix A](#appendex-a) below for instructions.

##### [OpenCV 3.4](https://opencv.org/)

OpenCV 3.4 is available in binary form for Windows platforms from the [OpenCV
Releases](https://opencv.org/releases/) page. `ifm3d` is tested against
v3.4.9, available from the following link:

* [OpenCV 3.4.9](https://sourceforge.net/projects/opencvlibrary/files/3.4.9/opencv-3.4.9-vc14_vc15.exe/download)

Download it and extract to a known location (this tutorial assumes a path of
`C:\opencv`.

#### Source Dependencies

`ifm3d` depends on several additional libraries (curl, xmlrpc-c, glog, and
gtest) which are not available as binary packages on Windows.

##### Automatically building source dependencies with ifm3d

ifm3d from version 0.90.4 onwards provides ```BUILD_IN_DEPS``` option to cmake configure command,
which fetches the required dependencies and builds them with ifm3d. After a successful first installation, the
user can disable BUILD_IN_DEPS option and can use the installed dependencies for future builds of the ifm3d.

Following instructions detail how to build ifm3d along with its dependencies.

⚠ The code on the branch {{ ifm3d_main_branch }} is updated nightly and contains the latest changes to the library. It is typically a work in progress.   
⚠ We recommend using tagged versions for your builds, to ensure consistency between builds. The latest tagged version can be found {{ '[here]({})'.format(ifm3d_latest_tag_url) }}.

Open Command Prompt and execute following instructions

```
#set the environment variables
set IFM3D_CMAKE_GENERATOR="Visual Studio 16 2019"
set IFM3D_BUILD_DIR=C:\ifm3d
set CONFIG=Release #set to Debug for debug binaries

#make the working dir
mkdir %IFM3D_BUILD_DIR%

# Clone the repository
cd %IFM3D_BUILD_DIR%
git clone https://github.com/ifm/ifm3d.git --branch o3r/main-next
cd %IFM3D_BUILD_DIR%\ifm3d

# Configure
mkdir build
cd build
cmake -G %IFM3D_CMAKE_GENERATOR% -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DCMAKE_PREFIX_PATH=%IFM3D_BUILD_DIR%\install -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install -DBUILD_IN_DEPS=ON ..

# Build ifm3d and dependencies
cmake --build . --config %CONFIG% --target ALL_BUILD

# install
cmake --build . --config %CONFIG% --target install
```
On successful execution of install step, user can disable the BUILD_IN_DEPS flag by appending
``` -DBUILD_IN_DEPS=OFF``` to cmake configure step, this will avoid building dependencies on every clean build.

***Note***: By default ```ifm3d::Image``` and ```ifm3d::opencv``` modules are disabled, to enabled these modules
use ```-DBUILD_MODULE_IMAGE=ON``` and ```-DBUILD_MODULE_OPENCV=ON``` respectively to cmake configure command.
Also append the opencv install binary path to -DCMAKE_PREFIX_PATH as shown in [Building ifm3d](Building-ifm3d) section

##### Manually building the source dependencies

The following instructions detail how to compile them from source for your target.

###### Environment Configuration

The following environment variables are used by this tutorial to make
customization simpler. Modify them as needed for your environment. You can
obtain a list of valid cmake generator strings via `cmake -h`. Again, `ifm3d`
supports version 2017 and newer.

Open Command Prompt and execute following instructions
```
set IFM3D_OPENCV_PATH=C:\opencv\build
set IFM3D_CMAKE_GENERATOR="Visual Studio 15 2017 Win64"
set IFM3D_BUILD_DIR=C:\ifm3d
set CONFIG=Release
```

Finally, create the working directory in which `ifm3d` and its dependencies will
be built:
```
mkdir %IFM3D_BUILD_DIR%
```

###### [curl](https://curl.haxx.se/)
```
cd %IFM3D_BUILD_DIR%
git clone --branch curl-7_47_1 https://github.com/curl/curl.git
cd %IFM3D_BUILD_DIR%\curl
mkdir build
cd build
cmake -G %IFM3D_CMAKE_GENERATOR% -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install ..
cmake --build . --clean-first --config %CONFIG% --target INSTALL
```

###### [xmlrpc-c](http://xmlrpc-c.sourceforge.net/)
```
cd %IFM3D_BUILD_DIR%
git clone --branch 1.33.14-cmake https://github.com/ifm/xmlrpc-c.git
cd %IFM3D_BUILD_DIR%\xmlrpc-c
mkdir build
cd build
cmake -G %IFM3D_CMAKE_GENERATOR% -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install ..
cmake --build . --clean-first --config %CONFIG% --target INSTALL
```

###### [glog](https://github.com/google/glog)
```
cd %IFM3D_BUILD_DIR%
git clone --branch v0.3.5 https://github.com/google/glog.git
cd %IFM3D_BUILD_DIR%\glog
mkdir build
cd build
cmake -G %IFM3D_CMAKE_GENERATOR% -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install ..
cmake --build . --clean-first --config %CONFIG% --target INSTALL
```

###### Building ifm3d
⚠ The code on the branch {{ ifm3d_main_branch }} is updated nightly and contains the latest changes to the library. It is typically a work in progress.   
⚠ We recommend using tagged versions for your builds, to ensure consistency between builds. The latest tagged version can be found {{ '[here]({})'.format(ifm3d_latest_tag_url) }}.

```
# Clone the repository
cd %IFM3D_BUILD_DIR%
git clone https://github.com/ifm/ifm3d.git
cd %IFM3D_BUILD_DIR%\ifm3d

# Configure
mkdir build
cd build
cmake -G %IFM3D_CMAKE_GENERATOR% -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DCMAKE_PREFIX_PATH=%IFM3D_BUILD_DIR%\install;%IFM3D_OPENCV_PATH% -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install ..

#install
cmake --build . --config %CONFIG% --target INSTALL
```

### Running the ifm3d test with device (optional)

ifm3d tests are based on [gtest](https://github.com/google/googletest.git). Use following instructions to build and run the tests

```
# clone 
cd %IFM3D_BUILD_DIR%
git clone --branch release-1.8.1 https://github.com/google/googletest.git

cd ifm3d
cd build 
cmake -G %IFM3D_CMAKE_GENERATOR% -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DBUILD_TESTS=ON -DGTEST_CMAKE_DIR=%IFM3D_BUILD_DIR%\googletest\googletest -Dgtest_force_shared_crt=TRUE -DCMAKE_PREFIX_PATH=%IFM3D_BUILD_DIR%\install;%IFM3D_OPENCV_PATH% -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install ..

#Tests
cmake --build . --config %CONFIG% --target check 
```

### Running the ifm3d command line tool
After Building `ifm3d`, the binary files will be installed at
``%IFM3D_BUILD_DIR%\install\bin``. To run the ifm3d tool you need to add this
directory to your path. You will also need to add the opencv directory to
your path.

If built targeting Visual Studio 2017/2019:
```
set PATH=%IFM3D_BUILD_DIR%\install\bin;%IFM3D_OPENCV_PATH%\x64\vc15\bin;%PATH%
```

After that you should be able to run the ifm3d tool
```
ifm3d
```

### Appendix A: Building without PCL
The `ifm3d` library offers an alternative image buffer implementation which
only depends on `OpenCV`, thus eliminating a dependency on `PCL`. In order to
build `ifm3d` without a dependency on `PCL`, the following modifications to
the instructions above are necessary.

#### Select the OpenCV Image Container

The `ifm3d::ImageBuffer` module (has dependency on PCL) must be disabled with
the flag `-DBUILD_MODULE_IMAGE=OFF` and the `ifm3d::OpenCV`Buffer module must
be enabled with the flag `-DBUILD_MODULE_OPENCV=ON`. The full `cmake`
configuration command is:

```
cmake -G %IFM3D_CMAKE_GENERATOR% -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DBUILD_SDK_PKG=ON -DGTEST_CMAKE_DIR=%IFM3D_BUILD_DIR%\googletest\googletest -Dgtest_force_shared_crt=TRUE -DCMAKE_PREFIX_PATH=%IFM3D_BUILD_DIR%\install;%IFM3D_OPENCV_PATH% -DBOOST_ROOT=%IFM3D_BOOST_ROOT% -DBoost_USE_STATIC_LIBS=ON -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install -DBUILD_MODULE_IMAGE=OFF -DBUILD_MODULE_OPENCV=ON ..
```