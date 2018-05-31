/*************************************************************************
  > Copyright (C), 2015-2016, Kimura Lab., IPS, Waseda Univ.
  > All rights reserved.
  > @File:         input.cpp
  > @Author:       Minghao Lin
  > @E-mail:       mhlin@ruri.waseda.jp
  > @Version:      1.0.0
  > @Date:         2015/12/27 18:21:49
  > @Description:
  > @History:      ----------------------------------
  >                date:         2015/12/27 18:21:49
  >                author:       Minghao Lin
  >                modification: Create
  >                ----------------------------------
  >                date:
  >                author:
  >                modification:
  >                ----------------------------------
  > @Others:
 ************************************************************************/
#include "inputo.h"
void CInput::setInputData(string file_path, int benchmark){
    ifstream file(file_path.c_str());
    if (!file.is_open())    cout << "file is not exist!" << endl;
    string buf;         // buffer, store redundant msgs
    if (benchmark == ISPD){
        file >> chip_size.ll_x >> chip_size.ll_y >> chip_size.ur_x >> chip_size.ur_y;
        file >> buf >> source.source_name >> source.x >> source.y >> source.buf_name;
        file >> buf >> buf >> num_of_sinks;

        sink = new struct Sink[2*num_of_sinks-1];
        for (int i=0; i<num_of_sinks; i++){
            file >> sink[i].id >> sink[i].x >> sink[i].y >> sink[i].cl;
			sink[i].id -- ;
        }

        file >> buf >> buf >> num_of_wirelib;
        wirelib = new struct Wirelib[num_of_wirelib];
        for (int i=0; i<num_of_wirelib; i++){
            file >> wirelib[i].id >> wirelib[i].r0 >> wirelib[i].c0;
        }
        file >> buf >> buf >> num_of_buflib;
        buflib = new struct Buflib[num_of_buflib];
        for (int i=0; i<num_of_buflib; i++){
            file >> buflib[i].id >> buflib[i].spice_file >> buflib[i].is_inv >> buflib[i].in_cl >> buflib[i].out_cl >> buflib[i].out_rl;
        }
        file >> buf >> buf >> vdd[0] >> vdd[1];
        file >> buf >> buf >> lmt_slew;
        file >> buf >> buf >> lmt_cap;
        file >> buf >> buf >> num_of_blockage;
        block = new struct Blockage[num_of_blockage];
        for (int i=0; i<num_of_blockage; i++){
            block[i].id = i;
            file >> block[i].ll_x >> block[i].ll_y >> block[i].ur_x >> block[i].ur_y;
        }
        file >> buf >> num_of_inst;

		rw = wirelib[1].r0;
        cw = wirelib[1].c0;
    }
    else if (benchmark == IBM){
        file >> chip_size.ll_x >> chip_size.ll_y >> chip_size.ur_x >> chip_size.ur_y;
        file >> buf >> buf >> num_of_sinks;
        file >> buf >> buf >> r0;
        file >> buf >> buf >> c0;
        c0 = c0*pow(10,15);
        sink = new struct Sink[2*num_of_sinks-1];
        for (int i = 0; i < num_of_sinks; i++){
            string buf;
            file >> buf >> buf >> sink[i].id;
            file >> buf >> buf >> sink[i].x >> sink[i].y;
            file >> buf >> buf >> buf >> sink[i].cl;
            sink[i].cl = sink[i].cl * pow(10,15);
        }
        file >> buf >> num_of_inst;

		rw = 0.003;
        cw = c0;

    }
    else{
        cout << "error: there is no such benchmark!" << endl;
    }
	for (int i=0; i<2*num_of_sinks-1; i++){
		sink[i].level = 0;
	}
    cv = TSV_CAP;
    cg = GATE_CAP;
    file.close();
}

