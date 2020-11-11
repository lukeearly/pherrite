# Pherrite

## Build process

Download the [ANTLR jar file](https://www.antlr.org/download/antlr-4.8-complete.jar) and place it in the subdirectory `external` of the project root.

Make sure you have pkg-config and libuuid installed on your system, then run in the project root directory:

``` sh
$ cmake .
$ make
```

After this, the `pherrite` executable should be available for use.
