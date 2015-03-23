/*
 * mHMCommunication.h
 *
 *  Created on: Mar 23, 2015
 *      Author: waltherm
 */

#ifndef MHM_MHMCOMMUNICATION_H_
#define MHM_MHMCOMMUNICATION_H_

#include <iostream>

// used by ogs to call mHM caller
extern "C" void call_main_mHM();

inline void test_func()
{

	std::cout << "| C |" << std::endl;

	//call_main_mHM();

}


#endif /* MHM_MHMCOMMUNICATION_H_ */
