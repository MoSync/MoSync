#include "sax_parser.h"
#include <maassert.h>
#include "MAHeaders.h"

static char tempString[1024];
const char* getString(FAXPP_Text *text) {
	memcpy(tempString, text->ptr, text->len);
	tempString[text->len] = 0;
	return tempString;
}

void StartDocumentCallback() {
	printf("start doc");
}

void EndDocumentCallback() {
	printf("end doc");
}

void StartElementCallback(const FAXPP_Event* event) {
	printf("start element: %s", getString(&event->name));
}

void EndElementCallback(const FAXPP_Event* event) {
	printf("end element: %s", getString(&event->name));
}

void CDataCallback(const FAXPP_Event* event) {
}

static const char xmlTest[] =
"<note>\n"
"<to>Tove</to>\n"
"<from>Jani</from>\n"
"<heading>Reminder</heading>\n"
"<body>Don't forget me this weekend!</body>\n"
"</note>\n";

int MAMain()
{

	FAXPP_SAX_Parser *parser = FAXPP_SAX_create_parser(StartDocumentCallback, EndDocumentCallback, StartElementCallback, EndElementCallback, CDataCallback);
	if(parser == NULL) return 1;
	FAXPP_Error err;

	#define BUFFER_SIZE 256
	char buffer[BUFFER_SIZE];
	int offset = 0;
	int fileSize = maGetDataSize(R_XML);
	BOOL done = FALSE;
	while(offset<fileSize) {
		void *endOfBuffer;
		int size = BUFFER_SIZE;
		if(offset+size >= fileSize) {
			done = TRUE;
			size -= (offset+size)-fileSize;
		}
		maReadData(R_XML, buffer, offset, size);
		err = FAXPP_SAX_parse(parser, buffer, size, &endOfBuffer, done);
		if(err == PREMATURE_END_OF_BUFFER) {
			offset+=(int)endOfBuffer-(int)buffer;
		} else if(err != NO_ERROR) {
	      printf("%03d:%03d ERROR: %s\n", FAXPP_get_error_line(parser),
             FAXPP_get_error_column(parser), FAXPP_err_to_string(err));
			 break;
		} else {
			offset+=size;
		}
	}
	
	FAXPP_SAX_close(parser);
	

	FREEZE;
	return 0;
}