void CInput::modiFile(int num_of_layers){
	ofstream f_mf("circuit");
	f_mf.precision(10);
    f_mf << chip_size.ll_x <<" "<< chip_size.ll_y << " " <<floor(chip_size.ur_x / sqrt(num_of_layers)) <<" "<< floor(chip_size.ur_y /sqrt(num_of_layers))<< endl;
	f_mf << "source 0 0 0 0"<< endl;
	f_mf << "num sink " << num_of_sinks << endl;
	for (int i=0; i<num_of_sinks; i++){
		f_mf << i+1 << " " << floor(sink[i].x/sqrt(num_of_layers)) <<" "<< floor(sink[i].y/sqrt(num_of_layers)) << " " << sink[i].cl << endl;
	}
	f_mf << "num wirelib " << num_of_wirelib<<endl;
	for (int i=0; i<num_of_wirelib; i++){
		f_mf<< i << " " << wirelib[i].r0 <<" "<< wirelib[i].c0 << endl;
	}
	f_mf << "num buflib " << num_of_buflib << endl;
	for (int i=0;i<num_of_buflib ; i++){
		f_mf<< i <<" clkinv"<<i <<".subckt "<<buflib[i].is_inv << " " << buflib[i].in_cl << " " << buflib[i].out_cl << " "<<  buflib[i].out_rl << endl;
	}
	f_mf <<"simulation vdd " << vdd[0] <<" " <<vdd[1]<< endl;
	f_mf <<"limit slew " << lmt_slew <<endl;
	f_mf << "limit cap " << lmt_cap<< endl;
	f_mf << "num blockage " << num_of_blockage << endl;
	for (int i=0;i<num_of_blockage; i++){
		f_mf << floor(block[i].ll_x/sqrt(num_of_layers)) << " " << floor(block[i].ll_y/sqrt(num_of_layers))<< " "<< floor(block[i].ur_x/sqrt(num_of_layers)) << " " << floor(block[i].ur_y /sqrt(num_of_layers))<< endl;
	}
     
}

void CInput::divideCircuit(int num_of_layers, bool is_load){
	chip_size.ur_x = floor(chip_size.ur_x / sqrt(num_of_layers));
    chip_size.ur_y = floor(chip_size.ur_y / sqrt(num_of_layers));
	if (!is_load){
		srand( (unsigned)time( NULL ) );      // here should use floor
		ofstream f_div_o("divided_cir");
		f_div_o.precision(10);
		for (int i=0; i<num_of_sinks; i++){
			sink[i].x = floor(sink[i].x / sqrt(num_of_layers));
			sink[i].y = floor(sink[i].y / sqrt(num_of_layers));
			sink[i].z = rand() % num_of_layers;
			f_div_o << sink[i].x << " "<< sink[i].y << " " << sink[i].z << endl;
		}
		source.z = rand() % num_of_layers;
		for (int i=0; i<num_of_blockage; i++){
			block[i].ll_x = floor(block[i].ll_x / sqrt(num_of_layers));
			block[i].ll_y = floor(block[i].ll_y / sqrt(num_of_layers));
			block[i].ur_x = floor(block[i].ur_x / sqrt(num_of_layers));
			block[i].ur_y = floor(block[i].ur_y / sqrt(num_of_layers));
			block[i].z = rand() % num_of_layers;
		}
         
	}
	else{
		ifstream f_div_in("divided_cir");
		f_div_in.precision(10);
		if (!f_div_in.is_open())    cout << "file is not exist!" << endl;
		for (int i=0; i<num_of_sinks; i++){
			f_div_in >> sink[i].x >> sink[i].y >> sink[i].z;
		}
		f_div_in.close();
	}
}

