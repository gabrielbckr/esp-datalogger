#ifndef DATALOFFER_JOB
#define DATALOFFER_JOB

#include "Arduino.h"
#include <FS.h>


namespace dataLogger
{
    const String DF_FILENAME = String("/dataframe_data.csv");

    void setup(int);

    void writeRow(int, unsigned long);

    void samplingTask();
    
    void puseFile();

    void restartFile();
} 

#endif
