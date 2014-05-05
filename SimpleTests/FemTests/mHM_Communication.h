/*
 * mHM_communication.h
 *
 *  Created on: Apr 29, 2014
 *      Author: waltherm
 */

#ifndef MHM_COMMUNICATION_H_
#define MHM_COMMUNICATION_H_


#include <iostream>


bool hello;


// used by ogs to call mHM caller
extern "C" void call_main_mHM();



void test_func()
{
	std::cout << "hello" << std::endl;
}


#endif /* MHM_COMMUNICATION_H_ */
