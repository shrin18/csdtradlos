# csdtradlos
Project 7:
Efficient Scheduling for Parallelized CDMA 
Transceiver with Software Defined Radio


![logo_wireless folks](https://gits-15.sys.kth.se/storage/user/9162/files/39c40400-fd4c-11e9-8284-4808746a504b)

### Manual Source Build
Complete build instructions are detailed in the 
[GNU Radio Build Guide](https://www.gnuradio.org/doc/doxygen/build_guide.html). 
Abbreviated instructions are duplicated below.

1. Ensure that you have satisfied the external dependencies, see 
[GNU Radio Dependencies](https://www.gnuradio.org/doc/doxygen/build_guide.html).

2. Before checking out the code ensure that you have access rights to the Repository and that you have added the SSHkey to your account

2. Checkout the latest code:
    ```
    $ git clone git@gits-15.sys.kth.se/shrinish/csdtradlos.git
    ```

3. Build with CMake:
    ```
    $ cd csdtradlos
    $ mkdir build
    $ cd build
    $ cmake [OPTIONS] ../
    $ make
    $ make test
    $ sudo make install
    ```
    Useful `[OPTIONS]` include setting the install prefix 
    `-DCMAKE_INSTALL_PREFIX=<directory to install to>` and the build type 
    `-DCMAKE_BUILD_TYPE=<type>`. Currently, GNU Radio has a `"Debug"` type 
    that builds with `-g -O2` which is useful for debugging the software, 
    and a `"Release"` type that builds with `-O3`, which is the default.
    
4.  Submodules can be pulled to the personal repo for easier installation.