void CInput::debug(){
    /*
       if (benchmark == ISPD){
       cout << "limit slew: " << lmt_slew << endl;
       cout << "num of block: " << num_of_blockage << endl;
       cout << "sink[2]: " << sink[2].x << endl;
       }
       else if (benchmark == IBM){
       cout << "num of sinks: "<< num_of_sinks << endl;
       cout << "r0: "<< r0 << endl;
       cout << "c0: "<< c0 << endl;
       cout << sink[0].id << endl;
       cout << sink[0].x << endl;
       cout << sink[0].y << endl;
       cout << sink[0].cl << endl;
       }
       */

    cout << endl << "layers of sinks" << endl;
    for (int i=0; i<4; i++){
        cout << sink[i].z << endl;
    }
    cout << endl << "layers of blocks" << endl;
    for (int i=0; i<num_of_blockage; i++){
        cout << block[i].z << endl;
    }

    deleteArray();
}

void CInput::generateAP(bool is_load){
    cap.init(num_of_sinks, num_of_inst);
    cout<<"hello2"<<endl;
    if (is_load)    AP = cap.loadAP();
    else            AP = cap.getAP(IS_WRITE_FILE);
    cout<<"hello3"<<endl;
    for (int i=0; i<num_of_sinks; i++){
        for (int j=0; j<INST_LENGTH; j++){
            sink[i].ap[j] = AP[i][j];
        }
        sink[i].p = cap.getP(sink[i].ap);
        sink[i].ptr = cap.getPtr(sink[i].ap);
    }
    cout << "generate AP successfully!" << endl;
}

void CInput::NNS(){
    tree_node = new struct InitTopoTree[2*num_of_sinks-1];
    for (int i=0; i<num_of_sinks; i++){
        tree_node[i].lchild = tree_node[i].rchild = -1;
        tree_node[i].data = i;
    }
    list <int> remain(num_of_sinks, 0);
    list <int>::iterator r_it;
    int inc_sink = 0;
    for (r_it = remain.begin(); r_it != remain.end(); r_it++){
        *r_it = sink[inc_sink++].id;
    }
    MergingCell t_mc;
    for (int i=0; i<num_of_sinks; i++){
        for (int j=i+1; j<num_of_sinks; j++){
            t_mc.a = i; t_mc.b = j;
            t_mc.merging_cost = getMergingCost(i, j);
            mc.push_back(t_mc);
        }
    }

    list <MergingCell>::iterator mc_it;

    /*         set info. of sinks bellows:              */
    mc.sort(cmpSort());
    ofstream f_sink("sink_info1");
    if (!f_sink.is_open())      cout << "can't open sink_info" << endl;
    f_sink << "num_of_sinks: " << num_of_sinks << endl;

    for (mc_it = mc.begin(); mc_it != mc.end(); mc_it++){
        f_sink << "merging left child, a = " << (*mc_it).a << endl;
        f_sink << "merging right child, b = " << (*mc_it).b << endl;
        f_sink << "merging cost = " << (*mc_it).merging_cost << endl;
    }
    f_sink << sink[213].z << " and " << sink[255].z << endl;
    f_sink.close();
    cout << "set info. of sinks successfully!" << endl;
    /*          ***********************             */

    int inner;
    inner = num_of_sinks;

    ofstream f_remain("remain_list");
    if (!f_remain.is_open())        cout << "so sad" << endl;

    for (int cycle_index = 0; cycle_index < num_of_sinks -1 ; cycle_index++ ){
        mc.sort(cmpSort());                       // sort mc
        int min_a = (*mc.begin()).a;
        int min_b = (*mc.begin()).b;

        /*              f_remain file               */
        f_remain << "cycle: " << inner << endl;
        int format = 0;
        for (r_it = remain.begin(); r_it != remain.end(); r_it++){
            f_remain << " " << (*r_it);
            if (format == 30){
                f_remain << endl;
                format = 0;
            }
            else    format ++;
        }
        f_remain << endl ;
        f_remain << "min_a: " << min_a << endl;
        f_remain << "min_b: " << min_b << endl;
        f_remain << endl ;
        //f_remain.close();
        /*              ************                */

        tree_node[inner].lchild = min_a;
        tree_node[inner].rchild = min_b;
        tree_node[min_a].parent = tree_node[min_b].parent = inner; // construct init topo tree
        tree_node[inner].data = inner;

        list <MergingCell>::iterator t_mc_it;
        for (mc_it = mc.begin(); mc_it != mc.end(); ){
            t_mc_it = mc_it++;
            if ( ((*t_mc_it).a == min_a) ||((*t_mc_it).a == min_b) ||((*t_mc_it).b == min_a) ||((*t_mc_it).b == min_b) ){
                //mc.remove(*mc_it);
                mc.erase(t_mc_it);
            }
        }

        //mc.remove_if(cmpRemove());
        list <int>::iterator t_r_it;
        for (r_it = remain.begin(); r_it != remain.end(); ){
            t_r_it = r_it ++;
            if (*t_r_it == min_a || *t_r_it == min_b )
                remain.erase(t_r_it);
        }
        //remain.remove(min_a);
        //remain.remove(min_b);

        mergeNode(inner, min_a, min_b);
 

        for (r_it = remain.begin(); r_it != remain.end(); r_it++){
            t_mc.a = inner; t_mc.b = *r_it;
            t_mc.merging_cost = getMergingCost(inner, *r_it);
            mc.push_back(t_mc);
        }
        remain.push_back(inner);
        inner++;
        
         
          /*set info. of sinks bellows:     */   
    
    
    if(inner==2*num_of_sinks-1)   { 
    mc.sort(cmpSort());
    ofstream f_sink2("sink_info3");
    if (!f_sink2.is_open())      cout << "can't open sink_info" << endl;
    f_sink2 << "num_of_sinks: " << num_of_sinks << endl;

    for (mc_it = mc.begin(); mc_it != mc.end(); mc_it++){
        f_sink2 << "merging left child, a = " << (*mc_it).a << endl;
        f_sink2 << "merging right child, b = " << (*mc_it).b << endl;
        f_sink2 << "merging cost = " << (*mc_it).merging_cost << endl;
    }
   
    f_sink2.close();
    cout<<"set info3. of sinks successfully!"<<endl;}
    
    /*          ***********************             */
    
  }

   f_remain.close();
}

