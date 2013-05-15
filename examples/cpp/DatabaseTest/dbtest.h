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
