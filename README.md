# KVS.osmesa
KVS.osmesa is a off-screen rendering framework with OSMesa for KVS.

## Prerequisite
The following software packages are needed to compile KVS.osmesa.

### OSMesa
OSMesa must be installed in your computing environment before compiling KVS.osmesa.

- [OSMesa](https://www.mesa3d.org/osmesa.html)

#### Mac OS X
In case of Mac OS X, OSMesa will be installed by installing XQuartz. You can dowload it from the following URL and install in your environment.

- [XQuartz](https://www.xquartz.org)

The environment variable ```KVS_OSMESA_DIR``` should be set to an install directory of OSMesa as follows:

```
export KVS_OSMESA_DIR=/usr/X11
```

### OSMesa-enabled KVS
OSMesa-enabled KVS can be compiled as follows:
