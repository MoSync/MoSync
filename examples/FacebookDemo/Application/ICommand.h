/*
 * ICommand.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

namespace FacebookDemoApplication
{

/*
 * base class for all the commands used in this application
 */
class ICommand
{
public:
	virtual void execute() = 0;
	virtual ~ICommand() {}
};

}//namespace FacebookDemoApplication

#endif /* ICOMMAND_H_ */
