#include "data_publish.cpp"

const string ADDRESS { "tcp://140.116.39.171:1883" };

const string CFGTOPIC { "iot-2/evt/wacfg/fmt/ITRI_scada" };
const string DATATOPIC { "iot-2/evt/wadata/fmt/ITRI_scada" };

const string USERNAME {"admin"};
const string PASSWORD {"admin"};


int main(){

  // 設備名稱
  const string name(getenv("USER"));
  const string deviceName = "device-" + name;

  // 初始化
  MQTT_nodeInit(deviceName);

  // 欲更新的測點名稱
  // string measurePoint1 = "Channel_1_non-defective";
  // string measurePoint2 = "Channel_2_non-defective";
  //
  // // 要傳給測點的內容
  // string message1 =  "\"5\"";
  // string message2 =  "\"3\"";
  //
  // // 將要傳的訊息做成符合WebAccess格式的字串
  // string msgs =  MQTT_makeMessage(measurePoint1, message1);
  // msgs += MQTT_makeMessage(measurePoint2, message2);
  //
  // // 將剛剛做好的字串傳送
  // MQTT_dataPublish(ADDRESS, DATATOPIC, USERNAME, PASSWORD, deviceName, msgs);


  return 0;
}
