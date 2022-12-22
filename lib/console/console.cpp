#include "console.h"

Console::Console(void) {
	Serial.begin(115200);
	Serial.println();
	this->max_tag_length = TAG_MAX_LENGTH;
}

void Console::setTagLength(uint8_t length) {
	this->max_tag_length = length;
}

void Console::line(style_t style, uint8_t length, bool newLine) {
	String line_str;
	String chr;

	switch(style)
	{
		case DOTTED:		chr = "."; break;
		case DASHED:		chr = "-"; break;
		case DOUBLE_DASHED:	chr = "="; break;
		case LINE:			chr = "_"; break;
		case WAVE:			chr = "~"; break;
		case HASH:			chr = "#"; break;
		case ASTERISK:		chr = "*"; break;
		default:			chr = "."; break;
	}

	for(uint8_t i=0 ; i<length ; i++)
		line_str += chr;

	if(newLine)
		Serial.println(line_str);
	else
		Serial.print(line_str);
}

void Console::header(String title, style_t style, uint8_t length) {
	uint8_t fill = (length - title.length() - 6);
	uint8_t side = fill / 2;
	line(style, length, true);
	line(style, side, false);
	Serial.print("   " + title + "   ");
	if(length%2)	// Odd length
		line(style, (title.length()%2 ? side : side+1), true);
	else			// Even length
		line(style, (title.length()%2 ? side+1 : side), true);
	line(style, length, true);
}

void Console::log(String tag, String message) {
	uint8_t num_spaces = this->max_tag_length - tag.length();
	String pad_str;
	for(uint8_t i=0 ; i<num_spaces ; i++)
		pad_str += " ";
	Serial.print("[ " + tag + pad_str + " ] ");
	Serial.println(message);
}

Console console;