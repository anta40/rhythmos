# Introduction #
If your native architecture is x86 then you will most likely already have a toolchain that will work for RhythmOS. If your architecture is something else you will have to build an toolchain to cross compile to x86.

## Find your architecture ##
You can find your architecture in Linux from one of the following commands:

```
$ arch
x86_64
```

```
$ lscpu | grep Arch
Architecture:          x86_64
```

```
$ uname -m
x86_64
```


# Set up build environment #

Export build variables
```
$ export PREFIX=/usr/local/cross
$ export TARGET=i386-elf
```

Create build directories for gcc binutils and newlibc
```
$ mkdir -p build/{gcc,binutils,newlib}
$ sudo mkdir /usr/local/cross
```

Download and extract Binutils and gcc and newlibc
```
$ wget 'http://ftp.gnu.org/gnu/binutils/binutils-2.21.1a.tar.bz2'
$ wget 'http://ftp.gnu.org/gnu/gcc/gcc-4.6.2/gcc-4.6.2.tar.bz2'
$ wget 'ftp://sources.redhat.com/pub/newlib/newlib-1.20.0.tar.gz'
$ tar xjvf binutils-2.21.1a.tar.bz2 
$ tar xjvf gcc-4.6.2.tar.bz2 
$ tar xzvf newlib-1.20.0.tar.gz
```

Download gmp mpfr and mpc required by gcc
```
$ wget 'ftp://ftp.gmplib.org/pub/gmp-5.0.2/gmp-5.0.2.tar.bz2'
$ wget 'http://www.mpfr.org/mpfr-current/mpfr-3.1.0.tar.bz2'
$ wget 'http://www.multiprecision.org/mpc/download/mpc-0.9.tar.gz'
```

Extract gmp mpfr and mpc into the gcc directory so that gcc will build them automatically
```
$ cd gcc-4.6.2
$ tar xjvf ../gmp-5.0.2.tar.bz2 && mv gmp-5.0.2/ gmp
$ tar xjvf ../mpfr-3.1.0.tar.bz2  && mv mpfr-3.1.0/ mpfr
$ tar xzvf ../mpc-0.9.tar.gz  && mv mpc-0.9/ mpc
$ cd ..
```

# Build #
## Binutils ##
configure binutils
```
$ cd build/binutils/
$ ../../binutils-2.21.1/configure --target=$TARGET --prefix=$PREFIX --disable-nls
```

Make and install binutils
```
$ make all
$ sudo make install
$ cd ../..
```

## GCC ##
Setup environment
```
$ export PATH=$PATH:$PREFIX/bin
```

configure gcc
```
$ cd build/gcc
$ ../../gcc-4.6.2/configure  --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c --without-headers
```

Make and install gcc
```
$ make all-gcc 
$ sudo make install-gcc
$ cd ../..
```

## Newlib ##

configure newlib
```
$ cd build/newlib
$ ../../newlib-1.20.0/configure --target=$TARGET --prefix=$PREFIX
```

Make and install newlib
```
$ make all
$ sudo make install
```

Run command below if `sudo make install` cant find utils in `/usr/cross/bin`
```
$ sudo ln -s /usr/local/cross/bin/i386-elf-* /usr/local/bin
$ sudo make install
```