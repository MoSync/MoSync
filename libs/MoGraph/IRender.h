/*
 * IRender.h
 *
 *  Created on: Mar 14, 2013
 *      Author: CodeArt
 */

#ifndef IRENDER_H_
#define IRENDER_H_

// TODO SCENE MIGHT NEED AN INTERFACE!

namespace MoGraph
{
	class Scene;

	/**
	 * 	\brief Render
	 *  Base class used for all render objects like Bars, Text, Lines
	 *	contains a reference to the scene,
	 *	the scene objects has all relevant information about the scene
	 */
	class IRender
	{
	public:

		/**
		 * \brief ~Render , Destructor
		 */
		virtual~IRender() 		{};

		/**
		 * \brief init(),	generic init function for all Renderable objects
		 * that inherits Render class
		 */
		virtual void init() 	= 0;

		/**
		 * \brief draw(),	generic draw function for all Renderable objects
		 * that inherits Render class
		 */
		virtual void draw() 	= 0;

		/**
		 * \brief getScene(),	get the Scene class reference
		 * which this objects belongs to, (parent)
		 * @return Scene
		 */
		virtual Scene *getScene() 		= 0;
	};

	class Render : public IRender
	{
	public:
		/**
		 * \brief Render , Constructor
		 * @param scene , input parameter for reference store to scene
		 */
		Render(Scene *scene) : mScene(scene) {}

		/**
		 * \brief ~Render , Destructor
		 */
		virtual~Render() 		{};

		/**
		 * \brief init(),	generic init function for all Renderable objects
		 * that inherits Render class
		 */
		virtual void init() 	{};

		/**
		 * \brief draw(),	generic draw function for all Renderable objects
		 * that inherits Render class
		 */
		virtual void draw() 	{};

		/**
		 * \brief getScene(),	get the Scene class reference
		 * which this objects belongs to, (parent)
		 * @return Scene
		 */
		virtual Scene *getScene() 		{return mScene;}

	protected:
		Scene *mScene;		// Scene reference
	};

}
#endif /* IRENDER_H_ */
