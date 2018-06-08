#include "GBDME.h"
void CGBDME::setInputData(string file_path){
    ifstream file(file_path.c_str());
    if (!file.is_open())    cout << "file is no exist!" << endl;
    string buf;
    getline(file, buf);     // get redundant info.
    getline(file, buf);     // get redundant info.
    file >> chip_size.ll_x  >> chip_size.ll_y >> chip_size.ur_x >> chip_size.ur_y;
    file >> r0 >> c0 >> cg >> cv;
    rg =61.2 ;  cgo =80;
    getline(file, buf);
    getline(file, buf);
    file >> buf >> num_of_nodes;
    node = new struct Node[num_of_nodes];
    for (int i=0; i<num_of_nodes; i++){
        file >> node[i].id >> node[i].x >> node[i].y >> node[i].z >> node[i].cl >> node[i].p >> node[i].ptr
             >> node[i].parent >> node[i].lchild >> node[i].rchild >> node[i].left_TSV >> node[i].right_TSV;
        node[i].t = 0;

        node[i].edge = node[i].nb = node[i].lmt = node[i].g = 0;
        //node[i].SC = node[i].cl;
    }
    num_of_tsv = 0;
    for (int i=0;i<num_of_nodes;i++){
        num_of_tsv =num_of_tsv+ node[i].left_TSV + node[i].right_TSV;
    }
    for (int i=0; i<num_of_nodes-1;i++){
        if (node[node[i].parent].lchild == i)
            node[i].nv = node[node[i].parent].left_TSV;
        else node[i].nv = node[node[i].parent].right_TSV;
        node[i].cl_v = node[i].cl + node[i].nv * cv;
        node[i].detour = 0;
    }

    file.close();
    num_of_sinks = (num_of_nodes+1)/2;

    for (int i=0; i<num_of_sinks; i++){
        node[i].lut_ll = 0;
        //node[i].lut_cv = node[i].cl_v;
        node[i].lut_clt = node[i].cl_v;
        //node[i].SC = node[i].cl;
        node[i].SC = node[i].cl_v;
    }
}

void CGBDME::dme(int v_sp){
    ofstream f_el("debug_edgeLength");
    if (!f_el.is_open())    cout << "can't open debug_edgeLength" << endl;
    f_el << "debug for function calcEdgeLength(l, r) in Bottom-Up Phase"<< endl;

    readBufLut(v_sp);
    cout<< "readBufLut over"<< endl;
    dmeInit();
    cout<< "dmeInit over"<< endl;
    // 1. the first phase: Bottom - Up Phase
    for (int i=num_of_sinks; i<num_of_nodes; i++){
        int l, r;
        cross_flag = 0;
        l = node[i].lchild;
        r = node[i].rchild;
        calcEdgeLength(l, r);
//*********** debug for calculate edge length ****************
    f_el << "id"<< i << endl;
    f_el << "el_e: " << el_e << " er_e: " << er_e << " el: " << el << " er: "<< er << endl;
    f_el << "nbl: " << node[l].nb << " nbr: " << node[r].nb << " gl: " << node[l].g << " gr: " << node[r].g << endl;
    f_el << "l_cl: " << node[l].cl_v << " r_cl: " << node[r].cl_v << "    l_t: "<< node[l].t << " r_t: " << node[r].t << endl;
    f_el << endl;
    if (node[r].cl_v<0 || node[l].cl_v <0 )     cout << "id(cl): " << node[l].parent << " l_cl: "<<node[l].cl_v<<" r_cl: "<< node[r].cl_v << endl;
    if (node[r].t <0 || node[l].t <0)    cout << "id(t): " << node[l].parent << " l_t: " << node[l].t << " r_t: " << node[r].t << endl;
    if (el <= 0 || er <= 0 )    cout << "id(el/er): " << node[l].parent << " el: " << el << " er: " << er << endl;
    
//*************************************************************
    //cout << trr[i].core.slope << "   " ;
        trr[l].r = el_i;
        trr[r].r = er_i;
        if (cross_flag == 1){
            cout << "parent:  " << i << endl;
            trr[i].core = cross_ms;
        }
        else
        trr[i].core = mergeMS(l, r);
        /*
        if (el_i+er_i != md )    cout << "mdddd ERROR : "<< i << endl;
        double mlml = MAX_VALUE;
        double mlt[4];
        mlt[0] = MD(trr[i].core.l_x, trr[i].core.l_y, trr[l].core.l_x, trr[l].core.l_y);
        mlt[1] = MD(trr[i].core.l_x, trr[i].core.l_y, trr[l].core.r_x, trr[l].core.r_y);
        mlt[2] = MD(trr[i].core.r_x, trr[i].core.r_y, trr[l].core.l_x, trr[l].core.l_y);
        mlt[3] = MD(trr[i].core.r_x, trr[i].core.r_y, trr[l].core.r_x, trr[l].core.r_y);
        for (int ii=0; ii<4; ii++){
            if (mlt[ii] < mlml)    mlml = mlt[ii];
        }
        if ( mlml != el_i )    cout << "mlml ERROR: "<< i <<" mlml="<<mlml<<endl;
        //if (i == 399 )    cout << "mlml: "<< mlml << "  el_i: "<< el_i<< endl;

        double mrmr = MAX_VALUE;
        double mrt[4];
        mrt[0] = MD(trr[i].core.l_x, trr[i].core.l_y, trr[r].core.l_x, trr[r].core.l_y);
        mrt[1] = MD(trr[i].core.l_x, trr[i].core.l_y, trr[r].core.r_x, trr[r].core.r_y);
        mrt[2] = MD(trr[i].core.r_x, trr[i].core.r_y, trr[r].core.l_x, trr[r].core.l_y);
        mrt[3] = MD(trr[i].core.r_x, trr[i].core.r_y, trr[r].core.r_x, trr[r].core.r_y);
        for (int ii=0; ii<4; ii++){
            if (mrt[ii] < mrmr)    mrmr = mrt[ii];
        }
        if ( mrmr != er_i )    cout << "mrmr ERROR: "<< i<<" mrmr=" << mrmr<<endl;
        //if (i == 235 )    cout << "mrmr: "<< mrmr << "  er_i: "<< er_i<< endl;
    */
    //cout<< "id: " << i<< " ms: " << trr[i].core.slope << " " << endl;
    //cout<< trr[i].core.l_x << " " << trr[i].core.l_y << " " << trr[i].core.r_x << " " << trr[i].core.r_y << endl;
        updateInfo(l, r);
    }
//*********** debug for trr ****************
    ofstream f_trr("debug_trr");
    if (!f_trr.is_open())    cout << "can't open debug_trr" << endl;
    f_trr << "debug for trr"<< endl;

    for (int i=0; i<num_of_nodes; i++){
        f_trr << "id="<< i <<"  parent="<< node[i].parent<< "    trr.r="<<trr[i].r << " trr.core="<< trr[i].core.slope << " " << trr[i].core.l_x << " "<< trr[i].core.l_y << " " << trr[i].core.r_x << " " << trr[i].core.r_y << endl;
        if (trr[i].core.slope != 0 && trr[i].core.slope!=1)    cout << "slope error: "<< i << endl;
    }

//*************************************************************
    cout<< "the first phase over"<< endl;

    // 2. the second phase: Top - Down Phase
    loc = new struct Point[num_of_nodes];
    int par;
    double ev;
    for (int i = 0; i< num_of_sinks; i++){
        loc[i].x = node[i].x;
        loc[i].y = node[i].y;
    }
    for (int i=num_of_nodes-1; i>num_of_sinks-1; i--){
        if (i==num_of_nodes-1){
            loc[i].x = trr[i].core.l_x;
            loc[i].y = trr[i].core.l_y;
        }
        else{
            par = node[i].parent;
            ev = trr[i].r;
            // 4 conditions. a terminal point in the square, both two points outside the square
            double x1, y1, x2, y2, xm, ym;
            int k;
            x1 = trr[i].core.l_x;    y1 = trr[i].core.l_y;
            x2 = trr[i].core.r_x;    y2 = trr[i].core.r_y;
            if (trr[i].core.slope == 1)    k = 1;
            else    k = -1;
            xm = loc[par].x;         ym = loc[par].y;

// ************************************
            if (i == DEBUG_NODE)    cout <<endl <<endl << xm << endl << ym<<endl << x1 << endl << y1 << endl << x2 << endl << y2 << endl<< ev<<endl<< endl;
// *****************************************

            if (MD(x1, y1, xm, ym)<=ev){
                loc[i].x = x1;
                loc[i].y = y1;
                if (i==DEBUG_NODE)    cout << "a11111111 "<< endl;
            }
            else if (MD(x2, y2, xm, ym) <=ev){
                loc[i].x = x2;
                loc[i].y = y2;
                if (i==DEBUG_NODE)    cout << "a22222222 "<< endl;
            }
            else{
                double yt = k*(xm - x2)+y2;
                double yyy;
                if (yt-ym>0)    yyy = yt-ym;
                else    yyy = ym - yt;
                //cout << "hhahahha--" << yyy<< " ev: "<< ev<< endl;
                if (i==DEBUG_NODE)    cout << "a3333333333 "<< endl;
                if (yyy <= ev){
                    loc[i].x = xm;
                    loc[i].y = yt;
                }
                else{
                    cout << "NO LOCATION for " << i << endl;
                }
            }
        }
        //if (i == 221)    cout << "loc_x: "<< loc[i].x << "  loc_y: " << loc[i].y << endl;
    }

    // 3. insert buffers and gates
    bufInsertion();
   
    cout << " buf insertion over" << endl;
    // 4. insert source
    //sourceInsertion();

}

