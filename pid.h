/*
 * pid.h
 *
 *  Created on: 2019Äê7ÔÂ23ÈÕ
 *      Author: admin
 */

#ifndef PID_H_
#define PID_H_

void pid_init(void);
int PIDControl(int pressure_set, int pressure_current);




#endif /* PID_H_ */
