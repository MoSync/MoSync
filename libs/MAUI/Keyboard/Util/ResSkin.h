#include <maapi.h>

/**
 * @brief Defines a class that adds skins to the ResourceManager.
 * @file ResSkin.h
 * @author Mattias Frånberg
 */
namespace MAUI {

class SkinSet;

/**
 * Structure for a skin that is accessible as a resource.
 */
struct askin_t
{
	/**
	 * Symbolic ID for the skin
	 */
	const char *name;

	/**
	 * The resource for the selected image.
	 */
	MAHandle selected;

	/**
	 * The resource for the unselected skin.
	 */
	MAHandle unselected;

	/**
	 * Selected skin is transparent.
	 */
	bool sTransparent;

	/**
	 * Unselected skin is transparent.
	 */
	bool uTransparent;

	/**
	 * The rect that defines the skin, i.e.
	 *
	 *       x1
	 *    ___.________
	 * y1.|__|____|__|
	 *    |  |    |  |
	 *    |__|____|__|.y2
	 *    |__|____|__|
	 *            .
	 *            x2
	 */
	struct
	{
		int x1;
		int	x2;
		int	y1;
		int	y2;
	} rect;
};

/**
 * Structure for a skin that is accessible only as
 * an array of pixels.
 */
struct uskin_t
{
	/**
	 * Name of the skin.
	 */
	const char *name;

	/**
	* Array of pixels for the selected image.
	*/
	unsigned int *sPixels;

	/**
	* Array of pixels for the unselected image.
	*/
	unsigned int *uPixels;

	/**
	 * Width of the image for the skin.
	 */
	unsigned int width;

	/**
	 * Height of the image for the skin.
	 */
	unsigned int height;

	/**
	 * Determines whether the unselected skin should be transparent.
	 */
	bool uTransparent;

	/**
	 * Determines whether the selected skin should be transparent.
	 */
	bool sTransparent;

	/**
	 * The rect that defines the skin, i.e.
	 *
	 *       x1
	 *    ___.________
	 * y1.|__|____|__|
	 *    |  |    |  |
	 *    |__|____|__|.y2
	 *    |__|____|__|
	 *            .
	 *            x2
	 */
	struct
	{
		int x1;
		int x2;
		int y1;
		int y2;
	} rect;
};

/**
 * Class that allows a skin to be added to the resource manager during
 * static initialization.
 */
class ResSkin {
public:
	/**
	 * Creates a skin set from a set of unallocated skins, i.e. a list of
	 * skins specified as RGBA arrays.
	 *
	 * @param name Name of which the skin set should be registered by.
	 * @param skins A list of skins, it is assumed that the last element has name
	 * 		   		equal to NULL. Otherwise, behavior is unspecified.
	 */
	ResSkin(const char *name, struct uskin_t *skins);

	/**
	 * Creates a skin set from a set of allocated skins, i.e. a list of
	 * skins specified with given handles.
	 *
	 * @param name Name of which the skin set should be registered by.
	 * @param skins A list of skins, it is assumed that the last element has name
	 * 		   		equal to NULL. Otherwise, behavior is unspecified.
	 */
	ResSkin(const char *name, struct askin_t *skins);

private:
	/**
	 * Adds skins from a list of allocated skins.
	 *
	 * @param skins A list of skins, it is assumed that the last
	 * 				element of the list has name equal to NULL.
	 */
	void addAllocatedSkins(SkinSet *skinSet, struct askin_t *skins);

	/**
	 * Adds skins from a list of unallocated skins.
	 *
	 * @param skins A list of skins, it assumed that the last
	 * 				element of the list has name equal to NULL.
	 */
	void addUnallocatedSkins(SkinSet *skinSet, struct uskin_t *skins);
};
}
