/*
 * op.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef OP_H_
#define OP_H_

//todo USER: define operation struct
//State defines. User states are greater than 3
#define STATE_USERSTATE      4;

//todo User: add user flags
typedef struct USER_FLAGS
{
	char user_error : 1;
}user_flags_struct;


#endif /* OP_H_ */
