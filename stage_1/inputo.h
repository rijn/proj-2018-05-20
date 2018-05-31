/*************************************************************************
    > Copyright (C), 2015-2016, Kimura Lab., IPS, Waseda Univ.
    > All rights reserved.
    > @File:         input.h
    > @Author:       Minghao Lin
    > @E-mail:       mhlin@ruri.waseda.jp
    > @Version:      1.0.0
    > @Date:         2015/12/27 18:21:28
    > @Description:
    > @History:      ----------------------------------
    >                date:         2015/12/27 18:21:28
    >                author:       Minghao Lin
    >                modification: Create
    >                ----------------------------------
    >                date:
    >                author:
    >                modification:
    >                ----------------------------------
    > @Others:
 ************************************************************************/
#ifndef INPUT_H
#define INPUT_H
#include "AP.h"
#include "stdafx.h"

#define ISPD 0
#define IBM 1

#define BENCH_TYPE_IBM 1

#define INST_LENGTH 10000
#define IS_WRITE_FILE 1

#define TSV_CAP 15.48
#define GATE_CAP 35

#define PRE_ORDER 0
#define IN_ORDER 1
#define POST_ORDER 2
#define ALFA_CL 1

struct Box {
    double ll_x, ll_y;  // lower_left_x,y (in nm)
    double ur_x, ur_y;  // upper_right_x,y (in nm)
};
struct Source {
    int    source_name;  // source name
    double x, y;         // coordinate
    double z;            // height of source, or layer
    int    buf_name;     // buf name
};
struct Sink {
    int    id;    // id of sink
    double x, y;  // coordinate of sink
    double z;     // located layer of a sink
    double cl;    // capacitive load (in fF)

    int   ap[INST_LENGTH];  // activity pattern
    float p;                // probability
    float ptr;              // transition probability
    int   level;            // level
};
struct Wirelib {
    int    id;
    double r0;  // unit_wire_resistance (ohm/nm)
    double c0;  // unit_wire_capacitance (fF/nm)
};
struct Buflib {
    int    id;          // buf_id
    string spice_file;  // spice_subckt_filename
    bool   is_inv;      // is inverted
    double in_cl;       // input_load_cap
    double out_cl;      // output_parasitic_cap
    double out_rl;      // output_resistance
};
struct Blockage {
    int    id;
    double ll_x, ll_y;  // lower_left_x,y (in nm)
    double ur_x, ur_y;  // upper_right_x,y (in nm)
    double z;           // located layer of a block
};

struct MergingCell {
    int    a, b;
    double merging_cost;
};
struct cmpRemove {
    bool operator( /*int re_a, int re_b*/ )( const MergingCell& mc ) {
        // if ( (mc.a == re_a ) || (mc.a == re_b ) || (mc.b == re_a) || (mc.b ==
        // re_b)){
        return true;
        // }
        // else return false;
    };
};
struct cmpSort {
    bool operator()( const MergingCell& a, const MergingCell& b ) {
        return ( a.merging_cost < b.merging_cost );
    };
};
struct InitTopoTree {
    int      data;
    int      parent;
    int      lchild, rchild;
    int      left_TSV, right_TSV;
    int      l;
    set<int> el;
};
class CInput {
   public:
    CInput()
        : num_of_sinks( 0 ),
          num_of_wirelib( 0 ),
          num_of_buflib( 0 ),
          num_of_blockage( 0 ) {
    }
    int bench;  // benchmark
    // parameters of ispd benchmarks
    Box       chip_size;  // chip_size
    Source    source;
    int       num_of_sinks;  // shared parameters
    Sink*     sink;          // shared parameters
    int       num_of_wirelib;
    Wirelib*  wirelib;
    int       num_of_buflib;
    Buflib*   buflib;
    float     vdd[2];    // spice simulation in different setting
    int       lmt_slew;  // slew limit in ps
    double    lmt_cap;   // cap limit in fF (to limit power)
    int       num_of_blockage;
    Blockage* block;

    // parameters of ibm benchmarks
    double r0;  // per unit resistance
    double c0;  // per unit capacitance
    double rw;

    // generate activity pattern
    CAP   cap;
    int   num_of_inst;  // number of instruction
    int** AP;           // activity pattern of each sink

    // Nearest neighbor seletion algorithm
    list<MergingCell> mc;
    InitTopoTree*     tree_node;
    ofstream          f_itree;
    double            cw;  // unit cap of wire
    double            cv;  // unit cap of TSV
    double            cg;  // input cap of gate
    // Sink * c_sink[16];

    void setInputData( string file_path, int benchmark );
    void modiFile( int num_of_layers );
    void divideCircuit(
        int  num_of_layers,
        bool is_load );               // divide circuit to generate z coordinate
    void generateAP( bool is_load );  // generate sink APs
    void NNS();                       // get initial topology
    void outputInitTree( int traverse_type );  // output the init tree file

    void DLE();                     // defered layer embedding
    void outputInsertedTopoTree();  // output the TSV inserted tree file
    void debug();

    void output4DME();  // the input data for 3D-GB-DME
   private:
    // bool compMC(const MergingCell &a, const MergingCell &b);
    double getMergingCost( int a, int b );
    void mergeNode( int inner, int a, int b );
    double distance( double a_x, double a_y, double b_x, double b_y );
    double abs( double x );
    double halfRound( double x );

    int max( int x, int y );
    int min( int x, int y );

    void preOrder( InitTopoTree root );
    void inOrder( InitTopoTree root );
    void postOrder( InitTopoTree root );

    void deleteArray();
};
#endif
