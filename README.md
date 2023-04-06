# BPoint

B-Point is a stand-alone pointing library written in C that uses spherical trigonometry instead of quaternions.


BPoint was created in the context the Cosmology Large Angular Scale Surveyor. The main purpose was to get a "quaternion-free"
(and properly documented!) telescope pointing library with the ability to handle boresight rotations, as well as "goodies" 
such as routines to calculate the "re-centering" focal-plane coordinates to Solar System bodies.

# Authorship

The author of this code is Michael "Mike" K. Brewer,

Me (Pedro Fluxa), I am a mere maintainer of this repository. 

OK fine, I also wrote a neglible portion of the library and the CMake installation files, _big deal_. 

I claim no authorship of this library anyways.

**Thanks Mike!**

# Dependencies

BPoint depends on SOFA (Standards Of Fundamental Astronomy) library, which can be downloaded from

https://www.iausofa.org/current_C.html#Downloads

After decompressing the tarbal, you will have to change the makefile to create a shared library instead of a static one.
The modification will also install SOFA libraries into `/usr` instead of your home folder.
The above can be accomplished by running the following commands in the `src/c` folder of SOFA

```
cp makefile makefile.backup
sed s%"INSTALL_DIR = \$(HOME)"%"INSTALL_DIR = /usr"%g makefile > _makefile1
sed s%"CFLAGF = -c -pedantic -Wall -O"%"CFLAGF = -c -pedantic -Wall -O -fPIC"%g _makefile1 > _makefile2
sed s%"CFLAGX = -pedantic -Wall -O"%"CFLAGX = -pedantic -Wall -O -fPIC"%g _makefile2 > _makefile3
sed s%"SOFA_LIB_NAME = libsofa_c.a"%"SOFA_LIB_NAME = libsofa_c.so"%g _makefile3 > _makefile4
cp _makefile4 makefile
rm _makefile*
make
# sudo is required because installation is performed in /usr/
sudo make install
```

# Compilation

BPoint uses `cmake` to perform compilation and installation. In the root folder of the repo, create a folder called `build`,
the `cd` into that folder and run

```
cmake -DCMAKE_INSTALL_PREFIX=../ ../
make 
make install
```

This will install the executables files under `bin/` and install libraries and headers into `lib/` and `include/`. If you
already know where to install the library, you can specify the path by changing the `../` next to `-DCMAKE_INSTALL_PREFIX`
to the desired system path. 

# Creating the ephemeris file

BPoint requires ephemeris data to perform some calculations, like computing coordinates of a focal plane with respect
to a Solar System body (i.e., the Moon). 

The ephemeris data can be downloaded from https://ssd.jpl.nasa.gov/ftp/eph/planets/ascii/de405/ The files are split
into intervals of 20 years; for example, if you are going to use BPoint for computations between the year 2000 and 2040,
you would need to download (spoiler, these are included in the repo!)

header.405
ascp2000.405
ascp2020.405
ascp2040.405

These files will then be used by `asc2bin.x` to create a binary file that will work as a lookup table, which is _way faster_
than using ASCII files. To create the binary look-up table, run

`./bin/asc2bin.x <( cat data/header.405 data/ascp2000.405 data/ascp2020.405 data/ascp2040.405 )`

This will create the file "DEc405"; you will need to place it under `data/` 

Please note that if you installed BPoint in the "standard" folders, you will need to add the path to your `LD_LIBRARY_PATH`
environment variable. To do so, run the following command 

`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<path to repo folder>/lib`

