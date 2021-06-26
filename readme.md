# Scoped-singleton 

This repository embeds the 'scoped-singleton' pattern. An interesting and elegant pattern that leverages 
on [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#:~:text=In%20RAII%2C%20holding%20a%20resource,finalization)%2C%20by%20the%20destructor.) and
[weak pointers](https://en.cppreference.com/w/cpp/memory/weak_ptr). 


## Usecase demo
In this repository we showcase the pattern as an application instrumentation for data monitoring. 
Some benefits the pattern carries on a this use-case are:
* reliability: through the use of RAII, delivery of the log/monitoring data to the log-server
  is guaranteed even in case flow control is unexpected (e.g.: exceptions).
* efficiency: the scope-oriented nature of this pattern allows for good fine-tuning on the granularity
  of the log/monitroing client-server communications. In other words, log data can be sent once per transaction to the 
  server regardless on how it's used across the whole transaction callback.
* testability: the pattern eases the testability aspect of the monitoring/log data that you create in your application.
  This is great to maintain stability of dashboards/alerts and 3rd party system that leverages on the 
  schema of the data that you sent to them.
* convenience of use: shares the same ease of use of the singleton pattern from the client usage point of view. 


## Building, running and testing

As this is a demo repository, the only interesting thing to be run are the unit-tests 
which were built also to support a presentation around the pattern.

You will be able to build this repository from whichever machine where you have access to:
1. [docker](https://www.docker.com/)
2. [Make](https://www.gnu.org/software/make/#:~:text=GNU%20Make%20is%20a%20tool,compute%20it%20from%20other%20files.) 

All operations are wrapped in the `Makefile` that you can find in the repository root folder.
This Makefile leverages on docker to compile and run the tests, this way you don't need to 
install specific compiler versions or cmake itself.
Below you have some operations supported by the Makefile (for more, issue `make help` command);

* Run everything (container build, application build and unit-tests)
   ```shell
   $ make 
   ```

### Finer grained operations:
   
1. Build your docker image:
    ```shell
    $ make dockerimgbuild
    ```
2. Run cmake to setup build:
    ```shell
    $ make cmakebuild
    ```
3. Build the binaries
    ```shell
    $  make build
    ```
4. Run unit-tests:
   ```shell
   $ make test
   ```
4. Unit-tests with arguments
   ```shell
   $ make test TSTARGS="--gtest_filter=YourFilter"
   ```

### Presentation

This repository embeds the presentation source code of the Scoped-Singleton pattern. 
All information on how to compile the information can be found in the `pres` folder.

### Github actions

This repository leverages on [github action](https://github.com/features/actions). 
You will be ble to see two different workflows:

1. `.github/workflows/build-and-test.yml` which handles building and unit-testing the code repository
2. `.github/workflows/presentation.yml` which builds the presentation and archives it. If you don't 
    want to build the presentation on your own but only have a look at the resulting `.pdf` you can 
    download the archive from the latest [presentation workflow](https://github.com/rbroggi/scoped-singleton/actions/workflows/presentation.yml).

### Shortcomings 
1. mind your scope: long-living reference
2. mind your scope: no anchor
3. thead-safety needs to be properly studied if you have a read-write API 

# References and acknowledgements



