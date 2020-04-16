# mqtt
利用 Paho mqtt library 傳值

# C++ MQTT 程式說明

### 1. 建立環境

#### 如果沒有 build 所需的套件的話要先裝
Debian based system
```
sudo apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
sudo apt-get install libssl-dev
sudo apt-get install doxygen graphviz
sudo apt-get install libcppunit-dev
```

需要先 build Paho C library
```
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
git checkout v1.3.1

cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF
sudo cmake --build build/ --target install
sudo ldconfig
```

再 build C++ library
```
git clone https://github.com/eclipse/paho.mqtt.cpp
cd paho.mqtt.cpp
cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
sudo cmake --build build/ --target install
sudo ldconfig
```

Pull 我的程式碼
```
https://github.com/j12358962/mqtt.git
```

# 
### 2. 使用 g++ 來compile 並運行
```
g++ data_publish.cpp -o data_publish -l paho-mqttpp3 -l paho-mqtt3as

./data_publish
```

