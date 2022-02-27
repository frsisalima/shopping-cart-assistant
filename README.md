##Car shopping assistant
Software to help blind people in supermarkets, this project works specifically on jetson nano

This project was build with:

* Jetson Inference to interact with TensorRT
* OpenCV
* C++ MJPEG pver HTTP Library - video streaming
* Pistache to build the Rest Api

#### Project structure:
* data:
  * images/ to test
  * videos/ to test
  * networks/ directory where the models are stored 
  * config.json config file to the app
  * detectnet.service linux service to run as service in system
* src:
    * detectnet/
      * headers/
      * sources/
      * detectnet.cpp main file
      * CMakeLists.txt cmake config
      
#### compile and run:
```shell script
mkdir build && cd build
```
```shell script
cmake .. && make
```
```shell script
./detectnet --config=../data/config.json
```