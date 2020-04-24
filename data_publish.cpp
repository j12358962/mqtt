#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdlib.h>
#include <bits/stdc++.h>
#include "mqtt/async_client.h"


using namespace std;
using namespace std::chrono;

int MQTT_nodeInit(string);
string MQTT_makeMessage(string, string);
int MQTT_dataPublish(string, string, string, string, string, string);

const string DFLT_ADDRESS { "tcp://140.116.39.171:1883" };

const string cfgTOPIC { "iot-2/evt/wacfg/fmt/ITRI_scada" };
const string dataTOPIC { "iot-2/evt/wadata/fmt/ITRI_scada" };
const int	 QOS = 1;

//Set the frequency of sending messages
const auto PERIOD = milliseconds(1000);

const int MAX_BUFFERED_MSGS = 300;	// 120 * 5sec => 10min off-line buffering

const string PERSIST_DIR { "data-persist" };

const string username {"admin"};
const string password {"admin"};

const string name(getenv("USER"));

int MQTT_nodeInit(string stringDeviceName){
	string address = DFLT_ADDRESS;
	mqtt::async_client cli(address, "", MAX_BUFFERED_MSGS, PERSIST_DIR);

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
	connOpts.set_clean_session(true);
	connOpts.set_automatic_reconnect(true);
	connOpts.set_user_name(username);
	connOpts.set_password(password);
	// Create a topic object. This is a conventience since we will
	// repeatedly publish messages with the same parameters.
	mqtt::topic top(cli, cfgTOPIC, QOS, true);


	try {
		// Connect to the MQTT broker
		// cout << "Connecting to server '" << address << "'..." << flush;
		cli.connect(connOpts)->wait();
		// cout << "OK\n" << endl;

		char tmbuf[32];
		unsigned nsample = 0;

		// The time at which to reads the next sample, starting now
		auto tm = steady_clock::now();

			// Pace the samples to the desired rate
		this_thread::sleep_until(tm);

		// Get a timestamp and format as a string
		time_t t = system_clock::to_time_t(system_clock::now());
		strftime(tmbuf, sizeof(tmbuf), "%FT%TZ", localtime(&t));
		string stringInitString = string("{\"d\": {\"" + stringDeviceName +"\": {\"TID\": 1,\"Hbt\": 60,\"BID\": \" \",\"UTg\":{");
		// Create the payload as a text CSV string

			//format: {status: boolean, mold_number: int, good: int, defective: int
		for (int i = 1; i <= 8; ++i)
		{
			stringInitString +=  string("\"Mold_number\": {\"TID\": 3,\"Dsc\": \"模次號碼\",") +
							"\"Ary\": 0,\"RO\": 0,\"SH\": 60,\"SL\": 0,\"Alm\": 0,\"EU\": \"text\", \"TypeWA\": \"9\",\"Log\": 1" +
	            "},\"Channel_" + to_string(i) + "_non-defective\": {\"TID\": 3,\"Dsc\": \"良品數量\",\"Ary\": 0,\"RO\": 0,\"SH\": 60,\"SL\": 0,\"Alm\": 0," +
	            "\"EU\": \"text\",\"TypeWA\": \"9\",\"Log\": 1},\"Channel_" + to_string(i) +"_defective\": {\"TID\": 3,\"Dsc\": \"不良品數量\",\"Ary\": 0," +
	            "\"RO\": 0,\"SH\": 60,\"SL\": 0,\"Alm\": 0,\"EU\": \"text\",\"TypeWA\": \"9\",\"Log\": 1}," +
	            "\"Channel_" + to_string(i) + "_last_defective_five\": {\"TID\": 3,\"Dsc\": \"最後5模不良品\",\"Ary\": 0,\"RO\": 0,\"SH\": 60,\"SL\": 0,\"Alm\": 0," +
	            "\"EU\": \"text\",\"TypeWA\": \"9\",\"Log\": 1},";
 		}

		stringInitString = stringInitString.substr(0, stringInitString.size()-1);
		stringInitString += "},\"DTg\": null,\"Dsc\":\"\"}},\"ts\":\"" + string(tmbuf) + "\"}";
		cout<< stringInitString <<endl;

			// Publish to the topic
		top.publish(std::move(stringInitString));

		tm += PERIOD;

		cout << "OK!" << '\n';
		// Disconnect
		cout << "\nDisconnecting..." << flush;
		cli.disconnect()->wait();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}

string MQTT_makeMessage(string stringMeasuringPoint, string stringMeasuringPointValue){
	char tmbuf[32];
	unsigned nsample = 0;
	// The time at which to reads the next sample, starting now
	auto tm = steady_clock::now();

		// Pace the samples to the desired rate
	this_thread::sleep_until(tm);

		// Get a timestamp and format as a string
	time_t t = system_clock::to_time_t(system_clock::now());
	strftime(tmbuf, sizeof(tmbuf), "%FT%TZ", localtime(&t));

	string stringPayload;

	stringPayload =  stringPayload + "\"" + stringMeasuringPoint + "\"";
	stringPayload =  stringPayload + ":"+ stringMeasuringPointValue + ",";

  return stringPayload;
}

int MQTT_dataPublish(string stringADDR, string stringTopic, string stringUsername, string stringPassword, string stringDeviceName, string stringMessage){
	cout<<PERSIST_DIR<<endl;
	mqtt::async_client cli(stringADDR, "", MAX_BUFFERED_MSGS, PERSIST_DIR);

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
	connOpts.set_clean_session(true);
	connOpts.set_automatic_reconnect(true);
	connOpts.set_user_name(stringUsername);
	connOpts.set_password(stringPassword);
	// Create a topic object. This is a conventience since we will
	// repeatedly publish messages with the same parameters.
	mqtt::topic top(cli, stringTopic, QOS, true);

	stringMessage = stringMessage.substr(0, stringMessage.size()-1);
	string stringPayload = "{\"d\":{\"" + stringDeviceName + "\":{\"Val\":{";
	try {
		// Connect to the MQTT broker
		cout << "Connecting to server '" << stringADDR << "'..." << flush;
		cli.connect(connOpts)->wait();
		cout << "OK\n" << endl;

		char tmbuf[32];
		unsigned nsample = 0;

		// The time at which to reads the next sample, starting now
		auto tm = steady_clock::now();

			// Pace the samples to the desired rate
		this_thread::sleep_until(tm);

			// Get a timestamp and format as a string
		time_t t = system_clock::to_time_t(system_clock::now());
		strftime(tmbuf, sizeof(tmbuf), "%FT%TZ", localtime(&t));

		// Create the payload as a text CSV string
		// Publish to the topic
		stringPayload += stringMessage;
		string stringTimestamp = "}}}, \"ts\":\"" + string(tmbuf) + "\"}";
		stringPayload += stringTimestamp;

		cout << stringPayload << endl;

		top.publish(std::move(stringPayload));

		tm += PERIOD;


		// Disconnect
		cout << "\nDisconnecting..." << flush;
		cli.disconnect()->wait();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}