void CGBDME::outputRes(){
    cout << "  ssss  ssss  ssss" <<endl;
    ofstream f_res("result");
    f_res.precision(9);
    f_res << "sourcenode 0 0" << endl;
    f_res << "num node "<< num_of_pl-num_of_sinks-1 <<endl;
    vector<Segment>::iterator it_sgw = sgw.begin();
    vector<Segment>::iterator it_sgb = sgb.begin();
    vector<Segment>::iterator it_sgv = sgv.begin();
    vector<PL>::iterator it_pl = pl.begin();
    cout << "plpl  begin"<< endl;
    for (it_pl = pl.begin();it_pl!=pl.end();it_pl++){
        if ((*it_pl).id >=num_of_sinks && (*it_pl).id< num_of_pl-1)
            f_res << (*it_pl).id+1 << " " << (*it_pl).x << " " << (*it_pl).y << endl;
    }

    cout << "plpl  over"<< endl;
    f_res << "num sinknode " << num_of_sinks << endl;
    for (it_pl = pl.begin();it_pl!=pl.end();it_pl++){
        if ((*it_pl).id < num_of_sinks)
            f_res << (*it_pl).id+1 << " " << (*it_pl).id+1 << endl;
    }
    f_res << "num wire " << num_of_sgw <<endl;
    for (it_sgw=sgw.begin(); it_sgw!=sgw.end();it_sgw++){
        if (it_sgw == sgw.end()-1)    f_res << 0 << " " << (*it_sgw).to +1 <<" 0"<< endl;
        else f_res << (*it_sgw).from+1 << " " << (*it_sgw).to+1 << " "<<WLIB << endl;
    }
    f_res << "num buffer " << num_of_sgb <<endl;
    for (it_sgb=sgb.begin(); it_sgb!=sgb.end();it_sgb++){
        f_res << (*it_sgb).from+1 << " " << (*it_sgb).to+1 << " 0" << endl;
    }
    f_res << "num tsv " << num_of_sgv << endl;
    for (it_sgv = sgv.begin();it_sgv!=sgv.end();it_sgv++){
        f_res << (*it_sgv).from+1 << " " << (*it_sgv).to+1 << endl;
    }
    total_SC = node[num_of_nodes-1].SC;
    cout << "total wire: " << total_wire << " total SC: " << total_SC << endl;
    for (int i =0; i<num_of_nodes; i++){
        total_gate = total_gate + node[i].g;
    }
    cout << "total gates: " << total_gate << endl;
    cout << "total buffers: " << num_of_sgb << endl;
    cout << "total tsv: "  << num_of_tsv << endl;
}

void CGBDME::readBufLut(int v_sp){
    // 1. set VDD and source phase, 6 conditions. VDD = 1, 1.1, 1.2; sp = 0, 1
    if (v_sp >=1 && v_sp <= 6) VDD_sourcePhase = v_sp;
    else cout << "error: VDD and source phase incorrect!" << endl;
    // 2. choose lut path based on VDD and source phase
    string lut_path = LUT_PATH;
    ostringstream sout;
    sout << VDD_sourcePhase;
    lut_path += sout.str();
    cout << "lut_path over" << endl;
    // 3. read Cload_Llimt_lut
    ifstream f_lut(lut_path.c_str());
    if (!f_lut.is_open())    cout << "f_lut is no exist!" << endl;
    string buf;
    f_lut >> buf >> num_of_lut;
    //lut = new struct LUT[num_of_lut];
    cl_llmt_lut = new double[num_of_lut];
    for (int i=0; i<num_of_lut; i++)
        f_lut >> buf >> cl_llmt_lut[i];
    f_lut.close();
}

void CGBDME::dmeInit(){
    iterative_complexity = 0;
    total_wire = total_SC = 0;
    total_gate = 0;
    max_length = getLimitLength(MAX_LENGTH);
    cout <<"max_length: "<< max_length << endl;
    trr = new struct TRR[num_of_nodes];
    for (int i=0; i<num_of_sinks; i++){
        trr[i].core.l_x = trr[i].core.r_x = node[i].x;
        trr[i].core.l_y = trr[i].core.r_y = node[i].y;
        trr[i].core.slope = 1;
        trr[i].r = -1;
    }
    
    for (int i=num_of_sinks; i<num_of_nodes; i++){
        trr[i].core.l_x = trr[i].core.r_x = 0;
        trr[i].core.l_y = trr[i].core.r_y = 0;
        trr[i].core.slope = 1;
        trr[i].r = -1;
    }
}

void CGBDME::calcEdgeLength(int l, int r){
    // 0. if two lines cross or overlap

    if (isLineCross(l,r).slope != -2){
        cross_ms = isLineCross(l,r);
        md = el_i = er_i = 0;
        node[l].edge = el_i;
        node[r].edge = er_i;
        cross_flag = 1;
        return ;
    }

    // 1. estimation. get predicted edge length, el_e and er_e;
    calcInitEL(l, r);
    node[l].lmt = getLimitLength(l);
    node[r].lmt = getLimitLength(r);
    el = el_e; er = er_e;
    int pre_nbl, pre_nbr;
    int pre_ngl, pre_ngr;
    double pre_el, pre_er;
    pre_nbl = pre_nbr = 0;
    pre_el = el; pre_er = er;
    pre_ngl = node[l].g = 0;   pre_ngr = node[r].g =0;
    int buf_flag = 0;
    while (1){
        iterative_complexity ++;
    // 2. buffer insertion. get the number of inserted buffers;
        double nbl = 0;
        double nbr = 0;
        double el_t, er_t;
        if (pre_nbl == 0) el_t = el - node[l].lmt;
        else el_t = el - (pre_nbl)*max_length - node[l].lmt;
        if (pre_nbr == 0 ) er_t = er -node[r].lmt;
        else er_t = er -(pre_nbr)*max_length-node[r].lmt;
        //debug
        if (el_t <= 0 )  nbl = 0;
        else{
            while (el_t > 0){
                nbl+=1;
                el_t-=max_length;
            }
        }
        if (er_t <= 0 )  nbr = 0;
        else{
            while (er_t > 0){
                nbr+=1;
                er_t-=max_length;
            }
        }
        /*
        if (buf_flag == 1){
            node[r].nb = nbr;
        }
        else if (buf_flag == 2){
            node[l].nb = nbl;
        }
        else{
            node[l].nb = nbl;
            node[r].nb = nbr;
        }
        buf_flag =0;
        */
        node[l].nb = nbl + pre_nbl;
        node[r].nb = nbr + pre_nbr;
        //node[l].nb = 0;
        //node[r].nb = 0;
        // 3. gate insertion. select the best SC strategy to insert gate;
        selectSC(l,  r, 0);
        // 4. edge calculation. get the accurate edge length, el and er;
        calcAccurateEdge(l, r);
        //cout << "hahahah" << node[l].parent << endl;
        if (pre_nbl == node[l].nb && pre_nbr == node[r].nb){
            break;
        }
        else{
            pre_nbl = node[l].nb;
            pre_nbr = node[r].nb;
        }
        /*
        if (pre_nbl == node[l].nb && pre_nbr == node[r].nb){
            break;
        }
        else if (pre_nbl > node[l].nb && pre_nbr >= node[r].nb){
            node[l].nb = pre_nbl; node[r].nb = pre_nbr;
            node[l].g = pre_ngl;  node[r].g = pre_ngr;
            el = pre_el ; er = pre_er;
            break;
        }
        else if (pre_nbl == node[l].nb && pre_nbr > node[r].nb){
            node[l].nb = pre_nbl; node[r].nb = pre_nbr;
            node[l].g = pre_ngl;  node[r].g = pre_ngr;
            el = pre_el ; er = pre_er;
            break;
        }
        else if (pre_nbl > node[l].nb && pre_nbr < node[r].nb){
            buf_flag = 1;
            pre_el = el;
            pre_er = er;
            pre_ngl = node[l].g;
            pre_ngr = node[r].g;
            pre_nbr = node[r].nb;
        }
        else if (pre_nbl < node[l].nb && pre_nbr > node[r].nb){
            buf_flag = 2;
            pre_el = el;
            pre_er = er;
            pre_ngl = node[l].g;
            pre_ngr = node[r].g;
            pre_nbl = node[l].nb;
        }
        else{
            pre_el = el;
            pre_er = er;
            pre_ngl = node[l].g;
            pre_ngr = node[r].g;
            pre_nbl = node[l].nb;
            pre_nbr = node[r].nb;
        }
        */
    }
    el_i = floor( el);
    er_i = md - el_i;
        if (node[l].parent==DEBUG_NODE)   cout << el_i << " " << er_i<< " "<<node[l].nb<<node[r].nb << endl;
    node[l].edge = el_i;
    node[r].edge = er_i;
    total_wire  = total_wire +el_i+er_i;
    //cout << "el = "  << el_i << "  er = " << er_i << endl;
}

