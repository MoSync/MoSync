
#include <conprint.h>

/* Attempt to copy 0xff from data[1] to dest[0] */
void CopyWithOffset(){
	unsigned char data[2] = { 0x7f, 0xff };
	unsigned char dest[2] = { 0, 0 };

	MAHandle placeholder = maCreatePlaceholder();
	maCreateData(placeholder, 2);
	maWriteData(placeholder, data, 0, 2);

	MAHandle dstPlaceholder = maCreatePlaceholder();
	maCreateData(dstPlaceholder, 1);

	MACopyData params = { dstPlaceholder, 0, placeholder, 1, 1};
	maCopyData(&params);

	maReadData(dstPlaceholder, dest, 0, 1);

	maDestroyObject(placeholder);
	maDestroyObject(dstPlaceholder);

	printf("dest[0] should be 255, is %i.", dest[0]);
}

extern "C" int MAMain() {
	CopyWithOffset();
	return 0;
}
