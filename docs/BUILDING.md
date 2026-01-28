The following is a non-exhaustive list of build arguments that can be passed to the `cmake` command-line tool in order to enable or disable options in build time:

* **-DCMAKE_BUILD_TYPE=[Release|Debug|RelWithDebInfo|MinSizeRel]** <br>
	An optional CMake variable for setting the build type. If not set, defaults to "Release" if `-DDEV=ON` is **not** used, and "Debug" if `-DDEV=ON` is used. See `CMAKE_BUILD_TYPE` in CMake Documentation for more information.

* **-DPREFER_BUNDLED_LIBS=[ON|OFF]** <br>
	Whether to prefer bundled libraries over system libraries. Setting to ON will make DDNet use third party libraries available in the `ddnet-libs` folder, which is the git-submodule target of the [ddnet-libs](https://github.com/ddnet/ddnet-libs) repository mentioned above -- Useful if you do not have those libraries installed and want to avoid building them. If set to OFF, will only use bundled libraries when system libraries are not found. Default value is OFF.

* **-DWEBSOCKETS=[ON|OFF]** <br>
	Whether to enable WebSocket support for server. Setting to ON requires the `libwebsockets-dev` library installed. Default value is OFF.

* **-DMYSQL=[ON|OFF]** <br>
	Whether to enable MySQL/MariaDB support for server. Requires at least MySQL 8.0 or MariaDB 10.2. Setting to ON requires the `libmariadb-dev-compat` library installed, which are also provided as bundled libraries for the common platforms. Default value is OFF.

	Note that the bundled MySQL libraries might not work properly on your system. If you run into connection problems with the MySQL server, for example that it connects as root while you chose another user, make sure to install your system libraries for the MySQL client. Make sure that the CMake configuration summary says that it found MySQL libs that were not bundled (no "using bundled libs").

* **-DTEST_MYSQL=[ON|OFF]** <br>
	Whether to test MySQL/MariaDB support in GTest based tests. Default value is OFF.

	Note that this requires a running MySQL/MariaDB database on localhost with this setup:

```sql
CREATE DATABASE ddnet;
CREATE USER 'ddnet'@'localhost' IDENTIFIED BY 'thebestpassword';
GRANT ALL PRIVILEGES ON ddnet.* TO 'ddnet'@'localhost';
FLUSH PRIVILEGES;
```

* **-DAUTOUPDATE=[ON|OFF]** <br>
	Whether to enable the autoupdater. Packagers may want to disable this for their packages. Default value is ON for Windows and Linux.

* **-DCLIENT=[ON|OFF]** <br>
	Whether to enable client compilation. If set to OFF, DDNet will not depend on Curl, Freetype, Ogg, Opus, Opusfile, and SDL2. Default value is ON.

* **-DVIDEORECORDER=[ON|OFF]** <br>
	Whether to add video recording support using FFmpeg to the client. Default value is ON.

* **-DDOWNLOAD_GTEST=[ON|OFF]** <br>
	Whether to download and compile GTest. Useful if GTest is not installed and, for Linux users, there is no suitable package providing it. Default value is OFF.

* **-DDEV=[ON|OFF]** <br>
	Whether to optimize for development, speeding up the compilation process a little. If enabled, don't generate stuff necessary for packaging. Setting to ON will set CMAKE\_BUILD\_TYPE to Debug by default. Default value is OFF.

* **-DUPNP=[ON|OFF]** <br>
	Whether to enable UPnP support for the server.
	You need to install `libminiupnpc-dev` on Debian, `miniupnpc` on Arch Linux.
	Default value is OFF.

* **-DVULKAN=[ON|OFF]** <br>
	Whether to enable the Vulkan graphics backend.
	You need to install the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) and set the `VULKAN_SDK` environment flag accordingly.
	Default value is ON for Linux, and OFF for Windows and macOS.

* **-GNinja** <br>
	Use the Ninja build system instead of Make. This automatically parallelizes the build and is generally faster. Compile with `ninja` instead of `make`. Install Ninja with `sudo apt install ninja-build` on Debian, `sudo pacman -S --needed ninja` on Arch Linux.

* **-DCMAKE_CXX_LINK_FLAGS=[FLAGS]** <br>
	Custom flags to set for compiler when linking.

* **-DEXCEPTION_HANDLING=[ON|OFF]** <br>
	Enable exception handling (only works with Windows as of now, uses DrMingw there). Default value is OFF.

* **-DIPO=[ON|OFF]** <br>
	Enable interprocedural optimizations, also known as Link Time Optimization (LTO). Default value is OFF.