MS CGBDME::mergeMS(int l, int r){
    MS ms, ms_t;
    MS l_ms[4];
    MS r_ms[4];
    int flag = 0;
    // 1. if overlap
    /*
    if (node[l].parent != 235){
        if (trr[l].r == 0) return trr[l].core;
        else if (trr[r].r == 0) return trr[r].core;
    }
    */
    //if (node[l].parent == 191)    cout << "@@@ 191 :  "<< trr[l].r << "  " << trr[r].r << " " << trr[l].core.slope << " "<< trr[r].core.slope<< endl;
    // 2. calc l0~l3, r0~r3
    if (trr[l].core.slope == 1){
        l_ms[0].slope = 0;
        l_ms[0].l_x = trr[l].core.l_x - el_i;    l_ms[0].l_y = trr[l].core.l_y;
        l_ms[0].r_x = trr[l].core.l_x;   l_ms[0].r_y = trr[l].core.l_y - el_i;
        //if (node[l].parent == 399 )  cout << "trr_l: " << trr[l].core.l_x << "  " << trr[l].core.l_y <<" "<< trr[l].core.r_x << " " << trr[l].core.r_y << endl;
        if(node[l].parent == DEBUG_NODE)cout << "l[0]: " << endl << l_ms[0].l_x << "  " << l_ms[0].l_y << endl << l_ms[0].r_x << " " << l_ms[0].r_y << endl;
        l_ms[1].slope = 0;
        l_ms[1].l_x = trr[l].core.r_x;   l_ms[1].l_y = trr[l].core.r_y + el_i;
        l_ms[1].r_x = trr[l].core.r_x + el_i;    l_ms[1].r_y = trr[l].core.r_y;
        if(node[l].parent == DEBUG_NODE)cout << "l[1]: " << endl << l_ms[1].l_x << "  " << l_ms[1].l_y << endl << l_ms[1].r_x << " " << l_ms[1].r_y << endl;

        l_ms[2].slope = 1;
        l_ms[2].l_x = trr[l].core.l_x;   l_ms[2].l_y = trr[l].core.l_y - el_i;
        l_ms[2].r_x = trr[l].core.r_x + el_i;    l_ms[2].r_y = trr[l].core.r_y;

        if(node[l].parent == DEBUG_NODE)cout << "l[2]: " <<endl<< l_ms[2].l_x << "  " << l_ms[2].l_y << endl << l_ms[2].r_x << " " << l_ms[2].r_y << endl;
        l_ms[3].slope = 1;
        l_ms[3].l_x = trr[l].core.l_x - el_i;   l_ms[3].l_y = trr[l].core.l_y;
        l_ms[3].r_x = trr[l].core.r_x;    l_ms[3].r_y = trr[l].core.r_y + el_i;
        if(node[l].parent == DEBUG_NODE)cout << "l[3]: " <<endl<< l_ms[3].l_x << "  " << l_ms[3].l_y << endl<< l_ms[3].r_x << " " << l_ms[3].r_y << endl;
    }
    else{
        l_ms[2].slope = 1;
        l_ms[2].l_x = trr[l].core.r_x;   l_ms[2].l_y = trr[l].core.r_y - el_i;
        l_ms[2].r_x = trr[l].core.r_x + el_i;   l_ms[2].r_y = trr[l].core.r_y;
        if(node[l].parent == DEBUG_NODE)cout << "l[2]: " << endl << l_ms[2].l_x << "  " << l_ms[2].l_y << endl << l_ms[2].r_x << " " << l_ms[2].r_y << endl;

        l_ms[3].slope = 1;
        l_ms[3].l_x = trr[l].core.l_x - el_i;   l_ms[3].l_y = trr[l].core.l_y;
        l_ms[3].r_x = trr[l].core.l_x;   l_ms[3].r_y = trr[l].core.l_y + el_i;
        if(node[l].parent == DEBUG_NODE)cout << "l[3]: " <<endl<< l_ms[3].l_x << "  " << l_ms[3].l_y << endl<< l_ms[3].r_x << " " << l_ms[3].r_y << endl;

        l_ms[0].slope = 0;
        l_ms[0].l_x = trr[l].core.l_x - el_i;   l_ms[0].l_y = trr[l].core.l_y;
        l_ms[0].r_x = trr[l].core.r_x;   l_ms[0].r_y = trr[l].core.r_y - el_i;
        if(node[l].parent == DEBUG_NODE)cout << "l[0]: " << endl << l_ms[0].l_x << "  " << l_ms[0].l_y << endl << l_ms[0].r_x << " " << l_ms[0].r_y << endl;

        l_ms[1].slope = 0;
        l_ms[1].l_x = trr[l].core.l_x;   l_ms[1].l_y = trr[l].core.l_y + el_i;
        l_ms[1].r_x = trr[l].core.r_x + el_i;   l_ms[1].r_y = trr[l].core.r_y;
        if(node[l].parent == DEBUG_NODE)cout << "l[1]: " << endl << l_ms[1].l_x << "  " << l_ms[1].l_y << endl << l_ms[1].r_x << " " << l_ms[1].r_y << endl;
    }

    if (trr[r].core.slope == 1){
        r_ms[0].slope = 0;
        r_ms[0].l_x = trr[r].core.l_x - er_i;    r_ms[0].l_y = trr[r].core.l_y;
        r_ms[0].r_x = trr[r].core.l_x;   r_ms[0].r_y = trr[r].core.l_y - er_i;
        if(node[l].parent == DEBUG_NODE)cout << "r[0]: " <<endl<< r_ms[0].l_x << "  " << r_ms[0].l_y << endl << r_ms[0].r_x << " " << r_ms[0].r_y << endl;

        r_ms[1].slope = 0;
        r_ms[1].l_x = trr[r].core.r_x;   r_ms[1].l_y = trr[r].core.r_y + er_i;
        r_ms[1].r_x = trr[r].core.r_x + er_i;    r_ms[1].r_y = trr[r].core.r_y;
        if(node[l].parent == DEBUG_NODE)cout << "r[1]: " <<endl<< r_ms[1].l_x << "  " << r_ms[1].l_y << endl<< r_ms[1].r_x << " " << r_ms[1].r_y << endl;

        r_ms[2].slope = 1;
        r_ms[2].l_x = trr[r].core.l_x;   r_ms[2].l_y = trr[r].core.l_y - er_i;
        r_ms[2].r_x = trr[r].core.r_x + er_i;    r_ms[2].r_y = trr[r].core.r_y;
        if(node[l].parent == DEBUG_NODE)cout << "r[2]: " <<endl<< r_ms[2].l_x << "  " << r_ms[2].l_y << endl << r_ms[2].r_x << " " << r_ms[2].r_y << endl;

        r_ms[3].slope = 1;
        r_ms[3].l_x = trr[r].core.l_x - er_i;   r_ms[3].l_y = trr[r].core.l_y;
        r_ms[3].r_x = trr[r].core.r_x;    r_ms[3].r_y = trr[r].core.r_y + er_i;
        if(node[l].parent == DEBUG_NODE)cout << "r[3]: " <<endl<< r_ms[3].l_x << "  " << r_ms[3].l_y << endl<< r_ms[3].r_x << " " << r_ms[3].r_y << endl;
    }
    else{
        r_ms[2].slope = 1;
        r_ms[2].l_x = trr[r].core.r_x;   r_ms[2].l_y = trr[r].core.r_y - er_i;
        r_ms[2].r_x = trr[r].core.r_x + er_i;   r_ms[2].r_y = trr[r].core.r_y;
        if(node[l].parent == DEBUG_NODE)cout << "r[2]: " <<endl<< r_ms[2].l_x << "  " << r_ms[2].l_y << endl << r_ms[2].r_x << " " << r_ms[2].r_y << endl;

        r_ms[3].slope = 1;
        r_ms[3].l_x = trr[r].core.l_x - er_i;   r_ms[3].l_y = trr[r].core.l_y;
        r_ms[3].r_x = trr[r].core.l_x;   r_ms[3].r_y = trr[r].core.l_y + er_i;
        if(node[l].parent == DEBUG_NODE)cout << "r[3]: " <<endl<< r_ms[3].l_x << "  " << r_ms[3].l_y << endl<< r_ms[3].r_x << " " << r_ms[3].r_y << endl;

        r_ms[0].slope = 0;
        r_ms[0].l_x = trr[r].core.l_x - er_i;   r_ms[0].l_y = trr[r].core.l_y;
        r_ms[0].r_x = trr[r].core.r_x;   r_ms[0].r_y = trr[r].core.r_y - er_i;
        if(node[l].parent == DEBUG_NODE)cout << "r[0]: " <<endl<< r_ms[0].l_x << "  " << r_ms[0].l_y << endl << r_ms[0].r_x << " " << r_ms[0].r_y << endl;

        r_ms[1].slope = 0;
        r_ms[1].l_x = trr[r].core.l_x;   r_ms[1].l_y = trr[r].core.l_y + er_i;
        r_ms[1].r_x = trr[r].core.r_x + er_i;   r_ms[1].r_y = trr[r].core.r_y;
        if(node[l].parent == DEBUG_NODE)cout << "r[1]: " <<endl<< r_ms[1].l_x << "  " << r_ms[1].l_y << endl<< r_ms[1].r_x << " " << r_ms[1].r_y << endl;
    }

    int flag2 = 0;                    // cross flag;
    if (trr[l].r == 0){
        if (trr[l].core.slope == 0){
            for (int i=0; i<2;i++){
                if (isColine(trr[l].core.l_x, trr[l].core.l_y,r_ms[i].l_x,r_ms[i].l_y, r_ms[i].r_x, r_ms[i].r_y)){
                    flag2 = 1;
                    if (trr[l].core.l_x <= r_ms[i].l_x){            // l_ms is in the left of r_ms
                        if (trr[l].core.r_x > r_ms[i].l_x){
                            flag = 1;
                            if (trr[l].core.r_x > r_ms[i].r_x){
                                ms.l_x = r_ms[i].l_x;
                                ms.l_y = r_ms[i].l_y;
                                ms.r_x = r_ms[i].r_x;
                                ms.r_y = r_ms[i].r_y;
                            }
                            else{
                                ms.l_x = r_ms[i].l_x;
                                ms.l_y = r_ms[i].l_y;
                                ms.r_x = trr[l].core.r_x;
                                ms.r_y = trr[l].core.r_y;
                            }
                        }
                        else if (trr[l].core.r_x == r_ms[i].l_x){
                            ms_t.l_x = ms_t.r_x = r_ms[i].l_x;
                            ms_t.l_y = ms_t.r_y = r_ms[i].l_y;
                        }
                    }
                    else{
                        if (r_ms[i].r_x > trr[l].core.l_x){
                            flag = 1;
                            if (r_ms[i].r_x > trr[l].core.r_x){
                                ms.l_x = trr[l].core.l_x;
                                ms.l_y = trr[l].core.l_y;
                                ms.r_x = trr[l].core.r_x;
                                ms.r_y = trr[l].core.r_y;
                            }
                            else{
                                ms.l_x = trr[l].core.l_x;
                                ms.l_y = trr[l].core.l_y;
                                ms.r_x = r_ms[i].r_x;
                                ms.r_y = r_ms[i].r_y;
                            }
                        }
                        else if (r_ms[i].r_x == trr[l].core.l_x){
                            ms_t.l_x = ms_t.r_x = r_ms[i].r_x;
                            ms_t.l_y = ms_t.r_y = r_ms[i].r_y;
                        }
                    }
                }
            }
        }
        else{
            int i_min, i_max;
            if (trr[l].core.l_x == trr[l].core.r_x && trr[l].core.l_y == trr[l].core.r_y){
                i_min = 0; i_max = 4;
            }
            else {
                i_min =2; i_max = 4;
            }
            for (int i=i_min; i<i_max; i++){
                if (isColine(trr[l].core.l_x, trr[l].core.l_y, trr[l].core.r_x, trr[l].core.r_y, r_ms[i].l_x, r_ms[i].l_y)){
                    flag2 = 1;
                    if (trr[l].core.l_x <= r_ms[i].l_x){
                        if (trr[l].core.r_x > r_ms[i].l_x){
                            flag = 1;
                            if (trr[l].core.r_x > r_ms[i].r_x){
                                ms.l_x = r_ms[i].l_x;
                                ms.l_y = r_ms[i].l_y;
                                ms.r_x = r_ms[i].r_x;
                                ms.r_y = r_ms[i].r_y;
                            }
                            else{
                                ms.l_x = r_ms[i].l_x;
                                ms.l_y = r_ms[i].l_y;
                                ms.r_x = trr[l].core.r_x;
                                ms.r_y = trr[l].core.r_y;
                            }
                        }
                        else if (trr[l].core.r_x == r_ms[i].l_x){
                            ms_t.l_x = ms_t.r_x = r_ms[i].l_x;
                            ms_t.l_y = ms_t.r_y = r_ms[i].l_y;
                        }
                    }
                    else{
                        if (r_ms[i].r_x > trr[l].core.l_x){
                            flag = 1;
                            if (r_ms[i].r_x > trr[l].core.r_x){
                                ms.l_x = trr[l].core.l_x;
                                ms.l_y = trr[l].core.l_y;
                                ms.r_x = trr[l].core.r_x;
                                ms.r_y = trr[l].core.r_y;
                            }
                            else {
                                ms.l_x = trr[l].core.l_x;
                                ms.l_y = trr[l].core.l_y;
                                ms.r_x = r_ms[i].r_x;
                                ms.r_y = r_ms[i].r_y;
                            }
                        }
                        else if (r_ms[i].r_x == trr[l].core.l_x){
                            ms_t.l_x = ms_t.r_x = r_ms[i].r_x;
                            ms_t.l_y = ms_t.r_y = r_ms[i].r_y;
                        }
                    }
                }
            }
        }
        if (flag2 == 0){
            double mlml = MAX_VALUE;
            int index;
            double mlt[4];
            mlt[0] = MD(trr[l].core.l_x, trr[l].core.l_y, trr[r].core.l_x, trr[r].core.l_y);
            mlt[1] = MD(trr[l].core.l_x, trr[l].core.l_y, trr[r].core.r_x, trr[r].core.r_y);
            mlt[2] = MD(trr[l].core.r_x, trr[l].core.r_y, trr[r].core.l_x, trr[r].core.l_y);
            mlt[3] = MD(trr[l].core.r_x, trr[l].core.r_y, trr[r].core.r_x, trr[r].core.r_y);
            for (int i=0; i<4; i++){
                if (mlt[i] < mlml){
                    mlml = mlt[i];
                    index = i;
                }
            }
            if (index < 2){
                ms.l_x = ms.r_x = trr[l].core.l_x;
                ms.l_y = ms.r_y = trr[l].core.l_y;
                ms.slope = 1;
            }
            else{
                ms.l_x = ms.r_x = trr[l].core.r_x;
                ms.l_y = ms.r_y = trr[l].core.r_y;
                ms.slope = 1;
            }
            return ms;
        }
        if (flag == 0) ms = ms_t;
        if (ms.l_x < ms.r_x && ms.l_y < ms.r_y)    ms.slope =1;
        else ms.slope = 0;
        return ms;
    }
    else if (trr[r].r == 0){
        if (trr[r].core.slope == 0){
            for (int i=0; i<2;i++){
                if (isColine(trr[r].core.l_x, trr[r].core.l_y,l_ms[i].l_x,l_ms[i].l_y, l_ms[i].r_x, l_ms[i].r_y)){
                    flag2 = 1;
                    if (trr[r].core.l_x <= l_ms[i].l_x){            // l_ms is in the left of l_ms
                        if (trr[r].core.r_x > l_ms[i].l_x){
                            flag = 1;
                            if (trr[r].core.r_x > l_ms[i].r_x){
                                ms.l_x = l_ms[i].l_x;
                                ms.l_y = l_ms[i].l_y;
                                ms.r_x = l_ms[i].r_x;
                                ms.r_y = l_ms[i].r_y;
                            }
                            else{
                                ms.l_x = l_ms[i].l_x;
                                ms.l_y = l_ms[i].l_y;
                                ms.r_x = trr[r].core.r_x;
                                ms.r_y = trr[r].core.r_y;
                            }
                        }
                        else if (trr[r].core.r_x == l_ms[i].l_x){
                            ms_t.l_x = ms_t.r_x = l_ms[i].l_x;
                            ms_t.l_y = ms_t.r_y = l_ms[i].l_y;
                        }
                    }
                    else{
                        if (l_ms[i].r_x > trr[r].core.l_x){
                            flag = 1;
                            if (l_ms[i].r_x > trr[r].core.r_x){
                                ms.l_x = trr[r].core.l_x;
                                ms.l_y = trr[r].core.l_y;
                                ms.r_x = trr[r].core.r_x;
                                ms.r_y = trr[r].core.r_y;
                            }
                            else{
                                ms.l_x = trr[r].core.l_x;
                                ms.l_y = trr[r].core.l_y;
                                ms.r_x = l_ms[i].r_x;
                                ms.r_y = l_ms[i].r_y;
                            }
                        }
                        else if (l_ms[i].r_x == trr[r].core.l_x){
                            ms_t.l_x = ms_t.r_x = l_ms[i].r_x;
                            ms_t.l_y = ms_t.r_y = l_ms[i].r_y;
                        }
                    }
                }
            }
        }
        else{
            int i_min, i_max;
            if (trr[r].core.l_x == trr[r].core.r_x && trr[r].core.l_y == trr[r].core.r_y){
                i_min = 0; i_max = 4;
            }
            else {
                i_min =2; i_max = 4;
            }
            for (int i=i_min; i<i_max; i++){
                if (isColine(trr[r].core.l_x, trr[r].core.l_y, trr[r].core.r_x, trr[r].core.r_y, l_ms[i].l_x, l_ms[i].l_y)){
                    flag2 = 1;
                    if (trr[r].core.l_x <= l_ms[i].l_x){
                        if (trr[r].core.r_x > l_ms[i].l_x){
                            flag = 1;
                            if (trr[r].core.r_x > l_ms[i].r_x){
                                ms.l_x = l_ms[i].l_x;
                                ms.l_y = l_ms[i].l_y;
                                ms.r_x = l_ms[i].r_x;
                                ms.r_y = l_ms[i].r_y;
                            }
                            else{
                                ms.l_x = l_ms[i].l_x;
                                ms.l_y = l_ms[i].l_y;
                                ms.r_x = trr[r].core.r_x;
                                ms.r_y = trr[r].core.r_y;
                            }
                        }
                        else if (trr[r].core.r_x == l_ms[i].l_x){
                            ms_t.l_x = ms_t.r_x = l_ms[i].l_x;
                            ms_t.l_y = ms_t.r_y = l_ms[i].l_y;
                        }
                    }
                    else{
                        if (l_ms[i].r_x > trr[r].core.l_x){
                            flag = 1;
                            if (l_ms[i].r_x > trr[r].core.r_x){
        if (node[l].parent == DEBUG_NODE)    cout <<" yyyyyyyyyyyyyyyyyy-----------555" <<endl;
                                ms.l_x = trr[r].core.l_x;
                                ms.l_y = trr[r].core.l_y;
                                ms.r_x = trr[r].core.r_x;
                                ms.r_y = trr[r].core.r_y;
                            }
                            else {
        if (node[l].parent == DEBUG_NODE)    cout <<" yyyyyyyyyyyyyyyyyy-----------666" <<endl;
                                ms.l_x = trr[r].core.l_x;
                                ms.l_y = trr[r].core.l_y;
                                ms.r_x = l_ms[i].r_x;
                                ms.r_y = l_ms[i].r_y;
                            }
                        }
                        else if (l_ms[i].r_x == trr[r].core.l_x){
        if (node[l].parent == DEBUG_NODE)    cout <<" yyyyyyyyyyyyyyyyyy-----------6777" <<endl;
                            ms_t.l_x = ms_t.r_x = l_ms[i].r_x;
                            ms_t.l_y = ms_t.r_y = l_ms[i].r_y;
                        }
                    }
                }
            }
        }
        if (flag2 == 0){
            double mrmr = MAX_VALUE;
            int index;
            double mrt[4];
            mrt[0] = MD(trr[r].core.l_x, trr[r].core.l_y, trr[l].core.l_x, trr[l].core.l_y);
            mrt[1] = MD(trr[r].core.l_x, trr[r].core.l_y, trr[l].core.r_x, trr[l].core.r_y);
            mrt[2] = MD(trr[r].core.r_x, trr[r].core.r_y, trr[l].core.l_x, trr[l].core.l_y);
            mrt[3] = MD(trr[r].core.r_x, trr[r].core.r_y, trr[l].core.r_x, trr[l].core.r_y);
            for (int i=0; i<4; i++){
                if (mrt[i] < mrmr){
                    mrmr = mrt[i];
                    index = i;
                }
            }
            if (index < 2){
                ms.l_x = ms.r_x = trr[r].core.l_x;
                ms.l_y = ms.r_y = trr[r].core.l_y;
                ms.slope = 1;
            }
            else{
                ms.l_x = ms.r_x = trr[r].core.r_x;
                ms.l_y = ms.r_y = trr[r].core.r_y;
                ms.slope = 1;
            }
            return ms;
        }

        if (flag == 0) ms = ms_t;
        if (ms.l_x < ms.r_x && ms.l_y < ms.r_y)    ms.slope =1;
        else ms.slope = 0;
        return ms;

    }

    // 3. calc ms
    // slope = -1
    for (int i=0; i<2; i++){
        for (int j=0; j<2; j++){
            if (isColine(l_ms[i].l_x, l_ms[i].l_y, l_ms[i].r_x, l_ms[i].r_y, r_ms[j].l_x, r_ms[j].l_y)){
                //if (node[r].parent == 191)  cout << "### i= "<< i << " j=" <<j <<endl;
                //ms.slope = 0; ms_t.slope = 0;
                if (l_ms[i].l_x <= r_ms[j].l_x){            // l_ms is in the left of r_ms
                    if (l_ms[i].r_x > r_ms[j].l_x){
                        //if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------1"<< endl;
                        flag = 1;
                        if (l_ms[i].r_x > r_ms[j].r_x){
                        //if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------2"<< endl;
                            ms.l_x = r_ms[j].l_x;
                            ms.l_y = r_ms[j].l_y;
                            ms.r_x = r_ms[j].r_x;
                            ms.r_y = r_ms[j].r_y;
                        }
                        else{
                        //if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------3"<< endl;
                            ms.l_x = r_ms[j].l_x;
                            ms.l_y = r_ms[j].l_y;
                            ms.r_x = l_ms[i].r_x;
                            ms.r_y = l_ms[i].r_y;
                        }
                    }
                    else if (l_ms[i].r_x == r_ms[j].l_x){
                        //if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------4"<< endl;
                        ms_t.l_x = ms_t.r_x = r_ms[j].l_x;
                        ms_t.l_y = ms_t.r_y = r_ms[j].l_y;
                    }
                }
                else{
                    if (r_ms[j].r_x > l_ms[i].l_x){
                        flag = 1;
                        if (r_ms[j].r_x > l_ms[i].r_x){
                        //if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------5"<< endl;
                            ms.l_x = l_ms[i].l_x;
                            ms.l_y = l_ms[i].l_y;
                            ms.r_x = l_ms[i].r_x;
                            ms.r_y = l_ms[i].r_y;
                        }
                        else{
                        //if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------6"<< endl;
                            ms.l_x = l_ms[i].l_x;
                            ms.l_y = l_ms[i].l_y;
                            ms.r_x = r_ms[j].r_x;
                            ms.r_y = r_ms[j].r_y;
                        }
                    }
                    else if (r_ms[j].r_x == l_ms[i].l_x){
                        //if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------7"<< endl;
                        ms_t.l_x = ms_t.r_x = r_ms[j].r_x;
                        ms_t.l_y = ms_t.r_y = r_ms[j].r_y;
                    }
                }
            }
        }
    }
    // slope = 1
    for (int i=2; i<4; i++){
        for (int j=2; j<4; j++){
            if (isColine(l_ms[i].l_x, l_ms[i].l_y, l_ms[i].r_x, l_ms[i].r_y, r_ms[j].l_x, r_ms[j].l_y)){
                //if (node[r].parent == 191)  cout << "@@@@@@@@@@@@@@@@@ i= "<< i << " j=" << j <<endl;
                //ms.slope = 1; ms_t.slope = 1;
                if (l_ms[i].l_x <= r_ms[j].l_x){
                    if (l_ms[i].r_x > r_ms[j].l_x){
                        if (node[l].parent == 585)    cout << "yyyyyyyyyyyyyyyy-----------1"<< endl;
                        flag = 1;
                        if (l_ms[i].r_x > r_ms[j].r_x){
                            ms.l_x = r_ms[j].l_x;
                            ms.l_y = r_ms[j].l_y;
                            ms.r_x = r_ms[j].r_x;
                            ms.r_y = r_ms[j].r_y;
                        }
                        else{
                            ms.l_x = r_ms[j].l_x;
                            ms.l_y = r_ms[j].l_y;
                            ms.r_x = l_ms[i].r_x;
                            ms.r_y = l_ms[i].r_y;
                        }
                        //if (node[r].parent == 191)  cout << "111111111111111 " <<endl;
                    }
                    else if (l_ms[i].r_x == r_ms[j].l_x){
                        ms_t.l_x = ms_t.r_x = r_ms[j].l_x;
                        ms_t.l_y = ms_t.r_y = r_ms[j].l_y;
                    }
                }
                else{
                    if (r_ms[j].r_x > l_ms[i].l_x){
                        flag = 1;
                        if (r_ms[j].r_x > l_ms[i].r_x){
                            ms.l_x = l_ms[i].l_x;
                            ms.l_y = l_ms[i].l_y;
                            ms.r_x = l_ms[i].r_x;
                            ms.r_y = l_ms[i].r_y;
                        }
                        else{
                            ms.l_x = l_ms[i].l_x;
                            ms.l_y = l_ms[i].l_y;
                            ms.r_x = r_ms[j].r_x;
                            ms.r_y = r_ms[j].r_y;
                        }
                       // if (node[r].parent == 191)  cout << "2222222222222" <<endl;
                    }
                    else if (r_ms[j].r_x == l_ms[i].l_x){
                        ms_t.l_x = ms_t.r_x = r_ms[j].r_x;
                        ms_t.l_y = ms_t.r_y = r_ms[j].r_y;
                    }
                }
            }
        }
    }
    if (flag == 0) ms = ms_t;
    if (ms.l_x < ms.r_x && ms.l_y < ms.r_y)    ms.slope =1;
    else ms.slope = 0;
   // if (node[l].parent == 191)cout << "ms_191:   " << ms.l_x << " " << ms.l_y << "  " << ms.r_x <<" " << ms.r_y << " " <<ms.slope <<endl;
    //if (node[l].parent == 191) cout << "flag:  " << flag<< endl;
    return ms;
}

