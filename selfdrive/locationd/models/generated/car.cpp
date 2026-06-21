#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_8830503593500460991) {
   out_8830503593500460991[0] = delta_x[0] + nom_x[0];
   out_8830503593500460991[1] = delta_x[1] + nom_x[1];
   out_8830503593500460991[2] = delta_x[2] + nom_x[2];
   out_8830503593500460991[3] = delta_x[3] + nom_x[3];
   out_8830503593500460991[4] = delta_x[4] + nom_x[4];
   out_8830503593500460991[5] = delta_x[5] + nom_x[5];
   out_8830503593500460991[6] = delta_x[6] + nom_x[6];
   out_8830503593500460991[7] = delta_x[7] + nom_x[7];
   out_8830503593500460991[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8937646500386039140) {
   out_8937646500386039140[0] = -nom_x[0] + true_x[0];
   out_8937646500386039140[1] = -nom_x[1] + true_x[1];
   out_8937646500386039140[2] = -nom_x[2] + true_x[2];
   out_8937646500386039140[3] = -nom_x[3] + true_x[3];
   out_8937646500386039140[4] = -nom_x[4] + true_x[4];
   out_8937646500386039140[5] = -nom_x[5] + true_x[5];
   out_8937646500386039140[6] = -nom_x[6] + true_x[6];
   out_8937646500386039140[7] = -nom_x[7] + true_x[7];
   out_8937646500386039140[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_2651136907830874002) {
   out_2651136907830874002[0] = 1.0;
   out_2651136907830874002[1] = 0.0;
   out_2651136907830874002[2] = 0.0;
   out_2651136907830874002[3] = 0.0;
   out_2651136907830874002[4] = 0.0;
   out_2651136907830874002[5] = 0.0;
   out_2651136907830874002[6] = 0.0;
   out_2651136907830874002[7] = 0.0;
   out_2651136907830874002[8] = 0.0;
   out_2651136907830874002[9] = 0.0;
   out_2651136907830874002[10] = 1.0;
   out_2651136907830874002[11] = 0.0;
   out_2651136907830874002[12] = 0.0;
   out_2651136907830874002[13] = 0.0;
   out_2651136907830874002[14] = 0.0;
   out_2651136907830874002[15] = 0.0;
   out_2651136907830874002[16] = 0.0;
   out_2651136907830874002[17] = 0.0;
   out_2651136907830874002[18] = 0.0;
   out_2651136907830874002[19] = 0.0;
   out_2651136907830874002[20] = 1.0;
   out_2651136907830874002[21] = 0.0;
   out_2651136907830874002[22] = 0.0;
   out_2651136907830874002[23] = 0.0;
   out_2651136907830874002[24] = 0.0;
   out_2651136907830874002[25] = 0.0;
   out_2651136907830874002[26] = 0.0;
   out_2651136907830874002[27] = 0.0;
   out_2651136907830874002[28] = 0.0;
   out_2651136907830874002[29] = 0.0;
   out_2651136907830874002[30] = 1.0;
   out_2651136907830874002[31] = 0.0;
   out_2651136907830874002[32] = 0.0;
   out_2651136907830874002[33] = 0.0;
   out_2651136907830874002[34] = 0.0;
   out_2651136907830874002[35] = 0.0;
   out_2651136907830874002[36] = 0.0;
   out_2651136907830874002[37] = 0.0;
   out_2651136907830874002[38] = 0.0;
   out_2651136907830874002[39] = 0.0;
   out_2651136907830874002[40] = 1.0;
   out_2651136907830874002[41] = 0.0;
   out_2651136907830874002[42] = 0.0;
   out_2651136907830874002[43] = 0.0;
   out_2651136907830874002[44] = 0.0;
   out_2651136907830874002[45] = 0.0;
   out_2651136907830874002[46] = 0.0;
   out_2651136907830874002[47] = 0.0;
   out_2651136907830874002[48] = 0.0;
   out_2651136907830874002[49] = 0.0;
   out_2651136907830874002[50] = 1.0;
   out_2651136907830874002[51] = 0.0;
   out_2651136907830874002[52] = 0.0;
   out_2651136907830874002[53] = 0.0;
   out_2651136907830874002[54] = 0.0;
   out_2651136907830874002[55] = 0.0;
   out_2651136907830874002[56] = 0.0;
   out_2651136907830874002[57] = 0.0;
   out_2651136907830874002[58] = 0.0;
   out_2651136907830874002[59] = 0.0;
   out_2651136907830874002[60] = 1.0;
   out_2651136907830874002[61] = 0.0;
   out_2651136907830874002[62] = 0.0;
   out_2651136907830874002[63] = 0.0;
   out_2651136907830874002[64] = 0.0;
   out_2651136907830874002[65] = 0.0;
   out_2651136907830874002[66] = 0.0;
   out_2651136907830874002[67] = 0.0;
   out_2651136907830874002[68] = 0.0;
   out_2651136907830874002[69] = 0.0;
   out_2651136907830874002[70] = 1.0;
   out_2651136907830874002[71] = 0.0;
   out_2651136907830874002[72] = 0.0;
   out_2651136907830874002[73] = 0.0;
   out_2651136907830874002[74] = 0.0;
   out_2651136907830874002[75] = 0.0;
   out_2651136907830874002[76] = 0.0;
   out_2651136907830874002[77] = 0.0;
   out_2651136907830874002[78] = 0.0;
   out_2651136907830874002[79] = 0.0;
   out_2651136907830874002[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_391547981499403636) {
   out_391547981499403636[0] = state[0];
   out_391547981499403636[1] = state[1];
   out_391547981499403636[2] = state[2];
   out_391547981499403636[3] = state[3];
   out_391547981499403636[4] = state[4];
   out_391547981499403636[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_391547981499403636[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_391547981499403636[7] = state[7];
   out_391547981499403636[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6651794259104444489) {
   out_6651794259104444489[0] = 1;
   out_6651794259104444489[1] = 0;
   out_6651794259104444489[2] = 0;
   out_6651794259104444489[3] = 0;
   out_6651794259104444489[4] = 0;
   out_6651794259104444489[5] = 0;
   out_6651794259104444489[6] = 0;
   out_6651794259104444489[7] = 0;
   out_6651794259104444489[8] = 0;
   out_6651794259104444489[9] = 0;
   out_6651794259104444489[10] = 1;
   out_6651794259104444489[11] = 0;
   out_6651794259104444489[12] = 0;
   out_6651794259104444489[13] = 0;
   out_6651794259104444489[14] = 0;
   out_6651794259104444489[15] = 0;
   out_6651794259104444489[16] = 0;
   out_6651794259104444489[17] = 0;
   out_6651794259104444489[18] = 0;
   out_6651794259104444489[19] = 0;
   out_6651794259104444489[20] = 1;
   out_6651794259104444489[21] = 0;
   out_6651794259104444489[22] = 0;
   out_6651794259104444489[23] = 0;
   out_6651794259104444489[24] = 0;
   out_6651794259104444489[25] = 0;
   out_6651794259104444489[26] = 0;
   out_6651794259104444489[27] = 0;
   out_6651794259104444489[28] = 0;
   out_6651794259104444489[29] = 0;
   out_6651794259104444489[30] = 1;
   out_6651794259104444489[31] = 0;
   out_6651794259104444489[32] = 0;
   out_6651794259104444489[33] = 0;
   out_6651794259104444489[34] = 0;
   out_6651794259104444489[35] = 0;
   out_6651794259104444489[36] = 0;
   out_6651794259104444489[37] = 0;
   out_6651794259104444489[38] = 0;
   out_6651794259104444489[39] = 0;
   out_6651794259104444489[40] = 1;
   out_6651794259104444489[41] = 0;
   out_6651794259104444489[42] = 0;
   out_6651794259104444489[43] = 0;
   out_6651794259104444489[44] = 0;
   out_6651794259104444489[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6651794259104444489[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6651794259104444489[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6651794259104444489[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6651794259104444489[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6651794259104444489[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6651794259104444489[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6651794259104444489[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6651794259104444489[53] = -9.8100000000000005*dt;
   out_6651794259104444489[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6651794259104444489[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6651794259104444489[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6651794259104444489[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6651794259104444489[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6651794259104444489[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6651794259104444489[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6651794259104444489[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6651794259104444489[62] = 0;
   out_6651794259104444489[63] = 0;
   out_6651794259104444489[64] = 0;
   out_6651794259104444489[65] = 0;
   out_6651794259104444489[66] = 0;
   out_6651794259104444489[67] = 0;
   out_6651794259104444489[68] = 0;
   out_6651794259104444489[69] = 0;
   out_6651794259104444489[70] = 1;
   out_6651794259104444489[71] = 0;
   out_6651794259104444489[72] = 0;
   out_6651794259104444489[73] = 0;
   out_6651794259104444489[74] = 0;
   out_6651794259104444489[75] = 0;
   out_6651794259104444489[76] = 0;
   out_6651794259104444489[77] = 0;
   out_6651794259104444489[78] = 0;
   out_6651794259104444489[79] = 0;
   out_6651794259104444489[80] = 1;
}
void h_25(double *state, double *unused, double *out_989584383030050329) {
   out_989584383030050329[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2143693935096995107) {
   out_2143693935096995107[0] = 0;
   out_2143693935096995107[1] = 0;
   out_2143693935096995107[2] = 0;
   out_2143693935096995107[3] = 0;
   out_2143693935096995107[4] = 0;
   out_2143693935096995107[5] = 0;
   out_2143693935096995107[6] = 1;
   out_2143693935096995107[7] = 0;
   out_2143693935096995107[8] = 0;
}
void h_24(double *state, double *unused, double *out_3325235274672278762) {
   out_3325235274672278762[0] = state[4];
   out_3325235274672278762[1] = state[5];
}
void H_24(double *state, double *unused, double *out_7158183428661679347) {
   out_7158183428661679347[0] = 0;
   out_7158183428661679347[1] = 0;
   out_7158183428661679347[2] = 0;
   out_7158183428661679347[3] = 0;
   out_7158183428661679347[4] = 1;
   out_7158183428661679347[5] = 0;
   out_7158183428661679347[6] = 0;
   out_7158183428661679347[7] = 0;
   out_7158183428661679347[8] = 0;
   out_7158183428661679347[9] = 0;
   out_7158183428661679347[10] = 0;
   out_7158183428661679347[11] = 0;
   out_7158183428661679347[12] = 0;
   out_7158183428661679347[13] = 0;
   out_7158183428661679347[14] = 1;
   out_7158183428661679347[15] = 0;
   out_7158183428661679347[16] = 0;
   out_7158183428661679347[17] = 0;
}
void h_30(double *state, double *unused, double *out_8870857033784099514) {
   out_8870857033784099514[0] = state[4];
}
void H_30(double *state, double *unused, double *out_374639023410253520) {
   out_374639023410253520[0] = 0;
   out_374639023410253520[1] = 0;
   out_374639023410253520[2] = 0;
   out_374639023410253520[3] = 0;
   out_374639023410253520[4] = 1;
   out_374639023410253520[5] = 0;
   out_374639023410253520[6] = 0;
   out_374639023410253520[7] = 0;
   out_374639023410253520[8] = 0;
}
void h_26(double *state, double *unused, double *out_3869183766123253869) {
   out_3869183766123253869[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5885197253971051331) {
   out_5885197253971051331[0] = 0;
   out_5885197253971051331[1] = 0;
   out_5885197253971051331[2] = 0;
   out_5885197253971051331[3] = 0;
   out_5885197253971051331[4] = 0;
   out_5885197253971051331[5] = 0;
   out_5885197253971051331[6] = 0;
   out_5885197253971051331[7] = 1;
   out_5885197253971051331[8] = 0;
}
void h_27(double *state, double *unused, double *out_1592900267706527846) {
   out_1592900267706527846[0] = state[3];
}
void H_27(double *state, double *unused, double *out_1800124288390171391) {
   out_1800124288390171391[0] = 0;
   out_1800124288390171391[1] = 0;
   out_1800124288390171391[2] = 0;
   out_1800124288390171391[3] = 1;
   out_1800124288390171391[4] = 0;
   out_1800124288390171391[5] = 0;
   out_1800124288390171391[6] = 0;
   out_1800124288390171391[7] = 0;
   out_1800124288390171391[8] = 0;
}
void h_29(double *state, double *unused, double *out_5230349795960550196) {
   out_5230349795960550196[0] = state[1];
}
void H_29(double *state, double *unused, double *out_884870367724645704) {
   out_884870367724645704[0] = 0;
   out_884870367724645704[1] = 1;
   out_884870367724645704[2] = 0;
   out_884870367724645704[3] = 0;
   out_884870367724645704[4] = 0;
   out_884870367724645704[5] = 0;
   out_884870367724645704[6] = 0;
   out_884870367724645704[7] = 0;
   out_884870367724645704[8] = 0;
}
void h_28(double *state, double *unused, double *out_649390414804865411) {
   out_649390414804865411[0] = state[0];
}
void H_28(double *state, double *unused, double *out_4197528649344884870) {
   out_4197528649344884870[0] = 1;
   out_4197528649344884870[1] = 0;
   out_4197528649344884870[2] = 0;
   out_4197528649344884870[3] = 0;
   out_4197528649344884870[4] = 0;
   out_4197528649344884870[5] = 0;
   out_4197528649344884870[6] = 0;
   out_4197528649344884870[7] = 0;
   out_4197528649344884870[8] = 0;
}
void h_31(double *state, double *unused, double *out_8752849639850722770) {
   out_8752849639850722770[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6511405356204402807) {
   out_6511405356204402807[0] = 0;
   out_6511405356204402807[1] = 0;
   out_6511405356204402807[2] = 0;
   out_6511405356204402807[3] = 0;
   out_6511405356204402807[4] = 0;
   out_6511405356204402807[5] = 0;
   out_6511405356204402807[6] = 0;
   out_6511405356204402807[7] = 0;
   out_6511405356204402807[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_8830503593500460991) {
  err_fun(nom_x, delta_x, out_8830503593500460991);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8937646500386039140) {
  inv_err_fun(nom_x, true_x, out_8937646500386039140);
}
void car_H_mod_fun(double *state, double *out_2651136907830874002) {
  H_mod_fun(state, out_2651136907830874002);
}
void car_f_fun(double *state, double dt, double *out_391547981499403636) {
  f_fun(state,  dt, out_391547981499403636);
}
void car_F_fun(double *state, double dt, double *out_6651794259104444489) {
  F_fun(state,  dt, out_6651794259104444489);
}
void car_h_25(double *state, double *unused, double *out_989584383030050329) {
  h_25(state, unused, out_989584383030050329);
}
void car_H_25(double *state, double *unused, double *out_2143693935096995107) {
  H_25(state, unused, out_2143693935096995107);
}
void car_h_24(double *state, double *unused, double *out_3325235274672278762) {
  h_24(state, unused, out_3325235274672278762);
}
void car_H_24(double *state, double *unused, double *out_7158183428661679347) {
  H_24(state, unused, out_7158183428661679347);
}
void car_h_30(double *state, double *unused, double *out_8870857033784099514) {
  h_30(state, unused, out_8870857033784099514);
}
void car_H_30(double *state, double *unused, double *out_374639023410253520) {
  H_30(state, unused, out_374639023410253520);
}
void car_h_26(double *state, double *unused, double *out_3869183766123253869) {
  h_26(state, unused, out_3869183766123253869);
}
void car_H_26(double *state, double *unused, double *out_5885197253971051331) {
  H_26(state, unused, out_5885197253971051331);
}
void car_h_27(double *state, double *unused, double *out_1592900267706527846) {
  h_27(state, unused, out_1592900267706527846);
}
void car_H_27(double *state, double *unused, double *out_1800124288390171391) {
  H_27(state, unused, out_1800124288390171391);
}
void car_h_29(double *state, double *unused, double *out_5230349795960550196) {
  h_29(state, unused, out_5230349795960550196);
}
void car_H_29(double *state, double *unused, double *out_884870367724645704) {
  H_29(state, unused, out_884870367724645704);
}
void car_h_28(double *state, double *unused, double *out_649390414804865411) {
  h_28(state, unused, out_649390414804865411);
}
void car_H_28(double *state, double *unused, double *out_4197528649344884870) {
  H_28(state, unused, out_4197528649344884870);
}
void car_h_31(double *state, double *unused, double *out_8752849639850722770) {
  h_31(state, unused, out_8752849639850722770);
}
void car_H_31(double *state, double *unused, double *out_6511405356204402807) {
  H_31(state, unused, out_6511405356204402807);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
