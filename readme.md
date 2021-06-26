### TODO 
1. check with Daniel Parker if I can make some marketing of his library
1. check with Jetbrains sponsorship for the pres

### Building with Docker 
1. Build your docker image:
    ```shell
    $ docker build -t cmake-image .
    ```
2. Run cmake to setup build:
    ```shell
    $ docker run --rm -v `pwd`:/app --user `id -u`:`id -g` cmake-image cmake -B./build
    ```
3. Run make to build:
    ```shell
    $  docker run --rm -v `pwd`:/app --user `id -u`:`id -g` cmake-image make -C build
    ```
4. Run unit-tests:
   ```shell
   $ docker run --rm -v `pwd`:/app --user `id -u`:`id -g` cmake-image ./build/tests/ScopedSingleton_tst
   ```

### Context - structured logging in transaction info

### Convenience of use 

### Resilient to different flows controls (send log data for early-terminated methods)

### One single network rpc per transaction - network optimization

### Shortcomings 
1. mind your scope: long-living reference
2. mind your scope: no anchor
3. thead-safety needs to be properly studied if you have a read-write API 

# References and acknowledgements

* [jsoncons](https://github.com/danielaparker/jsoncons)