void CGBDME::updateInfo(int l, int r){
    int v = node[l].parent;
    double t1 = node[l].t;
    double t2 = node[r].t;
    // 1. update node[v].x and node[v].y;
    /*
    if (el_i ==0 ) {
        node[v].x = node[l].x;
        node[v].y = node[l].y;
    }
    else if (er_i == 0){
        node[v].x = node[r].x;
        node[v].y = node[r].y;
    }
    else{
        node[v].x = (node[r].x + (er_i/el_i)*node[l].x) / (1+er_i/el_i);
        node[v].y = (node[r].y + (er_i/el_i)*node[l].y) / (1+er_i/el_i);
    }
    */

    // 2. update capacitance and delay;
    double cl1, cl2;
    double lut_ll1, lut_ll2;
    double lut_clt1, lut_clt2;
    if (node[l].g == 1){
        cl1 = cg;
        lut_ll1 = 0;
        lut_clt1 = cg;
        if (node[l].nb == 0)    t1 += delay_b(el_i, node[l].cl_v);
        // else if (node[l].nb == 1)    t1 += delay_b(node[l].lmt, node[l].cl_v) + delay_b(el_i-node[l].lmt, cg);
        else    t1 += delay_b(node[l].lmt, node[l].cl_v) + (node[l].nb - 1) * delay_b(max_length, cg) +
                      delay_b(el_i - node[l].lmt - (node[l].nb -1)*max_length, cg);
    }
    else{
        if (node[l].nb == 0){
            lut_ll1 = el_i;
            lut_clt1 = node[l].cl_v;

            cl1 = node[l].cl_v + c0*lut_ll1;
            t1 += delay_w(el_i, node[l].cl_v);

        }
        // else if (node[l].nb == 1)    cl1 = cg + c0*(el_i - node[l].lmt);
        else{
            lut_ll1 = el_i - node[l].lmt - (node[l].nb - 1)*max_length ;
            lut_clt1 = cg;

            cl1 = cg + c0*lut_ll1;
            t1 += delay_b(node[l].lmt, node[l].cl_v) + (node[l].nb-1) * delay_b(max_length, cg) +
                  delay_w(el_i - node[l].lmt - (node[l].nb -1)* max_length, cg);

        }
    }

    if (node[r].g == 1){
        cl2 = cg;
        lut_ll2 = 0;
        lut_clt2 = cg;
        if (node[r].nb == 0) t2 += delay_b(er_i, node[r].cl_v);
        else    t2 += delay_b(node[r].lmt, node[r].cl_v) + (node[r].nb - 1) * delay_b(max_length, cg) +
                      delay_b(er_i - node[r].lmt - (node[r].nb - 1)*max_length, cg);
    }
    else{
        if (node[r].nb == 0){
            lut_ll2 = er_i;
            lut_clt2 = node[r].cl_v;

            cl2 = node[r].cl_v + c0*er_i;
            t2 += delay_w(er_i, node[r].cl_v);
        }
        // else if (node[r].nb == 1)    cl2 = cg + c0*(er_i - node[r].lmt);
        else {
            lut_ll2 = er_i - node[r].lmt - (node[r].nb - 1)*max_length ;
            lut_clt2 = cg;

            cl2 = cg + c0*(er_i - node[r].lmt - (node[r].nb - 1)*max_length);
            t2 += delay_b(node[r].lmt, node[r].cl_v) + (node[r].nb-1) * delay_b(max_length, cg) +
                  delay_w(er_i - node[r].lmt - (node[r].nb -1)* max_length, cg);
        }
    }
    // lut parameter updation
    if (delay_w(lut_ll1,lut_clt1) > delay_w(lut_ll2,lut_clt2)){
        node[v].lut_ll = lut_ll1;
        node[v].lut_clt = lut_clt1;
    }
    else{
        node[v].lut_ll = lut_ll2;
        node[v].lut_clt = lut_clt2;
    }
    // update cl_v
    node[v].cl = cl1 + cl2;
    node[v].cl_v =node[v].cl + node[v].nv * cv;
    // 3. update node delay;
    node[v].t = max(t1, t2);
    // 4. update node SC;
    //selectSC(l, r, 1);
}

