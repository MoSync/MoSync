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
 * dbtest.h
 *
 *  Created on: May 15, 2013
 *      Author:
 */

#ifndef DBTEST_H_
#define DBTEST_H_


/**
 * Class that contains the database tests.
 */
class DBTest
{
public:
	void runTest();

	MAHandle openDatabase();

	void createTable(MAHandle db);
	void updateRow(MAHandle db);
	void deleteRow(MAHandle db);
	void countRows(MAHandle db);
	void queryRows(MAHandle db);

	void testParameters();

private:
	void shouldhold(bool condition, MAUtil::String errorMessage, MAUtil::String succesMessage);

};


#endif /* DBTEST_H_ */
