/*************************************************************************
    > Copyright (C), 2015-2016, Kimura Lab., IPS, Waseda Univ.
    > All rights reserved.
    > @File:         GBDME.h
    > @Author:       Minghao Lin
    > @E-mail:       mhlin@ruri.waseda.jp
    > @Version:      1.0.0
    > @Date:         2016/3/6 23:02:08
    > @Description:
    > @History:      ----------------------------------
    >                date:         2016/3/6 23:02:08
    >                author:       Minghao Lin
    >                modification: Create
    >                ----------------------------------
    >                date:
    >                author:
    >                modification:
    >                ----------------------------------
    > @Others:
 ************************************************************************/
#ifndef CGBDME_H
#define CGBDME_H
#include "stdafx.h"
#include <sstream>
#define EST_ALFA 0.3
#define LUT_PATH "bufferInsertion/Cl_Llmt_lut/Llimit"
#define MAX_VALUE 100000000000
#define MAX_LENGTH -1
#define NUM_OF_LAYERS 2
#define PA    -1
#define PB    0.1
#define WLIB  1
#define DEBUG
#define DEBUG_NODE -2
#define IS_IBM    1
#define DETOUR_COF  4
struct Box{
    double ll_x,ll_y;       // lower_left_x,y (in nm)
    double ur_x,ur_y;       // upper_right_x,y (in nm)
};

struct Node{
    int id;
    double x,y;
    double z;
    double cl;
    double cl_v;                    // cl + nv*cv
    double p, ptr;
    int parent, lchild, rchild;
    int left_TSV, right_TSV;
    double t;                       // node delay

    double edge;                    // edge_length
    int nv;                         // num of TSVs
    int nb;                         // num of buffer
    double lmt;                     // limit edge length

    double lut_ll;                  // opposite edge length
    double lut_clt;                 // lut parameter

    int g;                          // is a gate inserted in the edge

    double SC;                      // SCv in the node v
    double detour;                  // detours
};

struct MS{                  // merging segment
    int slope;             // the slope of merging segment (pos/neg)
    double l_x, l_y;        // left point
    double r_x, r_y;        // right point
};

struct TRR{
    double r;               // radius of TRR, edge length of ms
    MS core;                // core of TRR
};

struct Point{
    double x, y;
};

struct Segment{
    int from;
    int to;
    //int type;           // 0: wire, 1:buffer
};

struct PL{
    int id;
    double x;
    double y;
};

struct LUT{
    double clt;
    double ll;
    double cv;
    double lv;
};

class CGBDME{
    public:
        int iterative_complexity;   // iterative number in calcEdgeLength()
        int VDD_sourcePhase;        // parameters of controlling VDD and source phase

        Box chip_size;
        int num_of_lut;              // lmt_Cap_LUT
        int num_of_nodes;
        int num_of_sinks;
        int num_of_bufs;
        int num_of_gates;
        int num_of_tsv;
        int num_of_pl;
        int num_of_sgw;
        int num_of_sgb;
        int num_of_sgv;
        Node *node;
        double r0, c0;              // unit resistance and capacitance of wire
        double cg;                  // input cap of buffer
        double rg, cgo;             // output res and cap of buffer
        double cv;                  // unit cap of TSV
        double *cl_llmt_lut;        // Cload Llimit lut
        LUT *lut;

        // MS * ms;
        TRR * trr;
        double el_e, er_e;          // predicted edge radius
        double el, er;              // edge radius
        double el_i, er_i;

        int nvl, nvr;               // num of TSVs inserted in the edge
        //int nbl, nbr;               // num of buffers inserted in the edge
        int gl, gr;                 // is a gate inserted int the edge

        double max_length;          // getLimitLength(MAX_LENGTH)
        double md;

        double total_wire;
        double total_SC;
        double total_gate;

        Point * loc;
        Point * buf;
        Point * gate;

        vector<Segment>  sgw, sgb, sgv;
        vector<PL>  pl;

        void setInputData(string file_path);

        void dme(int v_sp);                 // main function of dme algorithm
        void outputRes();                   // output the result

        // debug function
        void debug_input();
        void debug_readBufLUT();
        void debug_loc();
        void debug_detour();
        void debug_iter_comp();             // get iterative complexity
    private:
        MS cross_ms;
        int cross_flag;
        void readBufLut(int v_sp);              // read Cload Llimit lut
        void dmeInit();                         // set initial value of sinks
        void calcEdgeLength(int l, int r);      // calculate edge length
        void locateBuf(int l, int r);           // locate buffers on two edges
        MS   mergeMS(int l, int r);             // merging MS(l) and MS(r) to MS(v)
        void updateInfo(int l, int r);          // after merging process, updata info. for the next mergement

        void bufInsertion();                    // insert buffers and gates
        //void sourceInsertion();                 // insert source to the root of tree

        MS isLineCross(int l, int r);
        void calcInitEL(int l, int r);
        double getLimitLength(int edge);        // do buffer insertion
        void selectSC(int l, int r, int flag);  // select the best SC strategy
        void calcAccurateEdge(int l, int r);    // calc the accurate edge length

        double lineDist(int l, int r);

        double MD(double x1, double y1, double x2, double y2);          // calc Manhattn Distance of two points
        double dist(int edge);
        double delay_w(double length, double c);
        double delay_b(double length, double c);                        // delay with wires and buffers
        double root(double a, double b, double c);                      // root of the function
        double min(double x, double y);
        double max(double x, double y);
        bool isColine(double x1, double y1, double x2, double y2, double x3, double y3);
        bool isIntersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
        double xx(double x1, double y1,double x2,double y2);
        double abs(double x);
};
#endif
