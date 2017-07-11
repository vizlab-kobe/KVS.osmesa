KVS.osmesa
===
KVS.osmesa is a off-screen rendering framework with OSMesa for KVS.

# Prerequisite
The following software packages are needed to compile KVS.osmesa.

## OSMesa
OSMesa must be installed in your computing environment before compiling KVS.osmesa.

- [OSMesa](https://www.mesa3d.org/osmesa.html)

#### Mac OS X
In case of Mac OS X, OSMesa will be installed by installing XQuartz. You can dowload it from the following URL and install in your environment.

- [XQuartz](https://www.xquartz.org)

The environment variable ```KVS_OSMESA_DIR``` should be set to an install directory of OSMesa as follows:

```
export KVS_OSMESA_DIR=/usr/X11
```

## OSMesa-enabled KVS
The source code of KVS can be downloaded via GitHub as follows:

```
$ git clone https://github.com/naohisas/KVS.git
```

The environment variable ```KVS_DIR``` should be set to an install directory of KVS. Here is an example of setting the variable. You can change the install directory to other directory, if you want.

```
export KVS_DIR=~/local/kvs
```

OSMesa-enabled KVS can be compiled as follows:

1. Change directory to KVS
```
$ cd KVS
```

2. Modify ```kvs.conf``` as follows
```
KVS_ENABLE_OPENGL     = 1
KVS_ENABLE_OSMESA     = 1
KVS_ENABLE_GLU        = 0
KVS_ENABLE_GLEW       = 0
KVS_ENABLE_OPENMP     = 0
KVS_ENABLE_DEPRECATED = 0

KVS_SUPPORT_CUDA      = 0
KVS_SUPPORT_GLUT      = 0
KVS_SUPPORT_OPENCV    = 0
KVS_SUPPORT_QT        = 0
KVS_SUPPORT_PYTHON    = 0
```

3. Build and install KVS
```
$ make
$ make install
```
