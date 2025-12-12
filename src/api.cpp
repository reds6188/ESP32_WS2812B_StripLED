#include "api.h"

String api_template(void) {
	JsonDocument res;
	String msg;
	//-----------------------
	// YOUR CODE HERE
	//-----------------------
	serializeJson(res, msg);
	return msg;
}

String api_template(uint8_t * payload) {
	JsonDocument req, res;
	String msg;
	deserializeJson(req, (const char*)payload);
	//-----------------------
	// YOUR CODE HERE
	//-----------------------
	serializeJson(res, msg);
	return msg;
}

String api_status(void) {
	JsonDocument res;
	String msg, status;

	switch(StatoStrip) {
		case STRIP_CHRISTMAS: 	status = "christmas"; 	break;
		case STRIP_RAINBOW:		status = "rainbow";		break;
		case STRIP_WATER:		status = "water";		break;
		default:				status = "off";			break;
	}

	res["status"] = status;
	serializeJson(res, msg);
	return msg;	
}

String api_christmas(void) {
	JsonDocument res;
	String msg, status;

	//if(StatoStrip != STRIP_OFF) {
	if(StatoStrip == STRIP_CHRISTMAS) {
		status = "off";
		offStrip();
	}
	else {
		clearStrip();
		status = "christmas";
		StatoStrip = STRIP_CHRISTMAS;
		led.setBlink(C8_BLUE,C8_BLACK,500,500);
	}


	res["status"] = status;
	serializeJson(res, msg);
	return msg;	
}

String api_rainbow(void) {
	JsonDocument res;
	String msg, status;

	//if(StatoStrip != STRIP_OFF) {
	if(StatoStrip == STRIP_RAINBOW) {
		status = "off";
		offStrip();
	}
	else {
		clearStrip();
		status = "rainbow";
		StatoStrip = STRIP_RAINBOW;
		startRainbow();
		led.setBlink(C8_FUCHSIA,C8_BLACK,500,500);
	}

	res["status"] = status;
	serializeJson(res, msg);
	return msg;	
}

String api_water(void) {
	JsonDocument res;
	String msg, status;

	//if(StatoStrip != STRIP_OFF) {
	if(StatoStrip == STRIP_WATER) {
		status = "off";
		offStrip();
	}
	else {
		clearStrip();
		status = "water";
		StatoStrip = STRIP_WATER;
		led.setBlink(C8_CYAN,C8_BLACK,500,500);
	}

	res["status"] = status;
	serializeJson(res, msg);
	return msg;	
}

String api_test(void) {
	JsonDocument res;
	String msg, status;

	if(StatoStrip == STRIP_TEST) {
		status = "off";
		offStrip();
	}
	else {
		clearStrip();
		status = "test";
		StatoStrip = STRIP_TEST;
		startTest();
		led.setBlink(C8_YELLOW,C8_BLACK,500,500);
	}

	res["status"] = status;
	serializeJson(res, msg);
	return msg;	
}

String api_set_test(uint8_t * payload) {
	JsonDocument req, res;
	String msg;

	deserializeJson(req, (const char*)payload);
	Serial.println("Payload: ");
	Serial.println((const char*)payload);
	if(req["red"]["seed"].is<unsigned int>()) {
		test_seed.r = req["red"]["seed"];
	}
	if(req["green"]["seed"].is<unsigned int>()) {
		test_seed.g = req["green"]["seed"];
	}
	if(req["blue"]["seed"].is<unsigned int>()) {
		test_seed.b = req["blue"]["seed"];
	}
	if(req["red"]["step"].is<unsigned int>()) {
		test_step.r = req["red"]["step"];
	}
	if(req["green"]["step"].is<unsigned int>()) {
		test_step.g = req["green"]["step"];
	}
	if(req["blue"]["step"].is<unsigned int>()) {
		test_step.b = req["blue"]["step"];
	}
	StatoStrip = STRIP_TEST;
	startTest();

	serializeJson(res, msg);
	return msg;	
}

String api_get_num_led(void) {
	JsonDocument res;
	String msg, status;

	res["num"] = 5;//Strip.getLength();
	serializeJson(res, msg);
	return msg;	
}

String api_set_num_led(uint8_t * payload) {
	JsonDocument req, res;
	String msg;

	deserializeJson(req, (const char*)payload);
	Serial.println("Payload: ");
	Serial.println((const char*)payload);
	if(req["num"].is<unsigned int>()) {
		setNumLed(req["num"]);
	}

	serializeJson(res, msg);
	return msg;	
}