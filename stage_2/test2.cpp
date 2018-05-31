/*************************************************************************
    > Copyright (C), 2015-2016, Kimura Lab., IPS, Waseda Univ.
    > All rights reserved.
    > @File:         test2.cpp
    > @Author:       Minghao Lin
    > @E-mail:       mhlin@ruri.waseda.jp
    > @Version:      1.0.0
    > @Date:         2016/3/7 0:10:04
    > @Description:
    > @History:      ----------------------------------
    >                date:         2016/3/7 0:10:04
    >                author:       Minghao Lin
    >                modification: Create
    >                ----------------------------------
    >                date:
    >                author:
    >                modification:
    >                ----------------------------------
    > @Others:
 ************************************************************************/
#include "GBDME.h"
#define FILE "DME_inputfile"
int main() {
    cout.precision( 20 );

    CGBDME dme;
    dme.setInputData( FILE );
    dme.dme( 1 );
    dme.outputRes();
    // dme.debug_input();
    // dme.debug_readBufLUT();
    dme.debug_loc();
    dme.debug_detour();
    dme.debug_iter_comp();
    return 0;
}
