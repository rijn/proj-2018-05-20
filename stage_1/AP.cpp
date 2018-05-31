/*************************************************************************
    > Copyright (C), 2015-2016, Kimura Lab., IPS, Waseda Univ.
    > All rights reserved.
    > @File:         AP.cpp
    > @Author:       Minghao Lin
    > @E-mail:       mhlin@ruri.waseda.jp
    > @Version:      1.0.0
    > @Date:         2016/1/5 10:06:56
    > @Description:
    > @History:      ----------------------------------
    >                date:         2016/1/5 10:06:56
    >                author:       Minghao Lin
    >                modification: Create
    >                ----------------------------------
    >                date:
    >                author:
    >                modification:
    >                ----------------------------------
    > @Others:
 ************************************************************************/
#include "AP.h"
void CAP::init(int sinks, int inst){
    num_of_sinks = sinks;
    num_of_inst = inst;
}

int ** CAP::getAP(bool is_write_file){
    generateModules(is_write_file);
    generateInstStream(is_write_file);
    generateAP(is_write_file);
    return AP;
}

int **CAP::loadAP(){
    AP = new int *[num_of_sinks];           // define 2-D dynamic array
    for (int i=0; i<num_of_sinks; i++){
        AP[i] = new int [INST_LENGTH];
    }
    for (int i=0; i<num_of_sinks; i++)
        for (int j=0; j<INST_LENGTH; j++)
            AP[i][j] = 0;                   // initialize AP

    ifstream f_AP("Activity_Pattern");
    if (!f_AP.is_open()){
        cout << "there's no such AP file exist!" << endl;
        return 0;
    }
    string buf;
    f_AP >> buf >> buf;
    for (int i=0; i<num_of_sinks; i++){
        for (int j=0; j<INST_LENGTH; j++){
            f_AP >> AP[i][j];
        }
    }
    return AP;
}

int * CAP::getAP(int * ap1, int * ap2){
    int * ap;
    ap = new int [INST_LENGTH];
    for (int i=0; i<INST_LENGTH; i++){
        ap[i] = *(ap1+i) || *(ap2+i);
    }
    return ap;
}


float CAP::getP(int ap[]){
    float sum = 0;
    for (int i=0; i<INST_LENGTH; i++){
        sum+=ap[i];
    }
    return (sum / INST_LENGTH);
}

float CAP::getPtr(int ap[]){
    float sum = 0;
    for (int i=1; i<INST_LENGTH; i++){
        if (ap[i] != ap[i-1]){
            sum++;
        }
    }
    return (sum / (INST_LENGTH-1));
}

float CAP::getP(int ap1[], int ap2[]){
    int * ap;
    ap = new int [INST_LENGTH];
    ap = getAP(ap1, ap2);
    return getP(ap);
}

float CAP::getPtr(int ap1[], int ap2[]){
    int * ap;
    ap = new int [INST_LENGTH];
    ap = getAP(ap1, ap2);
    return getPtr(ap);
}

void CAP::generateModules(bool is_write_file){
    ofstream f_M("Inst_Module");
    if (!f_M.is_open())     cout << "can't create Inst_Module" << endl;

    for (int i=0; i<INST; i++)
        for (int j=0; j<MODULE; j++)
            inst_module[i][j] = 0;          // initialize inst_module
    // 35% - 45% modules
    float rate;
    int num_of_modules;
    float ave_modules;
    srand( (unsigned)time( NULL ) );
    for (int i=0; i<num_of_inst; i++){
        rate = (rand() % INC_MODULE + MIN_MODULE) * 0.01;
        num_of_modules = num_of_sinks * rate;
        getKnum(i, num_of_modules, num_of_sinks);

        if (is_write_file){
            ave_modules += rate;
            f_M << "instruction: " << i << endl;
            f_M << "rate: " << rate << endl;
            f_M << "Inst_Module: " << endl;
            for (int j=0; j<num_of_sinks; j++){
                f_M << inst_module[i][j];
            }
            f_M << endl << endl;
        }
    }
    if (is_write_file)  f_M << "Ave. Modules: " << (ave_modules/num_of_inst) << endl;
    f_M.close();
}

void CAP::generateInstStream(bool is_write_file){
    srand( (unsigned)time( NULL ) );
    // int count1 = 0; int count2 = 0;
    int high_inst, low_inst;
    high_inst = HIGH_INST_RATE * num_of_inst;   // 10% high activity inst.
    low_inst = num_of_inst - high_inst;         // 90% low activity inst.
    for (int i=0; i<INST_LENGTH; i++){
        if (rand()% HIGH_INST_NUM <1){          // each(high and low inst.) appear 50% of the time in the stream
            inst_stream[i] = rand() % high_inst;
        }
        else{
            inst_stream[i] = rand() % low_inst + high_inst;
        }
    }

    if (is_write_file){
        ofstream f_IS("Inst_Stream");
        if (!f_IS.is_open())    cout << "can't create Inst_Stream file" << endl;
        f_IS << "high_inst: " << high_inst << endl;
        f_IS << "low_inst: " << low_inst << endl;
        f_IS << "inst_stream: " << endl;
        float high_inst_proportion = 0;
        for (int i=0; i<INST_LENGTH; i++){
            f_IS << inst_stream[i] << endl;
            if (inst_stream[i]<high_inst)  high_inst_proportion++;
        }
        high_inst_proportion /= INST_LENGTH;
        f_IS << "high_inst_proportion: " << high_inst_proportion << endl;
        f_IS.close();
    }
}

void CAP::generateAP(bool is_write_file){
    AP = new int *[num_of_sinks];           // define 2-D dynamic array
    for (int i=0; i<num_of_sinks; i++){
        AP[i] = new int [INST_LENGTH];
    }
    for (int i=0; i<num_of_sinks; i++)
        for (int j=0; j<INST_LENGTH; j++)
            AP[i][j] = 0;                   // initialize AP

    for (int i=0; i<INST_LENGTH; i++){
        for (int j=0; j<num_of_sinks; j++){
            AP[j][i] = inst_module[inst_stream[i]][j];
        }
    }

    if (is_write_file){
        ofstream f_AP("Activity_Pattern");
        if (!f_AP.is_open())    cout << "can't create Activity_Pattern file" << endl;
        f_AP << "num_of_sinks: " << num_of_sinks << endl;
        for (int i=0; i<num_of_sinks; i++){
            for (int j=0; j<INST_LENGTH; j++){
                f_AP << AP[i][j] << " ";
            }
            f_AP << endl;
        }
        f_AP.close();
    }
}

void CAP::getKnum(int inst, int k, int n){
    for (int i=0; k>0 && i<n; i++){
        if (rand() % (n-1) < k){
             inst_module[inst][i] = 1;
             k--;
        }
    }
}

