/*
 * SensorCov().c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"


ops_struct ops_temp, sc_start_ops;
data_struct data_temp;
stopwatch_struct* conv_watch;

void SensorCov()
{
	SensorCovInit();
	while (ops.State == STATE_SENSOR_COV)
	{
		LatchStruct();
		SensorCovMeasure();
		UpdateStruct();
		FillCANData();
	}
	SensorCovDeInit();
}

void SensorCovInit()
{
	//todo USER: SensorCovInit()
	//CONFIG ADC
	adcinit();

	//CONFIG GP_BUTTON
	ConfigGPButton();

	//CONFIG LEDS
	//led 0
	ConfigLED0();
	//led 1
	ConfigLED1();
	//CONFIG 12V SWITCH
	Config12V();
	conv_watch = StartStopWatch(4);
}


void LatchStruct()
{
	memcpy(&ops_temp, &ops, sizeof(struct OPERATIONS));
	memcpy(&data_temp, &data, sizeof(struct DATA));
}

void SensorCovMeasure()
{
	memcpy(&sc_start_ops, &ops_temp, sizeof(struct OPERATIONS));
	StopWatchRestart(conv_watch);
	//todo USER: Sensor Conversion
	//update data_temp and ops_temp
	//use stopwatch to catch timeouts
	//waiting should poll isStopWatchComplete() to catch timeout and throw StopWatchError

	readADC();
	data_temp.adc = A0RESULT;

	data_temp.gp_button = READGPBUTTON();

	if (data_temp.gp_button == 0) 			//if pushed cause stopwatch
	{
		SETLED0();
		int i = 0;
		while (i < 100)
		{
			i++;
		}
	}
	else
	{
		CLEARLED0();
	}

	if (data_temp.adc > 2000)
	{
		SETLED1();
	}
	else
	{
		CLEARLED1();
	}

	//exit and stopwatch error if timeout
	if (isStopWatchComplete(conv_watch) == 1)
	{
		ops_temp.Flags.bit.cov_error = 1;
	}
	else
	{
		ops_temp.Flags.bit.cov_error = 0;
	}


	if (ops_temp.Flags.all != 0)
	{
		SET12V();
	}
	else
	{
		CLEAR12V();
	}
}

void UpdateStruct()
{
	memcpy(&data, &data_temp, sizeof(struct DATA));

	//todo USER: UpdateStruct
	//update with node specific op changes

	//if ops is not changed outside of sensor conversion copy temp over, otherwise don't change

	//Change bit is only set by ops changes outside of SensorCov.
	if(sc_start_ops.State == ops.State)
	{
		ops.State = ops_temp.State;
	}

	if(sc_start_ops.Flags.all == ops.Flags.all)
	{
		//only cov error happens inside of conversion so all other changes are considered correct.
		//update accordingly to correct cov_errors
		ops.Flags.bit.cov_error = ops_temp.Flags.bit.cov_error;
	}
}

void SensorCovDeInit()
{
	//todo USER: SensorCovDeInit()
	StopStopWatch(conv_watch);
	CLEARLED0();
	CLEARLED1();
	CLEAR12V();
}