void CGBDME::bufInsertion(){
    ofstream f_d_el("edge_length_comp");
    num_of_bufs = 0;
    num_of_gates = 0;
    gate = new struct Point[num_of_nodes];

    num_of_sgw = num_of_sgb = num_of_sgv = 0;
    num_of_pl = num_of_sinks;

    PL pl_t;
    vector<PL>::iterator it_pl;
    Segment sgw_t, sgb_t, sgv_t;

    int * cross_point;
    cross_point = new int[num_of_nodes];
    Point *buf_t;
    buf_t = new struct Point[10000];
    for (int i=0; i<num_of_nodes; i++){
        if (num_of_pl == 244)  cout << "iiiii  " << i << endl;
        if (i<num_of_sinks){
            pl_t.id = i;
            pl_t.x = loc[i].x;
            pl_t.y = loc[i].y;
            pl.push_back(pl_t);
            int ss = i;
            for (int i_v=0; i_v < node[i].nv; i_v++){
                pl_t.id = num_of_pl++;
                pl_t.x = loc[i].x;
                pl_t.y = loc[i].y;
                pl.push_back(pl_t);

                if (ss == 0)  cout << "pl_x:= " << loc[i].x << " pl_y:= " << loc[i].y << endl;
                if (ss == 0)  cout << "pl_x:= " << loc[0].x << " pl_y:= " << loc[0].y << endl;
                sgv_t.from = pl_t.id;
                sgv_t.to = ss;
                sgv.push_back(sgv_t);
                num_of_sgv ++;
                ss = pl_t.id;
            }
        }
        else{
            sgw_t.from = num_of_pl;
            sgw_t.to = cross_point[node[i].lchild];
            sgw.push_back(sgw_t);
            num_of_sgw ++;
            sgw_t.to = cross_point[node[i].rchild];
            sgw.push_back(sgw_t);
            num_of_sgw ++;

            pl_t.id = num_of_pl;
            pl_t.x = loc[i].x;
            pl_t.y = loc[i].y;
            pl.push_back(pl_t);
            num_of_pl++;
            for (int i_v=0; i_v < node[i].nv; i_v++){
                pl_t.id = num_of_pl++;
                pl_t.x = loc[i].x;
                pl_t.y = loc[i].y;
                pl.push_back(pl_t);

                sgv_t.from = pl_t.id;
                sgv_t.to = pl_t.id -1;
                sgv.push_back(sgv_t);
                num_of_sgv ++;
                //ss = pl_t.id;
            }
        }
        // if detour occurs in the edge i
        if (node[i].detour > 0){
            sgw_t.to = pl_t.id;
            sgw_t.from = num_of_pl;
            sgw.push_back(sgw_t);
            num_of_sgw++;
            pl_t.id = num_of_pl ++;
            pl_t.x = loc[i].x+node[i].detour/DETOUR_COF ;
            //pl_t.x = loc[i].x+node[i].detour/node[i].detour ;
            //pl_t.x = loc[i].x+1;
            pl_t.y = loc[i].y+1;
            pl.push_back(pl_t);

            sgw_t.to = pl_t.id;
            sgw_t.from = num_of_pl;
            sgw.push_back(sgw_t);
            num_of_sgw++;
            pl_t.id = num_of_pl ++;
            pl_t.x = loc[i].x;
            pl_t.y = loc[i].y;
            pl.push_back(pl_t);
        }
        if (num_of_pl == 1312)    cout << "detour ERROR: " << i << endl;
        // 1. buffer insertion
        double x1, y1, x2, y2, x3, y3;
        double a, b, k;
        x1 = loc[i].x;    y1 = loc[i].y;
        if (i == num_of_nodes-1){
            x2 = 0; y2 = 0;
        }
        else{
            x2 = loc[node[i].parent].x;    y2 = loc[node[i].parent].y;
        }
        //double e_u = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
        double e_u = MD(x1,y1,x2,y2);
        //  debug for edge length
        f_d_el << e_u << "   " << node[i].edge<<endl;
        double e_t = e_u - node[i].lmt;
        if (i == 529)    cout << e_t << endl;
        int nb_t = 0;
        // f_debug
        if (e_t < 0 ){
            x3 = loc[i].x;
            y3 = loc[i].y;
            nb_t = 0;
        }
        else{
            int flag = 0;
            //if (x2-x1 == 0)    flag =1;
            //else k = (y2-y1) / (x2-x1);
            while (e_t >= 0){
                //a = (e_u - e_t)*(e_u-e_t);
                //b = e_t * e_t;
                a = e_u - e_t;
                b = e_t;
                if (flag == 0){
                    //x3 = (a-b+y2*y2-y1*y1+x2*x2-x1*x1+(2*k*x1-2*y1)*(y2-y1)) / (2*k*(y2-y1)+2*(x2-x1));
                    //y3 = k*(x3-x1)+y1;
                    //x3 = x2 - sqrt(b)/(sqrt(a)+sqrt(b))*(x2-x1);
                    //y3 = y2 - sqrt(b)/(sqrt(a)+sqrt(b))*(y2-y1);
                    if (a+b==0){
                        x3 = loc[i].x;
                        y3 = loc[i].y;
                    }
                    else{
                        x3 = x2 - b/(a+b)*(x2-x1);
                        y3 = y2 - b/(a+b)*(y2-y1);
                    }
                }
                else {
                    x3 = x1;
                    //y3 = (e_u+y1+y2)/2;
                    y3 = (a-b+y2*y2-y1*y1) / (2*(y2-y1));
                }
                // buf_t[num_of_bufs].x = x3;
                // buf_t[num_of_bufs].y = y3;

                //if (i == 529)    cout <<  "x3 = "<< x3 << "   y3: = " << y3 << "  kk="<<k<< endl;

                // add 2 pl, 1 sgw and 1 sgb
                sgw_t.to = pl_t.id;
                sgw_t.from = num_of_pl;
                sgw.push_back(sgw_t);
                num_of_sgw ++;
                pl_t.id = num_of_pl++;
                pl_t.x = x3;
                pl_t.y = y3;
                pl.push_back(pl_t);

                sgb_t.to = pl_t.id;
                sgb_t.from = num_of_pl;
                sgb.push_back(sgb_t);
                num_of_sgb ++;
                pl_t.id = num_of_pl++;
                pl_t.x = x3;
                pl_t.y = y3;
                pl.push_back(pl_t);

                num_of_bufs ++;
                nb_t ++;
                e_t-=max_length;
            }
        }
        if (nb_t != node[i].nb)    cout <<"# of BUFFERs ERROR: " << i <<"    before: "<< node[i].nb << "    after: "<<nb_t<< endl;

        // 2. gate insertion
        if (node[i].g !=0 ){
        // f_debug
        if (i == 2)    cout << "node[].g "  << node[i].g <<endl;
            gate[num_of_gates].x = loc[i].x;
            gate[num_of_gates].y = loc[i].y;
            num_of_gates++;

            // add 2 pl, 1 sgw and 1 sgb
            sgw_t.to = pl_t.id;
            sgw_t.from = num_of_pl;
            sgw.push_back(sgw_t);
            num_of_sgw ++;
            pl_t.id = num_of_pl++;
            pl_t.x = x2;
            pl_t.y = y2;
            pl.push_back(pl_t);

            sgb_t.to = pl_t.id;
            sgb_t.from = num_of_pl;
            sgb.push_back(sgb_t);
            num_of_sgb ++;
            pl_t.id = num_of_pl++;
            pl_t.x = x2;
            pl_t.y = y2;
            pl.push_back(pl_t);
        }

        // if detour occurs in the edge i
        //if (node[i].detour != 0){
        //    sgw_t.to = pl_t.id;
        //    sgw_t.from = num_of_pl;
        //    sgw.push_back(sgw_t);
        //    num_of_sgw++;
        //    pl_t.id = num_of_pl ++;
        //    if ()
        //}

        // add the last edge.
        sgw_t.to = pl_t.id;
        sgw_t.from = num_of_pl;
        sgw.push_back(sgw_t);
        num_of_sgw ++;
                if (x3 == 0 && y3 == 0)    cout << "555 num_of_pl = " << num_of_pl << endl;
        pl_t.id = num_of_pl++;
        if (i != num_of_nodes-1){
            pl_t.x = loc[node[i].parent].x;
            pl_t.y = loc[node[i].parent].y;
        }
        else{
            pl_t.x = 0;
            pl_t.y = 0;
        }
        cross_point[i] = pl_t.id;
        pl.push_back(pl_t);
    }
    /*
    buf = new struct Point[num_of_bufs];
    for (int i=0; i<num_of_nodes; i++){
        buf[i].x = buf_t[i].x;
        buf[i].y = buf_t[i].y;
    }
    */
    //delete [] buf_t;
}