* **-DFUSE_LD=[OFF|LINKER]** <br>
	Linker to use. Default value is OFF to try mold, lld, gold.

* **-DSECURITY_COMPILER_FLAGS=[ON|OFF]** <br>
	Whether to set security-relevant compiler flags like `-D_FORTIFY_SOURCE=2` and `-fstack-protector-all`. Default Value is ON.

* **-DMACOS_CODESIGN=[ON|OFF]** <br>
	Whether to sign code when building the DMG on macOS.


## Cross-compiling on Linux to Windows x86/x86\_64

Install MinGW cross-compilers of the form `i686-w64-mingw32-gcc` (32 bit) or
`x86_64-w64-mingw32-gcc` (64 bit). This is probably the hard part. ;)

Then add `-DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/mingw64.toolchain` to the
**initial** CMake command line.

## Cross-compiling on Linux/Windows to Android

Cross-compilation to Android is explained in a [separate guide](https://github.com/ddnet/ddnet/blob/master/scripts/android/README.md).

## Cross-compiling on Linux to WebAssembly via Emscripten

Install Emscripten cross-compilers on a modern linux distro. Follow the [instructions to install the emsdk](https://emscripten.org/docs/getting_started/downloads.html). Installing Emscripten from the package manager (e.g. with `sudo apt install emscripten`) may not work, as building some libraries requires a more recent version of Emscripten than available via package manager.

If you need to compile the ddnet-libs for WebAssembly, simply call

```sh
scripts/compile_libs/gen_libs.sh build-webasm-libs webasm
```

from the project's source directory. It will automatically create a directory called `ddnet-libs` in your build directory.
You can then merge this directory with the one in the ddnet source directory:

```sh
find ddnet-libs -type d -name webasm -exec rm -r {} + -prune
cp -r build-webasm-libs/ddnet-libs/. ddnet-libs/
```

Note that using Rust versions 1.90.0 - 1.93.0 does not work. Either use an older or newer version of Rust.
Run `rustup target add wasm32-unknown-emscripten` to install the WASM target for compiling Rust.

Create a new directory to build the client in.
Then run `emcmake cmake .. -G "Unix Makefiles" -DVIDEORECORDER=OFF -DVULKAN=OFF -DSERVER=OFF -DTOOLS=OFF -DPREFER_BUNDLED_LIBS=ON` in your build directory to configure followed by `cmake --build . -j8` to build.
For testing it is highly recommended to build in debug mode by also passing the argument `-DCMAKE_BUILD_TYPE=Debug` when invoking `emcmake cmake`, as this speeds up the build process and adds debug information as well as additional checks.
Note that using the Ninja build system with Emscripten is not currently possible due to [CMake issue 16395](https://gitlab.kitware.com/cmake/cmake/-/issues/16395).

To test the compiled code locally, just use `emrun --browser firefox DDNet.html`.

To host the compiled .html file copy all `.data`, `.html`, `.js`, `.wasm` files to the web server.
See `other/emscripten/minimal.html` for a minimal HTML example. You can also run `other/emscripten/server.py` to host a minimal server for testing using Python without needing to install Emscripten.

Enable cross-origin policies when using a proper web server. Example for apache2 on Debian-based distros:

```sh
sudo a2enmod header

# edit the apache2 config to allow .htaccess files
sudo nano /etc/apache2/apache2.conf

# set AllowOverride to All for your directory
# then create a .htaccess file on the web server (where the .html is)
# and add these lines
Header add Cross-Origin-Embedder-Policy "require-corp"
Header add Cross-Origin-Opener-Policy "same-origin"

# now restart apache2
sudo service apache2 restart
```

## Cross-compiling on Linux to macOS

Install [osxcross](https://github.com/tpoechtrager/osxcross), then add
`-DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/darwin.toolchain` and
`-DCMAKE_OSX_SYSROOT=/path/to/osxcross/target/SDK/MacOSX10.11.sdk/` to the
**initial** CMake command line.

Install `dmg` and `hfsplus` from
[libdmg-hfsplus](https://github.com/mozilla/libdmg-hfsplus) and `newfs_hfs`
from
[diskdev\_cmds](http://pkgs.fedoraproject.org/repo/pkgs/hfsplus-tools/diskdev_cmds-540.1.linux3.tar.gz/0435afc389b919027b69616ad1b05709/diskdev_cmds-540.1.linux3.tar.gz)
to unlock the `package_dmg` target that outputs a macOS disk image.

