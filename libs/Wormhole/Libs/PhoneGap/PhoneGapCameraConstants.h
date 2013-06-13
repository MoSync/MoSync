#ifndef PHONEGAPCAMERACONSTANTS_H
#define PHONEGAPCAMERACONSTANTS_H

namespace Wormhole
{
	enum DestinationType
	{
		DATA_URL, // Return image as base64 encoded string
		FILE_URI  // Return image file URI
	};

	enum PictureSourceType
	{
		PHOTOLIBRARY,
		CAMERA,
		SAVEDPHOTOALBUM
	};

	enum EncodingType
	{
		JPEG, // Return JPEG encoded image
		PNG   // Return PNG encoded image
	};

	enum MediaType
	{
		PICTURE,  // allow selection of still pictures only. DEFAULT.
				  // Will return format specified via DestinationType
		VIDEO,    // allow selection of video only, WILL ALWAYS RETURN FILE_URI
		ALLMEDIA  // allow selection from all media types
	};
}

#endif // PHONEGAPCAMERACONSTANTS_H
