//#include <input_ibm.h>
#include <sys/time.h>
#include <unordered_map>

#include "../tree.hpp"
#include "inputo.h"

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

    auto tree = input.mapToTree();
    tree->update();
    cout << "Update finish" << endl;
    int key = -1;
    tree->optimize(
        [&]( Tree<Sink *>::Node& a, Tree<Sink *>::Node& b ) {
          double GI = input.cw * input.distance( a.data->x, a.data->y, b.data->x, b.data->y );
          double LI = input.cv * ( abs( a.data->z - b.data->z ) );
          double BI = ( a.data->cl * a.data->p + b.data->cl * b.data->p ) +
                      0.5 * input.cg * ( a.data->ptr + b.data->ptr );
          return (float)( GI + LI + BI );
        },
        [&]( Tree<Sink *>::Node a, Tree<Sink *>::Node b ) {
          Tree<Sink *>::Node node(-1);
          node.data = new Sink();

          int *  t_ap = input.cap.getAP( a.data->ap, b.data->ap );
          double x1, x2, y1, y2, cx1, cx2, cy1, cy2;
          if ( a.data->x <= b.data->x ) {
              x1  = a.data->x;
              x2  = b.data->x;
              cx1 = a.data->cl;
              cx2 = b.data->cl;
          } else {
              x1  = b.data->x;
              x2  = a.data->x;
              cx1 = b.data->cl;
              cx2 = a.data->cl;
          }
          if ( a.data->y <= b.data->y ) {
              y1  = a.data->y;
              y2  = b.data->y;
              cy1 = a.data->cl;
              cy2 = b.data->cl;
          } else {
              y1  = b.data->y;
              y2  = a.data->y;
              cy1 = b.data->cl;
              cy2 = a.data->cl;
          }

          node.data->id = key--;
          node.data->x  = floor( ( a.data->x + b.data->x ) / 2 );
          node.data->y  = floor( ( a.data->y + b.data->y ) / 2 );
          node.data->z  = input.halfRound( ( a.data->z + b.data->z ) / 2 );
          node.data->cl = a.data->cl + b.data->cl;
          for ( int i = 0; i < INST_LENGTH; i++ ) node.data->ap[i] = t_ap[i];
          node.data->p = input.cap.getP( t_ap );
          node.data->ptr = input.cap.getPtr( t_ap );

          return node;
        } );
    tree->print();
    auto optimizedNodes = tree->findAllNodes([]( Tree<Sink *>::Node *node ) {
        return true;
    });
    struct MergeStep {
      int leftId;
      int rightId;
      int parentId;
    };
    std::vector<MergeStep> mergeSteps;
    std::unordered_map<int, int> mergeStepMap;
    int k = input.num_of_sinks;
    for ( auto& node : optimizedNodes ) {
        if (node->left != nullptr) {
          int leftId = node->left->data->id;
          int rightId = node->right->data->id;
          int parentId = node->data->id;
          if (parentId >= input.num_of_sinks || parentId < 0) {
              mergeStepMap[parentId] = k++;
          }
          if (mergeStepMap.find(leftId) != mergeStepMap.end()) leftId = mergeStepMap[leftId];
          if (mergeStepMap.find(rightId) != mergeStepMap.end()) rightId = mergeStepMap[rightId];
          if (mergeStepMap.find(parentId) != mergeStepMap.end()) parentId = mergeStepMap[parentId];
          mergeSteps.push_back({leftId, rightId, parentId});
        }
    }

    for ( auto& mergeStep : mergeSteps ) {
        cout << mergeStep.leftId << " + " << mergeStep.rightId << " => " << mergeStep.parentId << endl;
    }

    //一个算法，优化中间节点的z坐标。不用管它
    input.DLE();

    //输出优化过后的树
    input.outputInsertedTopoTree();

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