void CInput::outputInitTree(int traverse_type){
    cout << "here i come"  << endl;
    f_itree.open("Init_Topo_Tree");
    switch (traverse_type){
        case PRE_ORDER:{
            f_itree << "pre-order travel of init topology tree: " << endl;
            preOrder(tree_node[2*num_of_sinks-2]);
            break;
        }
        case IN_ORDER:{
            f_itree << "in-order travel of init topology tree: " << endl;
            inOrder(tree_node[2*num_of_sinks-2]);
            break;
        }
        case POST_ORDER:{
            f_itree << "post-order travel of init topology tree: " << endl;
            postOrder(tree_node[2*num_of_sinks-2]);
            break;
        }
        default: break;
    }
    f_itree.close();
}

void CInput::preOrder(InitTopoTree root){
    f_itree << root.data << endl;
    if (root.lchild == -1) return;
    preOrder(tree_node[root.lchild]);
    preOrder(tree_node[root.rchild]);
}

void CInput::inOrder(InitTopoTree root){
    if (root.lchild == -1){f_itree << root.data << endl; return;}
    inOrder(tree_node[root.lchild]);
    f_itree << root.data << endl;
    inOrder(tree_node[root.rchild]);
}

void CInput::postOrder(InitTopoTree root){
    if (root.lchild == -1){f_itree << root.data << endl; return;}
    postOrder(tree_node[root.lchild]);
    postOrder(tree_node[root.rchild]);
    f_itree << root.data << endl;
}

