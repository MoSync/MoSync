/*
 * ActionListener.h
 *
 *  Created on: Mar 15, 2010
 *      Author: romain
 */

#ifndef ACTIONLISTENER_H_
#define ACTIONLISTENER_H_

class ActionListener {
public:
	virtual void onCreate(int id) = 0 ;
	virtual void onClick(int id) = 0 ;
};

#endif /* ACTIONLISTENER_H_ */
