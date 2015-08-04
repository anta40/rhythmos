How to build Rhythmos from sources.

## Download ##
Use git to clone a copy of the source tree.
```
$ git clone http://code.google.com/p/rhythmos/
```
Go to the [RhythmOS source](http://code.google.com/p/rhythmos/source/checkout) page for more info.

## Configure ##
From the top level project directory execute the `configure` script.
```
$ ./configure
```
Install any missing programs that `configure` recommends.
## Build ##
After `configure` is run a `Makefile` is generated. From the top level project directory run `make'.
```
$ make
```

## Running ##
If you just want to run it and don't care how it does it then type:
```
$ make run
```

Rhythmos can be simulated with QEMU a couple of different ways:

**1.** Using QEMU as the bootloader. Pass QEMU the built kernel image `src/kernel.img` and the file system image `src/filesystem.img`.
```
$ qemu -kernel src/kernel.img -initrd src/filesystem.img
```

**2.** Build a bootable floppy image that uses grub as the bootloader. There is a one already created for you in the source tree. Just pass QEMU the floppy image: directly use the command below.
```
$ qemu -fda src/grub.img
```
The `make run` target also boots using a floppy image with grub as the bootlader, but it first creates a fresh image before booting.

To regenerate the `grub.img` because the ramdisk and/or the the kernel image was updated the `grub.img' make target will do this for you.
```
$ make grub.img
```
Then use method 2 to boot from it.

See documentation in the source for more specifics.