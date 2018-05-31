/*************************************************************************
    > Copyright (C), 2015-2016, Kimura Lab., IPS, Waseda Univ.
    > All rights reserved.
    > @File:         AP_test.cpp
    > @Author:       Minghao Lin
    > @E-mail:       mhlin@ruri.waseda.jp
    > @Version:      1.0.0
    > @Date:         2016/1/5 15:36:48
    > @Description:
    > @History:      ----------------------------------
    >                date:         2016/1/5 15:36:48
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

#define IS_WRITE_FILE 1
int main() {
    ofstream f_test( "AP_testfile" );
    if ( !f_test.is_open() ) cout << "love's beautiful so beautiful" << endl;

    cout.precision( 20 );
    /*
    CInput input;
    input.getInputData(FILE,1);
    input.divideCircuit(3);
    input.debug();
    input.generateModules();
    input.generateInstStream();
    input.generateAP();
    */
    CAP   c_ap( 267, 64 );
    int** AP;
    // AP = c_ap.getAP(IS_WRITE_FILE);
    cout << "i love you" << endl;
    AP = c_ap.loadAP();
    int* mergeAP;
    // mergeAP = new int [10000];

    f_test << "AP[0]: " << endl;
    for ( int i = 0; i < 10000; i++ ) {
        f_test << AP[0][i];
    }
    f_test << endl;
    f_test << "AP[1]: " << endl;
    for ( int i = 0; i < 10000; i++ ) {
        f_test << AP[1][i];
    }
    f_test << endl << endl;

    cout << "from my heart" << endl;

    // verigy fucntion
    float p0, ptr0;
    float pm0, ptrm0;
    f_test << "P of AP[0]: " << c_ap.getP( AP[0] ) << endl;
    f_test << "Ptr of AP[0]: " << c_ap.getPtr( AP[0] ) << endl;

    mergeAP = c_ap.getAP( AP[0], AP[1] );
    f_test << endl << "merging AP: " << endl;
    for ( int i = 0; i < 10000; i++ ) {
        f_test << mergeAP[i];
    }
    f_test << endl << endl;

    f_test << "P of merging AP: " << c_ap.getP( AP[0], AP[1] ) << endl;
    f_test << "Ptr of merging AP: " << c_ap.getPtr( AP[0], AP[1] ) << endl;

    for ( int i = 0; i < 267; i++ ) {
        delete[] AP[i];
    }
    delete[] AP;
    delete[] mergeAP;
    system( "pause" );
    f_test.close();
    return 0;
}
