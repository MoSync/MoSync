/*
 * GraphDesc.h
 *
 *  Created on: Mar 18, 2013
 *      Author: CodeArt
 */

#ifndef GRAPHDESC_H_
#define GRAPHDESC_H_

class IFont;

namespace MoGraph
{
	enum
	{
		DEFAULT_GRIDS = 0,
		MIRRORED_GRIDS = 1,
		OFFSET_GRIDS = 2
	};

	/**
	 * \brief GraphDesc. Graph Description used for its input data to set up the graph
	 * \note extDesc ptr has been reserved for the future in case the interface grows.
	 * it is set to zero and must not be used until provided with further info.
	 */

	struct GraphDesc
	{
		/**
		 * \brief GraphDesc, Constructor.
		 */
		GraphDesc() :
			scrWidth(0),
			scrHeight(0),
			gridX(10),
			gridZ(10),
			gridDecimals(1),
			gridYLines(5),
			gridOffsetStartLine(0.0f),
			gridStepYLines(5.0f),
			gridStepValue(0.5f),
			gridOffsetStartValue(0.0f),
			bFitScreen(true),
			flagGridLines(DEFAULT_GRIDS),
			bUseGridValue(true),
			font(0),
			extDesc(0)
		{
		}

		int 	scrWidth;			// Screen height
		int 	scrHeight;			// Screen width
		int 	gridX;				// Grid size of Z for graph
		int 	gridZ;				// Grid size on X for graph
		int		gridDecimals;		// set amount of decimals to be displayed as height data. 1 = 0.0 , 2 = 0.00 , 3 = 0.000 etc
		int 	gridYLines;			// Amount of grid lines
		float	gridOffsetStartLine;	// Lowest bound of line to start grid from
		float 	gridStepYLines;		// The steps in height for each line
		float	gridStepValue;		// Grid step value (step=10 => 10,20,30,40,50)
		float	gridOffsetStartValue;	// Lowest bound of Value to start from
		unsigned  bFitScreen:1, flagGridLines:2, bUseGridValue:1;
		IFont* 	font;				// ptr to the Font system
		void *	extDesc;			// extended parameter struct, reserved for future reference.
	};
}

#endif /* GRAPHDESC_H_ */