MS CGBDME::isLineCross(int l, int r){
    MS ms_t;
    ms_t.l_x =ms_t.l_y = ms_t.r_x = ms_t.r_y =0;
    ms_t.slope = -2;
    if (trr[l].core.slope == trr[r].core.slope){
        if (node[l].parent == DEBUG_NODE){
            //cout << "is Line Cross-----------1" << endl;
            cout <<trr[l].core.l_x <<endl<< trr[l].core.l_y << endl<< trr[l].core.r_x<< endl <<trr[l].core.r_y << endl << trr[r].core.l_x << endl << trr[r].core.l_y << endl;
        }
        if (isColine(trr[l].core.l_x, trr[l].core.l_y, trr[l].core.r_x, trr[l].core.r_y, trr[r].core.l_x, trr[r].core.l_y)){
            if (node[l].parent == DEBUG_NODE)    cout << "is Line Cross-----------1.1" << endl;
            if (trr[l].core.l_x<trr[r].core.l_x && trr[r].core.r_x< trr[l].core.r_x){
                if (node[l].parent == DEBUG_NODE)    cout << "is Line Cross-----------1.1.1" << endl;
                ms_t = trr[r].core;
            }
            else if (trr[r].core.l_x<trr[l].core.l_x && trr[l].core.r_x< trr[r].core.r_x){
                if (node[l].parent == DEBUG_NODE)    cout << "is Line Cross-----------1.1.2" << endl;
                ms_t = trr[l].core;
            }
        }
    }
    else{
    if (node[l].parent == DEBUG_NODE)    cout << "is Line Cross-----------2" << endl;
        double x1,y1,x2,y2,x3,y3,x4,y4;
        double b1,b2, x,y;
        x1 = trr[l].core.l_x; y1 = trr[l].core.l_y;
        x2 = trr[l].core.r_x; y2 = trr[l].core.r_y;
        x3 = trr[r].core.l_x; y3 = trr[r].core.l_y;
        x4 = trr[r].core.r_x; y4 = trr[r].core.r_y;
        if (isIntersection(x1,y1,x2,y2,x3,y3,x4,y4)){
    if (node[l].parent == DEBUG_NODE)    cout << "is Line Cross-----------3" << endl;
            if (trr[l].core.slope == 1){
                b1 = y1 - x1;
                b2 = y3 + x3;
            }
            else{
                b1 = y3 - x3;
                b2 = y1 + x1;
            }
            y = (b1+b2)/2;
            x = (b2-b1)/2;
            ms_t.slope = 1;
            ms_t.l_x = ms_t.r_x = x;
            ms_t.l_y = ms_t.r_y = y;
        }
    }
    return ms_t;
}

