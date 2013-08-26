/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
