# C++ MQTT 程式環境建立與執行

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
g++ main.cpp -o main -l paho-mqttpp3 -l paho-mqtt3as

./main
```

# C++ MQTT API說明


### 1. int MQTT_nodeInit(string deviceName)

輸入WebAccess中的設備名，若是這個設備中沒有測點，則建立40個測點，若已存在測點，則覆蓋，其中為8個channel， 每個channel中有5個專屬的測點。
測點的意思分別代表:
```
1. Channel_x_non-defective: Channel_通道號碼x_良品數量
2. Channel_x_defective: Channel_通道號碼x_不良品數量
3. Channel_x_last_defective_five: Channel_通道號碼x_最後5模不良品
4. Mold_number: 8個channel的模次號碼
note1: 最後5模不良品為一個不良品號碼的陣列，如[2,56,78,100,123]
note2: 會建立位址如 ITRI_scada_{deviceName}_Channel_x_non-defective的測點
```

### 2. string MQTT_makeMessage(string measuringPoint, string mesuringPointValue)

輸入(要更新的測點名稱, 要更新的測點數值)，會建立出符合WebAccess傳輸協議的部分字串。

### 3. int MQTT_dataPublish(string addr, string topic, string username, string password, string deveiceName, string msg)

輸入(MQTT broker的位址, MQTT的topic, 登入MQTT的username, 登入MQTT的password, WebAccess的設備名, 由 MQTT_makeMessage做好的字串)後，會將測點的資料更新。