double CInput::getMergingCost(int a, int b){
    double GI = cw * distance(sink[a].x, sink[a].y, sink[b].x, sink[b].y);
    double LI = cv * (abs(sink[a].z - sink[b].z));
    double BI = (sink[a].cl*sink[a].p+sink[b].cl*sink[b].p) + 0.5*cg*(sink[a].ptr+sink[b].ptr);
    //return cw*distance(a.x,a.y,b.x,b.y) + cv*abs(a.z-b.z) + (a.cl*a.p+b.cl*b.p) + cg*(a.ptr+b.ptr);
    //return (GI + LI + BI + ALFA_CL*(sink[a].cl+sink[b].cl)*abs(sink[a].level + sink[b].level));
	//return GI;
	//return GI*abs(sink[a].cl-sink[b].cl);
	return (GI+LI+BI);
}

double CInput::distance(double a_x,double a_y,double b_x,double b_y){
    return abs(a_x-b_x)+abs(a_y-b_y);
}

double CInput::abs(double x){
    return x>0?x:(-x);
}

double CInput::halfRound(double x){
    double t_x = floor(x);
    if ((x-t_x) == 0.25) return t_x;
    else if ((x-t_x) == 0.75) return (t_x+1);
    else return x;
}

void CInput::mergeNode(int inner, int a, int b ){
    int * t_ap = sink[inner].ap;
	double x1,x2,y1,y2,cx1,cx2,cy1,cy2;
	if (sink[a].x <= sink[b].x) {x1=sink[a].x;x2=sink[b].x;cx1=sink[a].cl;cx2=sink[b].cl;}
	else {x1=sink[b].x;x2=sink[a].x;cx1=sink[b].cl;cx2=sink[a].cl;}
	if (sink[a].y <= sink[b].y) {y1=sink[a].y;y2=sink[b].y;cy1=sink[a].cl;cy2=sink[b].cl;}
	else {y1=sink[b].y;y2=sink[a].y;cy1=sink[b].cl;cy2=sink[a].cl;}
    //int * t_ap_a = sink[a].ap;
    //int * t_ap_b = sink[b].ap;
    sink[inner].id = inner;
    sink[inner].x = floor( (sink[a].x+sink[b].x) / 2 );
    sink[inner].y = floor( (sink[a].y+sink[b].y) / 2 );
	//sink[inner].x = floor( (cx2/cx1*x2+x1)/(1+cx2/cx1) );
    //sink[inner].y = floor( (cy2/cy1*y2+y1)/(1+cy2/cy1) );
    sink[inner].z = halfRound( (sink[a].z+sink[b].z) / 2 );
    sink[inner].cl = sink[a].cl + sink[b].cl;
    //sink[inner].ap = cap.getAP(sink[a].ap, sink[b].ap);
    //sink[inner].ap = cap.getAP(*t_ap_a, *t_ap_b);
    t_ap = cap.getAP(sink[a].ap, sink[b].ap);
    //sink[inner].p = cap.getP(sink[inner].ap);
    for (int i=0; i<INST_LENGTH; i++)    sink[inner].ap[i] = t_ap[i];
	sink[inner].p = cap.getP(t_ap);
    //sink[inner].ptr = cap.getPtr(sink[inner].ap);
    sink[inner].ptr = cap.getPtr(t_ap);
	//ofstream f_d_ap("Inner_AP");
	//cout << "id: " << inner <<" " << *t_ap << endl << "p: " << sink[inner].p <<" ptr: " << sink[inner].ptr << endl << endl;
	sink[inner].level++;
}

