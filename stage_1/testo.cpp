//#include <input_ibm.h>
#include <sys/time.h>

#include "inputo.h"
#include "../tree.hpp"

//#define FILE "ispd2009/s1"
#define FILE "ispd2009/ispd09f31"
//#define FILE "ibm/r5.txt"
#define IS_WRITE_FILE 1
#define IS_LOAD1 0
#define IS_LOAD2 0
#define NUM_OF_LAYERS 2

int main() {
    // ofstream f_test("testfile");
    // if (!f_test.is_open())      cout << "love's beautiful so beautiful" <<
    // endl;
    struct timeval start1;
    struct timeval end1;
    struct timeval start2;
    struct timeval end2;
    unsigned long  diff1;
    unsigned long  diff2;

    gettimeofday( &start1, NULL );  // gettimeofday(&start,&tz);结果一样
    cout << "start1_tv_sec:" << start1.tv_sec << endl;
    cout << "start1_tv_usec:" << start1.tv_usec << endl;

    cout.precision( 20 );

    CInput input;

    //从benchmark ispd2009/s1中读取数据
    input.setInputData( FILE, 0 );

    //为了强行把一层变成两层而保持芯片面积不变，每层强行变化一下所有坐标缩小了√2倍
    input.modiFile( NUM_OF_LAYERS );

    //强行把一维的只有xy坐标的点分成两层，加上z坐标
    input.divideCircuit( NUM_OF_LAYERS, IS_LOAD1 );

    //调用AP.cpp 生成activity
    input.generateAP( IS_LOAD2 );

    // pattern。以上函数都不用管它，不用改

    gettimeofday( &start2, NULL );  // gettimeofday(&start,&tz);结果一样
    cout << "start2_tv_sec:" << start2.tv_sec << endl;
    cout << "start2_tv_usec:" << start2.tv_usec << endl;

    //最近邻选择算法，用这种算法每次选最小cost的配对，生成拓扑二叉树
    input.NNS();

    gettimeofday( &end2, NULL );
    cout << "end2_tv_sec:" << end2.tv_sec << endl;
    cout << "end2_tv_usec:" << end2.tv_usec << endl;
    diff2 = ( end2.tv_sec - start2.tv_sec ) * 1000000 + end2.tv_usec -
            start2.tv_usec;
    cout << "total time2 use is " << diff2 << endl;

    //输出生成的拓扑二叉树
    input.outputInitTree( 0 );

    //一个算法，优化中间节点的z坐标。不用管它
    input.DLE();

    //输出优化过后的树
    input.outputInsertedTopoTree();

    // Optimize
    auto tree = input.mapToTree();
    

    //输出dme_inputfile
    input.output4DME();

    input.debug();
    gettimeofday( &end1, NULL );

    cout << "end_tv_sec:" << end1.tv_sec << endl;
    cout << "end_tv_usec:" << end1.tv_usec << endl;
    diff1 = ( end1.tv_sec - start1.tv_sec ) * 1000000 + end1.tv_usec -
            start1.tv_usec;
    cout << "total time1 use is " << diff1 << endl;

    return 0;
}
