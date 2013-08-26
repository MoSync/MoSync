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

/*
 * ICommand.h
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