void CInput::DLE(){
    // bottom_up phase
    for (int i=0; i<num_of_sinks; i++){
        tree_node[i].left_TSV = tree_node[i].right_TSV = 0;
        tree_node[i].l = sink[i].z;
        tree_node[i].el.insert(tree_node[i].l);
    }
    for (int i = num_of_sinks; i<2*num_of_sinks-1; i++){
        set <int> a_el = tree_node[tree_node[i].lchild].el;
        set <int> b_el = tree_node[tree_node[i].rchild].el;
        set <int> v_el = tree_node[i].el;
        set_intersection(a_el.begin(), a_el.end(), b_el.begin(), b_el.end(),
                insert_iterator < set <int> > (v_el,v_el.begin()) );
        if (v_el.empty()){
            int l_margin = min (*a_el.end(), *b_el.end());
            int r_margin = max (*a_el.begin(), *b_el.begin());
            for (int i=l_margin; i<=r_margin; i++){
                v_el.insert(i);
            }
        }
    }

    // top_down phase
    for (int i=2*num_of_sinks-2; i>=num_of_sinks; i--){
        if (i == 2*num_of_sinks-2){
            tree_node[i].l = *tree_node[i].el.begin();
        }
        else{
            int par = tree_node[i].parent;
            if (tree_node[par].l < *tree_node[i].el.begin())
                tree_node[i].l = *tree_node[i].el.begin();
            else if (tree_node[par].l > *tree_node[i].el.end())
                tree_node[i].l = *tree_node[i].el.end();
            else
                tree_node[i].l = tree_node[par].l;
        }
    }

    // set left_TSV and right_TSV
    for (int i=num_of_sinks; i<2*num_of_sinks-1; i++){
        tree_node[i].left_TSV = abs (tree_node[i].l - tree_node[tree_node[i].lchild].l);
        tree_node[i].right_TSV = abs (tree_node[i].l - tree_node[tree_node[i].rchild].l);
    }
}

void CInput::outputInsertedTopoTree(){
    ofstream f_stree("inserted_tree");
    f_stree << "Inserted topo tree: " << endl;
    for (int i=0; i<2*num_of_sinks-1; i++){
        f_stree << "data: " << tree_node[i].data << endl;
        f_stree << "parent: " << tree_node[i].parent << endl;
        f_stree << "lchild: " << tree_node[i].lchild << "    rchild: "<< tree_node[i].rchild << endl;
        f_stree << "left_TSV: " << tree_node[i].left_TSV << "     right_TSV: " << tree_node[i].right_TSV << endl;
        f_stree << "layer: " << tree_node[i].l << endl;
        f_stree << endl;
    }
}

void CInput::output4DME(){
    ofstream f_DME("DME_inputfile");
	f_DME.precision(10);
    f_DME << "This is a output file of the first stage topo tree generation, and it is the input file of the second stage 3D-DB-DME." << endl;
    //f_DME << "chipsize(ll_x, ll_y, ur_x, ur_y), wirelib(r0, c0), buflib(is_inv, in_cl, out_cl, out_rl) " << endl;
    f_DME << "chipsize(ll_x, ll_y, ur_x, ur_y), wirelib(r0, c0), buf(input_cap), TSV(cap) " << endl;
    f_DME << chip_size.ll_x << " " << chip_size.ll_y << " " << chip_size.ur_x << " " << chip_size.ur_y << " ";
    f_DME << rw << " " << cw << " ";
    //f_DME << buflib[0].is_inv << " " << buflib[0].in_cl << " " << buflib[0].out_cl << " " << buflib[0].out_rl << endl;
    f_DME << cg << " " << cv << endl;
    f_DME << "id, x,y,z, Cl, p,ptr, parent, lchild, rchild, left_TSV, right_TSV" << endl;
    f_DME << "num_of_nodes: " << 2*num_of_sinks-1 << endl;
    for (int i=0; i<2*num_of_sinks-1; i++){
        f_DME << sink[i].id << " " << sink[i].x << " " << sink[i].y << " " << sink[i].z << " "
              << sink[i].cl << " " << sink[i].p << " " << sink[i].ptr << " "
              << tree_node[i].parent << " " << tree_node[i].lchild << " " << tree_node[i].rchild << " "
              << tree_node[i].left_TSV << " " << tree_node[i].right_TSV << endl;
    }
}
int CInput::max(int x, int y){
    return (x>y)?x:y;
}

int CInput::min(int x, int y){
    return (x>y)?y:x;
}


void CInput::deleteArray(){
    delete [] sink;
    for (int i=0; i<num_of_sinks; i++){
        delete [] AP[i];
    }
    delete [] AP;
    delete [] tree_node;
}