void::CGBDME::calcInitEL(int l, int r){
    md = lineDist(l, r);
    /*
    md = MAX_VALUE;
    double mdt[4];
    mdt[0] = MD(trr[l].core.l_x, trr[l].core.l_y, trr[r].core.l_x, trr[r].core.l_y);
    mdt[1] = MD(trr[l].core.l_x, trr[l].core.l_y, trr[r].core.r_x, trr[r].core.r_y);
    mdt[2] = MD(trr[l].core.r_x, trr[l].core.r_y, trr[r].core.l_x, trr[r].core.l_y);
    mdt[3] = MD(trr[l].core.r_x, trr[l].core.r_y, trr[r].core.r_x, trr[r].core.r_y);
    if (node[l].parent == DEBUG_NODE)    cout <<"md calc"<< trr[l].core.l_x << endl << trr[l].core.l_y << endl <<trr[l].core.r_x << endl <<trr[l].core.r_y << endl<< trr[r].core.l_x << endl<< trr[r].core.l_y << endl<<trr[r].core.r_x << endl << trr[r].core.r_y << endl;
    for (int i=0; i<4; i++){
        if (mdt[i] < md)    md = mdt[i];
    }
    */
    double t1 = node[l].t;
    double t2 = node[r].t;
    double c1 = node[l].cl_v;
    double c2 = node[r].cl_v;
    double l1, l2;
    double L = md;
    l1  = ((t2 - t1) + r0*L*(0.5*c0*L+c2)) / (r0 * (c1+c2+c0*L));
    if (l1>=0 && l1<=L)     l2 = L-l1;
    else if (l1 < 0){
        l1 = 0;
        //l2 = root(0.5*r0*c0, r0*c2, t2-t1);
        node[r].detour = root(0.5*r0*c0, r0*c2, t2-t1) - L;
        l2 = L;
    }
    else {
        l2 = 0;
        //l1 = root(0.5*r0*c0, r0*c1, t1-t2);
        node[l].detour = root(0.5*r0*c0, r0*c1, t1-t2) - L;
        l1 = L;
    }
    el_e = l1;
    er_e = l2;
}

double CGBDME::getLimitLength(int edge){
    // 1. calc num of buffers in this edge
    int cl_index;
    double rb = 0.09466;
    double kb = 30.71;
    double ln9 = 2.1972245773;
    double de,delta_c;

    
    //double lut_clt= node[edge].lut_clt;
    //double lut_ll = node[edge].lut_ll;
    double lut_clt;
    double lut_ll;
    if(edge == -1) {
    	lut_clt=0;
    	lut_ll=0;
    } else {
    	lut_clt= node[edge].lut_clt;
    	lut_ll = node[edge].lut_ll;
    }
   
    //double lut_cv = node[edge].cl_v;
    //if (IS_IBM)    return 100000000;
    de = r0*lut_ll* (0.5*c0*lut_ll+lut_clt)/1000;
    if (de==0) delta_c = 0; else delta_c=(ln9*de-kb)/rb+lut_ll*c0;
    //if (delta_c <0)   delta_c=0;
    //cout <<"id:  " <<edge <<"  "<< de<<"    "<< lut_ll <<endl;
    if (edge == MAX_LENGTH)    return  cl_llmt_lut[36+PA];           // cl = cg;
    //else cl_index = (int) (node[edge].cl_v + PA);
    //else cl_index = (int) (node[edge].cl_v+ de+PA);
    else cl_index = (int) (node[edge].cl_v+ PA+PB*c0*lut_ll );

    //else cl_index = (int) (node[edge].cl_v+PA);
    return cl_llmt_lut[cl_index];
}

void CGBDME::selectSC(int l, int r, int flag){
    double SCv = MAX_VALUE;
    double SCl_e[2], SCr_e[2], USCl, USCr;

    int ll = node[l].lchild;
    int lr = node[l].rchild;
    int rl = node[r].lchild;
    int rr = node[r].rchild;
    flag = 0;
    double el_s, er_s;

    if (flag == 0 ){
        el_s = el;
        er_s = er;
    }
    else{
        el_s = el_i;
        er_s = er_i;
    }

    // calc the value of USCl and USCr
    if (ll == -1 || lr == -1)     USCl =0;
    else{
        if (node[ll].g == 0)
            if (node[lr].g == 0)
                USCl = node[l].SC;
            else
                USCl = cg + node[ll].SC + c0*node[ll].edge + node[ll].nb * cg;
        else
            if (node[lr].g == 0)
                USCl = cg + node[lr].SC + c0*node[lr].edge + node[lr].nb * cg;
            else
                USCl = 2 * cg;
    }
    if (rl == -1 || rr == -1)     USCr =0;
    else {
    if (node[rl].g == 0)
        if (node[rr].g == 0)
            USCr = node[r].SC;
        else
            USCr = cg + node[rl].SC + c0*node[rl].edge + node[rl].nb * cg;
    else
        if (node[rr].g == 0)
            USCr = cg + node[rr].SC + c0*node[rr].edge + node[rr].nb * cg;
        else
            USCr = 2 * cg;
    }

    SCl_e[0] = c0*el_s + node[l].nb*cg + node[l].SC;
    //SCl_e[1] = cg + (c0*el_s + USCl) * node[l].p + 0.5*(c0 * dist(node[l].parent) + cg+(ceil(dist(node[l].parent)/max_length)*cg)) * node[l].ptr + node[l].SC - USCl;
    SCl_e[1] = cg + (c0*el_s +node[l].nb*cg+ USCl) * node[l].p + 0.5*(c0 * dist(node[l].parent) + cg) * node[l].ptr + node[l].SC - USCl;

    SCr_e[0] = c0*er_s + node[r].nb*cg + node[r].SC;
    //SCr_e[1] = cg + (c0*er_s + USCr) * node[r].p + 0.5*(c0 * dist(node[r].parent) + cg+(ceil(dist(node[r].parent)/max_length)*cg)) * node[r].ptr + node[r].SC - USCr;
    SCr_e[1] = cg + (c0*er_s +node[r].nb*cg+ USCr) * node[r].p + 0.5*(c0 * dist(node[r].parent) + cg) * node[r].ptr + node[r].SC - USCr;

    // 4 strategy
    for (int i=0; i<2; i++){
        for (int j=0; j<2; j++){
            if ((SCl_e[i]+SCr_e[j]) < SCv){
                SCv = SCl_e[i] +SCr_e[j];
                node[l].g = i;
                node[r].g = j;
            }
        }
    }
    node[node[l].parent].SC = SCv;
}

void CGBDME::calcAccurateEdge(int l, int r){
    // 1. consider buffer and Llimit
    double c1, c2, t1, t2, lt1, lt2, l1, l2;

    if (node[l].parent == DEBUG_NODE)    cout << " md = " << md << endl;
    double L = md;
    //if (node[l].parent == 354)    cout << "354 L : " << L << endl;
    //if (node[l].parent == 354)    cout << "354:  "<<node[l].nb<<node[r].nb<< " "<<node[l].g << node[r].g << endl;
    if ( node[l].nb == 0 ){
        c1 = node[l].cl_v;
        t1 = node[l].t;
        lt1 = 0;
    }
    else if ( node[l].nb == 1 ){
        c1 = cg;
        t1 = node[l].t + delay_b(node[l].lmt, node[l].cl_v);
        lt1 = node[l].lmt;
    }
    else{
        c1 = cg;
        t1 = node[l].t + (node[l].nb - 1) * delay_b(max_length, cg) + delay_b(node[l].lmt, node[l].cl_v);
        lt1 = (node[l].nb - 1) * max_length + node[l].lmt;
    }

    if ( node[r].nb == 0 ){
        c2 = node[r].cl_v;
        t2 = node[r].t;
        lt2 = 0;
    }
    else if ( node[r].nb == 1 ){
        c2 = cg;
        t2 = node[r].t + delay_b(node[r].lmt, node[r].cl_v);
        lt2 = node[r].lmt;
    }
    else{
        c2 = cg;
        t2 = node[r].t + (node[r].nb - 1) * delay_b(max_length, cg) + delay_b(node[r].lmt, node[r].cl_v);
        lt2 = (node[r].nb - 1) * max_length + node[r].lmt;
    }
    L = L - lt1 - lt2;
    //if (node[l].parent == 202)    cout << "202  L: "<<L << endl;
    if (L<=0)    cout << "WTF-------:" << "id: " << node[l].parent << "L:   "<<L <<endl;
    // 2. 4 strategy of gate insertion
    if (node[l].g == 0 && node[r].g == 0){
        l1  = ((t2 - t1) + r0*L*(0.5*c0*L+c2)) / (r0 * (c1+c2+c0*L));
        if (l1>=0 && l1<=L)     l2 = L-l1;
        else if (l1 < 0){
            l1 = 0;
            //l2 = root(0.5*r0*c0, r0*c2, t2-t1);
            node[r].detour = root(0.5*r0*c0, r0*c2, t2-t1) - L;
            l2 = L;
        }
        else {
            l2 = 0;
            //l1 = root(0.5*r0*c0, r0*c1, t1-t2);
            node[l].detour = root(0.5*r0*c0, r0*c1, t1-t2) - L;
            l1 = L;
        }
    }
    else if(node[l].g == 1 && node[r].g == 0){
        l1 = (t2-t1-rg*cgo-rg*c1 + r0*L*(0.5*c0*L+c2)) / (r0*(c1+c2+c0*L)+rg*c0);
        if (l1 >= 0 && l1 <= L)     l2 = L-l1;
        else if (l1 < 0){
            l1 = 0;
            //l2 = root(0.5*r0*c0, r0*c2, t2-t1-rg*cgo-rg*c1);
            node[r].detour = root(0.5*r0*c0, r0*c2, t2-t1-rg*cgo-rg*c1) - L;
            l2 = L;
        }
        else{
            l2 = 0;
            //l1 = root(0.5*r0*c0, rg*c0+r0*c1, t1+rg*cgo+rg*c1-t2);
            node[l].detour = root(0.5*r0*c0, rg*c0+r0*c1, t1+rg*cgo+rg*c1-t2)- L;
            l1 = L;
        }
    }
    else if (node[l].g == 0 && node[r].g == 1){
        l2 = (t1-t2-rg*cgo-rg*c2 + r0*L*(0.5*c0*L+c1)) / (r0*(c1+c2+c0*L)+rg*c0);
        if (l2 >=0 && l2 <= L)      l1 = L-l2;
        else if (l2 < 0){
            l2 = 0;
            //l1 = root(0.5*r0*c0, r0*c1, t1-t2-rg*cgo-rg*c2);
            node[l].detour = root(0.5*r0*c0, r0*c1, t1-t2-rg*cgo-rg*c2) - L;
            l1 = L;
        }
        else{
            l1 = 0;
            //l2 = root(0.5*r0*c0, rg*c0+r0*c2, t2+rg*cgo+rg*c2-t1);
            node[r].detour = root(0.5*r0*c0, rg*c0+r0*c2, t2+rg*cgo+rg*c2-t1) -L;
            l2 = L;
        }
    }
    else{
        l1 = ((t2+rg*c2)-(t1+rg*c1) + r0*L*(0.5*c0*L+c2) + rg*c0*L) / (r0*(c1+c2+c0*L)+2*rg*c0);
        //if (node[l].parent == 354 )    cout << "354 L1: " << l1 << endl;
        if (l1>=0 && l1 <=L)        l2 = L-l1;
        else if (l1 < 0){
            l1 = 0;
            //l2 = root(0.5*r0*c0, r0*c2+rg*c0, (t2+rg*c2)-(t1+rg*c1));
            node[r].detour = root(0.5*r0*c0, r0*c2+rg*c0, (t2+rg*c2)-(t1+rg*c1))-L;
            l2 = L;
        }
        else{
            l2 = 0;
            //l1 = root(0.5*r0*c0, r0*c1+rg*c0, (t1+rg*c1)-(t2+rg*c2));
            node[l].detour = root(0.5*r0*c0, r0*c1+rg*c0, (t1+rg*c1)-(t2+rg*c2))-L;
            l1 = L;
        }
    }
    //if(node[l].parent == 354 )    cout << "202 lt1: " << lt1 << "   lt2: "<< lt2 << " l1: " << l1 << "   l2: "<< l2 << endl;
    el = l1 + lt1;
    er = l2 + lt2;
}

