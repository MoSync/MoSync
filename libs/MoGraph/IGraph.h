/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#ifndef GRAPHAPI_H_
#define GRAPHAPI_H_

#include <glm/glm.hpp>

class IFont;

namespace MoGraph
{
	class Scene;
	class GraphDesc;
	class TouchInput;

	/**
	 * \brief IGraph class,	interface for the Graph
	 * \note Touch input is optional
	 */
	class IGraph
	{
	public:
		/**
		 * \brief ~IGraph Destructor
		 */
		virtual ~IGraph()  {}

		/**
		 * \brief GraphDesc, using the Graph Description struct
		 * to fill in the input values of the Graph
		 * @param desc, see GraphDesc struct
		 * @return
		 */
		virtual int init(GraphDesc *desc) = 0;
		/**
		 * \brief setBGColor
		 * @param color
		 */
		virtual void setBGColor(glm::vec4 &color) = 0;
		/**
		 * \brief setValues,	set the bar height value array
		 * @param values,	input pointer to a float array
		 * @param sz,	input size of array
		 */
		virtual void setValues(float *values,int sz) = 0;
		/**
		 * \brief setColor,	set the bar color array
		 * @param colors,	input pointer to a glm::vec4 color array
		 * @param sz,	input size of color array
		 */
		virtual void setColors(glm::vec4 *colors, int sz) = 0;
		/**
		 * \brief draw,	render entire scene
		 */
		virtual void draw() = 0;
		/**
		 * \brief getScene,	get the reference to the scene class
		 * @return	scene
		 */
		virtual Scene &getScene() = 0;
		/**
		 * \brief getTouch,	get the reference to the TouchInput helper class
		 * @return touch
		 */
		virtual TouchInput &getTouch() = 0;
	};
}

#endif /* GRAPHAPI_H_ */
