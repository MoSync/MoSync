#include <maapi.h>
#include <mastring.h>
#include <conprint.h>

extern "C" int MAMain()
{
	//Some text to transfer
	const char* text = "Manufacturers of ball-tracking systems say England's disallowed World Cup goal underlines the need for goal-line technology (GLT).";

	lprintfln("Length: %lu", strlen(text));

	//A char array so we can test what is being transferred
	char temp[strlen(text) + 1];

	MAHandle source = maCreatePlaceholder();

	//Create the data space
	maCreateData(source, 2014);

	//Read the text into the source
	maWriteData(source, text, 0, strlen(text));

	//Read the data back out as a test
	maReadData(source, &temp, 0, strlen(text));

	lprintfln("Source: %s", temp);

	//Create a target to copy the data to
	MAHandle target = maCreatePlaceholder();
	maCreateData(target, 1024);

	//Copy the data
	MACopyData* params = new MACopyData();

	params->dst = target;
	params->dstOffset = 0;
	params->src = source;
	params->srcOffset = 0;
	params->size = strlen(text);

	//Perform the copy
	maCopyData(params);

	//Clear any old data. Also set up a null terminator.
	memset(&temp, 0, sizeof(temp));

	//Read the target into the temp area now
	maReadData(target, &temp, 0, strlen(text));

	//Write out the copied data for comparison
	lprintfln("Target: %s", temp);

	return 0;
}; 