double CGBDME::lineDist(int l, int r){
    double md_l = MAX_VALUE;
    double x1 = trr[l].core.l_x;
    double y1 = trr[l].core.l_y;
    double x2 = trr[l].core.r_x;
    double y2 = trr[l].core.r_y;
    double x3 = trr[r].core.l_x;
    double y3 = trr[r].core.l_y;
    double x4 = trr[r].core.r_x;
    double y4 = trr[r].core.r_y;
    if (min(x1,x2)<max(x3,x4) && min(x3,x4)<max(x1,x2) &&      // is retangle cross?
            min(y1,y2)<max(y3,y4) && min(y3,y4)<max(y1,y2)){
            if (node[l].parent==DEBUG_NODE)    cout<< "lineCROSS_vertical" << endl;
        if (trr[l].core.slope == trr[r].core.slope){            // parallel
            if (trr[l].core.slope == 1)
                md_l = abs(y1-y3-(x1-x3));
            else
                md_l = abs(y1-y3+x1-x3);
        }
        else{                                                   // vertical
            if (node[l].parent==DEBUG_NODE)    cout<< "lineCROSS_vertical" << endl;
            double b1, b2;
            double mdt[4];
            double yt;
            if (trr[l].core.slope == 1){
                b1 = y1-x1; b2 = y3+x3;
                yt = -x1+b2;    mdt[0] = abs(y1-yt);
                yt = -x2+b2;    mdt[1] = abs(y2-yt);
                yt = x3+b1;    mdt[2] = abs(y3-yt);
                yt = x4+b1;    mdt[3] = abs(y4-yt);
            }
            else{
                b1 = y1+x1; b2 = y3-x3;
                yt = x1+b2;    mdt[0] = abs(y1-yt);
                yt = x2+b2;    mdt[1] = abs(y2-yt);
                yt = -x3+b1;    mdt[2] = abs(y3-yt);
                yt = -x4+b1;    mdt[3] = abs(y4-yt);
            }
            for (int i=0; i<4; i++){
                if (mdt[i] < md_l)    md_l = mdt[i];
            }
        }
    }
    else{
        double mdt[4];
        mdt[0] = MD(trr[l].core.l_x, trr[l].core.l_y, trr[r].core.l_x, trr[r].core.l_y);
        mdt[1] = MD(trr[l].core.l_x, trr[l].core.l_y, trr[r].core.r_x, trr[r].core.r_y);
        mdt[2] = MD(trr[l].core.r_x, trr[l].core.r_y, trr[r].core.l_x, trr[r].core.l_y);
        mdt[3] = MD(trr[l].core.r_x, trr[l].core.r_y, trr[r].core.r_x, trr[r].core.r_y);
        for (int i=0; i<4; i++){
            if (mdt[i] < md_l)    md_l = mdt[i];
        }
    }
    return md_l;
}

double CGBDME::MD(double x1, double y1, double x2, double y2){
    double x,y ;
    if (x1-x2 > 0)  x = x1-x2;
    else x =x2-x1;
    if(y1-y2 >0) y =y1-y2;
    else y=y2-y1;
    return (x+y);
}

double CGBDME::dist(int edge){
    //chip_size.ur_x = floor(chip_size.ur_x / sqrt(NUM_OF_LAYERS));
    //chip_size.ur_y = floor(chip_size.ur_y / sqrt(NUM_OF_LAYERS));
    return sqrt((node[edge].x-chip_size.ur_x / 2) * (node[edge].x-chip_size.ur_x /2) + (node[edge].y-chip_size.ur_y /2 ) * (node[edge].y-chip_size.ur_y /2));
}

double CGBDME::delay_w(double length, double c){
    return r0*length*(0.5*c0*length+c);
}

double CGBDME::delay_b(double length, double c){
    return rg * (c0 * length + c) + r0 * length*(0.5 * c0 * length + c) + rg*cgo;
}

double CGBDME::root(double a, double b, double c){
    double out;
    if (b*b-4*a*c<0)  cout << "yuyuyuyyuyuyuyuuyuyuyuyuyuyuyuy"<< endl;
    out = (-1*b + sqrt( b*b - 4*a*c )) / (2*a);
    return out;
}

double CGBDME::min(double x, double y){
    return (x<y)?x:y;
}

double CGBDME::max(double x, double y){
    return (x>y)?x:y;
}

bool CGBDME::isColine(double x1, double y1, double x2, double y2, double x3, double y3){
    double k;
    /*
    if (y3-y1!=0 && y2-y1!=0)
        k = (x2-x1)/(y2-y1) - (x3-x1)/(y3-y1);
    else
    */
        k = (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
    //k1 = (y2-y1)/(x2-x1);
    //k2 = (y3-y1)/(x3-x1);
    if (k>-0.02 && k<0.02)
        return true;
    else return false;
}

bool CGBDME::isIntersection(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4){
    if (min(x1,x2)<=max(x3,x4) && min(x3,x4)<=max(x1,x2) &&
            min(y1,y2)<=max(y3,y4) && min(y3,y4)<=max(y1,y2)){
        double a1b1_x = x1-x3;
        double a1b1_y = y1-y3;
        double b2b1_x = x4-x3;
        double b2b1_y = y4-y3;
        double a2a1_x = x2-x1;
        double a2a1_y = y2-y1;
        double b2a1_x = x4-x1;
        double b2a1_y = y4-y1;
        double a2b1_x = x2-x3;
        double a2b1_y = y2-y3;
        if (xx(a1b1_x,a1b1_y,b2b1_x,b2b1_y)*xx(b2b1_x,b2b1_y,a2b1_x,a2b1_y)>=0){
            a1b1_y = -a1b1_y; a1b1_x = -a1b1_x;
            if (xx(a1b1_x,a1b1_y,a2a1_x,a2a1_y)*xx(a2a1_x,a2a1_y,b2a1_x,b2a1_y)>=0){
                return 1;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
}

double CGBDME::xx(double x1,double y1,double x2,double y2){
    return (x1*y2-x2*y1);
}

double CGBDME::abs(double x){
    return (x>0)?x:(-x);
}

void CGBDME::debug_input(){
    ofstream f_d_in("debug_input");
    if (!f_d_in.is_open())    cout << "can't open debug_input" << endl;
    f_d_in << "chip_size: " << chip_size.ll_x << " " << chip_size.ll_y << " " << chip_size.ur_x << " " << chip_size.ur_y << endl;
    f_d_in << "r0_c0_cg_cv: " << r0 << " " << c0 << " " << cg << " " << cv << endl;
    f_d_in << "num_of_nodes: " << num_of_nodes << endl;
    for (int i=267; i<280; i++){
        f_d_in << "id: "<< node[i].id << " " << node[i].x << " " << node[i].y << " " << node[i].z << " " << node[i].cl << " "<< node[i].p << " " << node[i].ptr << " "
               << node[i].parent << " " << node[i].lchild << " " << node[i].rchild << " " << node[i].left_TSV << " " << node[i].right_TSV << endl;
    }
}

void CGBDME::debug_readBufLUT(){
    ofstream f("debug_readBufLUT");
    if (!f.is_open())    cout << "can't open debug_readBufLUT" << endl;
    f << "max_length is : " << max_length << endl;
    f << "this is cl_llmt_lut:" << endl;
}

void CGBDME::debug_loc(){
    ofstream f_loc("debug_loc");
    f_loc.precision(9);
    if(!f_loc.is_open())    cout << "cant open debug_loc  "  << endl;
    for (int i=0; i<num_of_nodes; i++){
        f_loc <<"id: " << i+1 << "  " << loc[i].x << " " << loc[i].y << endl;
    }
    cout << "node: 189 "<<loc[189].x <<" "<<loc[189].y <<" "<<node[189].cl_v << endl;
}

void CGBDME::debug_detour(){
    ofstream f_detour("debug_detour");
    f_detour.precision(9);
    if(!f_detour.is_open())    cout << "cant open debug_detour" << endl;
    for (int i=0; i<num_of_nodes;i++){
        f_detour << "idL " << i+1 << " " << node[i].detour << endl;
    }
}

void CGBDME::debug_iter_comp(){
    ofstream f_iter_comp("debug_iter_comp");
    f_iter_comp.precision(9);
    if(!f_iter_comp.is_open())    cout << "cant open debug_iter_comp" << endl;
    f_iter_comp << "num_sinks: " << num_of_sinks << endl;
    cout << "num_sinks:" << num_of_sinks << endl;
    f_iter_comp << "num_nodes: " << num_of_nodes << endl;
    cout << "num_nodes:" << num_of_nodes << endl;
    f_iter_comp << "iterative number: " << iterative_complexity << endl;
    cout << "iterative number: " << iterative_complexity << endl;
}
