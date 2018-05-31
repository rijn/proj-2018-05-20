/*************************************************************************
    > Copyright (C), 2015-2016, Kimura Lab., IPS, Waseda Univ.
    > All rights reserved.
    > @File:         AP.h
    > @Author:       Minghao Lin
    > @E-mail:       mhlin@ruri.waseda.jp
    > @Version:      1.0.0
    > @Date:         2016/1/5 10:07:06
    > @Description:
    > @History:      ----------------------------------
    >                date:         2016/1/5 10:07:06
    >                author:       Minghao Lin
    >                modification: Create
    >                ----------------------------------
    >                date:
    >                author:
    >                modification:
    >                ----------------------------------
    > @Others:
 ************************************************************************/
#ifndef AP_H
#define AP_H
#include "stdafx.h"

#define INST 200
#define MODULE 1500
#define INST_LENGTH 10000
#define HIGH_INST_RATE 0.1  // 10% high activity inst. 90% low inst
#define HIGH_INST_NUM 2     // high_inst 1/2 distribute
#define MIN_MODULE 35
#define INC_MODULE 10  // 35% - 45% modules (35 - 35+10)
class CAP {
   public:
    // CAP(int sinks, int inst) : num_of_sinks(sinks), num_of_inst(inst){}
    // generate activity pattern
    int   num_of_sinks;
    int   num_of_inst;                // number of instruction
    int   inst_module[INST][MODULE];  // inst.- module table
    int   inst_stream[INST_LENGTH];   // inst. stream
    int** AP;                         // activity pattern of each sink

    void init( int sinks, int inst );   // set num_of_sinks and num_of_inst
    int** getAP( bool is_write_file );  // get initial AP (sink AP)
    int** loadAP();                     // loat AP from AP file
    // int * getAP(int ap1[], int ap2[]);          // get merging AP of two aps
    int* getAP( int* ap1, int* ap2 );  // get merging AP of two aps

    float getP( int ap[] );                // get probability
    float getPtr( int ap[] );              // get transition probability
    float getP( int ap1[], int ap2[] );    // get probability
    float getPtr( int ap1[], int ap2[] );  // get transition probability

   private:
    void generateModules(
        bool is_write_file );  // generate modules for each inst.
    void generateInstStream(
        bool is_write_file );               // generate instruction stream
    void generateAP( bool is_write_file );  // generate activity pattern

    void getKnum( int inst, int k, int n );  // select k non-duplicate numbers
                                             // from (0 ~ n-1). inst is the
                                             // instruction index.
};

#endif
