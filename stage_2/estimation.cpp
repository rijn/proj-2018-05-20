#define r0 100
#define c0 30
#define alfa 0.3

#include <math.h>
double ea, eb;
void estimation(double MD_ab, double ta, double tb, double Cla, double Clb){
    ea = MD_ab / (1 + alfa * sqrt(tb/ta) + (1-alfa) * (Clb/Cla));
    eb = MD_ab - ea;
}
