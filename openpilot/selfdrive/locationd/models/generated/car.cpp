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
void err_fun(double *nom_x, double *delta_x, double *out_8266163715671326717) {
   out_8266163715671326717[0] = delta_x[0] + nom_x[0];
   out_8266163715671326717[1] = delta_x[1] + nom_x[1];
   out_8266163715671326717[2] = delta_x[2] + nom_x[2];
   out_8266163715671326717[3] = delta_x[3] + nom_x[3];
   out_8266163715671326717[4] = delta_x[4] + nom_x[4];
   out_8266163715671326717[5] = delta_x[5] + nom_x[5];
   out_8266163715671326717[6] = delta_x[6] + nom_x[6];
   out_8266163715671326717[7] = delta_x[7] + nom_x[7];
   out_8266163715671326717[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7126845649854226755) {
   out_7126845649854226755[0] = -nom_x[0] + true_x[0];
   out_7126845649854226755[1] = -nom_x[1] + true_x[1];
   out_7126845649854226755[2] = -nom_x[2] + true_x[2];
   out_7126845649854226755[3] = -nom_x[3] + true_x[3];
   out_7126845649854226755[4] = -nom_x[4] + true_x[4];
   out_7126845649854226755[5] = -nom_x[5] + true_x[5];
   out_7126845649854226755[6] = -nom_x[6] + true_x[6];
   out_7126845649854226755[7] = -nom_x[7] + true_x[7];
   out_7126845649854226755[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_4330413888236142730) {
   out_4330413888236142730[0] = 1.0;
   out_4330413888236142730[1] = 0.0;
   out_4330413888236142730[2] = 0.0;
   out_4330413888236142730[3] = 0.0;
   out_4330413888236142730[4] = 0.0;
   out_4330413888236142730[5] = 0.0;
   out_4330413888236142730[6] = 0.0;
   out_4330413888236142730[7] = 0.0;
   out_4330413888236142730[8] = 0.0;
   out_4330413888236142730[9] = 0.0;
   out_4330413888236142730[10] = 1.0;
   out_4330413888236142730[11] = 0.0;
   out_4330413888236142730[12] = 0.0;
   out_4330413888236142730[13] = 0.0;
   out_4330413888236142730[14] = 0.0;
   out_4330413888236142730[15] = 0.0;
   out_4330413888236142730[16] = 0.0;
   out_4330413888236142730[17] = 0.0;
   out_4330413888236142730[18] = 0.0;
   out_4330413888236142730[19] = 0.0;
   out_4330413888236142730[20] = 1.0;
   out_4330413888236142730[21] = 0.0;
   out_4330413888236142730[22] = 0.0;
   out_4330413888236142730[23] = 0.0;
   out_4330413888236142730[24] = 0.0;
   out_4330413888236142730[25] = 0.0;
   out_4330413888236142730[26] = 0.0;
   out_4330413888236142730[27] = 0.0;
   out_4330413888236142730[28] = 0.0;
   out_4330413888236142730[29] = 0.0;
   out_4330413888236142730[30] = 1.0;
   out_4330413888236142730[31] = 0.0;
   out_4330413888236142730[32] = 0.0;
   out_4330413888236142730[33] = 0.0;
   out_4330413888236142730[34] = 0.0;
   out_4330413888236142730[35] = 0.0;
   out_4330413888236142730[36] = 0.0;
   out_4330413888236142730[37] = 0.0;
   out_4330413888236142730[38] = 0.0;
   out_4330413888236142730[39] = 0.0;
   out_4330413888236142730[40] = 1.0;
   out_4330413888236142730[41] = 0.0;
   out_4330413888236142730[42] = 0.0;
   out_4330413888236142730[43] = 0.0;
   out_4330413888236142730[44] = 0.0;
   out_4330413888236142730[45] = 0.0;
   out_4330413888236142730[46] = 0.0;
   out_4330413888236142730[47] = 0.0;
   out_4330413888236142730[48] = 0.0;
   out_4330413888236142730[49] = 0.0;
   out_4330413888236142730[50] = 1.0;
   out_4330413888236142730[51] = 0.0;
   out_4330413888236142730[52] = 0.0;
   out_4330413888236142730[53] = 0.0;
   out_4330413888236142730[54] = 0.0;
   out_4330413888236142730[55] = 0.0;
   out_4330413888236142730[56] = 0.0;
   out_4330413888236142730[57] = 0.0;
   out_4330413888236142730[58] = 0.0;
   out_4330413888236142730[59] = 0.0;
   out_4330413888236142730[60] = 1.0;
   out_4330413888236142730[61] = 0.0;
   out_4330413888236142730[62] = 0.0;
   out_4330413888236142730[63] = 0.0;
   out_4330413888236142730[64] = 0.0;
   out_4330413888236142730[65] = 0.0;
   out_4330413888236142730[66] = 0.0;
   out_4330413888236142730[67] = 0.0;
   out_4330413888236142730[68] = 0.0;
   out_4330413888236142730[69] = 0.0;
   out_4330413888236142730[70] = 1.0;
   out_4330413888236142730[71] = 0.0;
   out_4330413888236142730[72] = 0.0;
   out_4330413888236142730[73] = 0.0;
   out_4330413888236142730[74] = 0.0;
   out_4330413888236142730[75] = 0.0;
   out_4330413888236142730[76] = 0.0;
   out_4330413888236142730[77] = 0.0;
   out_4330413888236142730[78] = 0.0;
   out_4330413888236142730[79] = 0.0;
   out_4330413888236142730[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_5168037528944651468) {
   out_5168037528944651468[0] = state[0];
   out_5168037528944651468[1] = state[1];
   out_5168037528944651468[2] = state[2];
   out_5168037528944651468[3] = state[3];
   out_5168037528944651468[4] = state[4];
   out_5168037528944651468[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5168037528944651468[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5168037528944651468[7] = state[7];
   out_5168037528944651468[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4416067367961397713) {
   out_4416067367961397713[0] = 1;
   out_4416067367961397713[1] = 0;
   out_4416067367961397713[2] = 0;
   out_4416067367961397713[3] = 0;
   out_4416067367961397713[4] = 0;
   out_4416067367961397713[5] = 0;
   out_4416067367961397713[6] = 0;
   out_4416067367961397713[7] = 0;
   out_4416067367961397713[8] = 0;
   out_4416067367961397713[9] = 0;
   out_4416067367961397713[10] = 1;
   out_4416067367961397713[11] = 0;
   out_4416067367961397713[12] = 0;
   out_4416067367961397713[13] = 0;
   out_4416067367961397713[14] = 0;
   out_4416067367961397713[15] = 0;
   out_4416067367961397713[16] = 0;
   out_4416067367961397713[17] = 0;
   out_4416067367961397713[18] = 0;
   out_4416067367961397713[19] = 0;
   out_4416067367961397713[20] = 1;
   out_4416067367961397713[21] = 0;
   out_4416067367961397713[22] = 0;
   out_4416067367961397713[23] = 0;
   out_4416067367961397713[24] = 0;
   out_4416067367961397713[25] = 0;
   out_4416067367961397713[26] = 0;
   out_4416067367961397713[27] = 0;
   out_4416067367961397713[28] = 0;
   out_4416067367961397713[29] = 0;
   out_4416067367961397713[30] = 1;
   out_4416067367961397713[31] = 0;
   out_4416067367961397713[32] = 0;
   out_4416067367961397713[33] = 0;
   out_4416067367961397713[34] = 0;
   out_4416067367961397713[35] = 0;
   out_4416067367961397713[36] = 0;
   out_4416067367961397713[37] = 0;
   out_4416067367961397713[38] = 0;
   out_4416067367961397713[39] = 0;
   out_4416067367961397713[40] = 1;
   out_4416067367961397713[41] = 0;
   out_4416067367961397713[42] = 0;
   out_4416067367961397713[43] = 0;
   out_4416067367961397713[44] = 0;
   out_4416067367961397713[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4416067367961397713[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4416067367961397713[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4416067367961397713[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4416067367961397713[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4416067367961397713[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4416067367961397713[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4416067367961397713[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4416067367961397713[53] = -9.8100000000000005*dt;
   out_4416067367961397713[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4416067367961397713[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4416067367961397713[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4416067367961397713[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4416067367961397713[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4416067367961397713[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4416067367961397713[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4416067367961397713[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4416067367961397713[62] = 0;
   out_4416067367961397713[63] = 0;
   out_4416067367961397713[64] = 0;
   out_4416067367961397713[65] = 0;
   out_4416067367961397713[66] = 0;
   out_4416067367961397713[67] = 0;
   out_4416067367961397713[68] = 0;
   out_4416067367961397713[69] = 0;
   out_4416067367961397713[70] = 1;
   out_4416067367961397713[71] = 0;
   out_4416067367961397713[72] = 0;
   out_4416067367961397713[73] = 0;
   out_4416067367961397713[74] = 0;
   out_4416067367961397713[75] = 0;
   out_4416067367961397713[76] = 0;
   out_4416067367961397713[77] = 0;
   out_4416067367961397713[78] = 0;
   out_4416067367961397713[79] = 0;
   out_4416067367961397713[80] = 1;
}
void h_25(double *state, double *unused, double *out_4710681085674461856) {
   out_4710681085674461856[0] = state[6];
}
void H_25(double *state, double *unused, double *out_573277936948522110) {
   out_573277936948522110[0] = 0;
   out_573277936948522110[1] = 0;
   out_573277936948522110[2] = 0;
   out_573277936948522110[3] = 0;
   out_573277936948522110[4] = 0;
   out_573277936948522110[5] = 0;
   out_573277936948522110[6] = 1;
   out_573277936948522110[7] = 0;
   out_573277936948522110[8] = 0;
}
void h_24(double *state, double *unused, double *out_6957739660515425269) {
   out_6957739660515425269[0] = state[4];
   out_6957739660515425269[1] = state[5];
}
void H_24(double *state, double *unused, double *out_7148849743540040211) {
   out_7148849743540040211[0] = 0;
   out_7148849743540040211[1] = 0;
   out_7148849743540040211[2] = 0;
   out_7148849743540040211[3] = 0;
   out_7148849743540040211[4] = 1;
   out_7148849743540040211[5] = 0;
   out_7148849743540040211[6] = 0;
   out_7148849743540040211[7] = 0;
   out_7148849743540040211[8] = 0;
   out_7148849743540040211[9] = 0;
   out_7148849743540040211[10] = 0;
   out_7148849743540040211[11] = 0;
   out_7148849743540040211[12] = 0;
   out_7148849743540040211[13] = 0;
   out_7148849743540040211[14] = 1;
   out_7148849743540040211[15] = 0;
   out_7148849743540040211[16] = 0;
   out_7148849743540040211[17] = 0;
}
void h_30(double *state, double *unused, double *out_6161312478303345023) {
   out_6161312478303345023[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7489968278440138865) {
   out_7489968278440138865[0] = 0;
   out_7489968278440138865[1] = 0;
   out_7489968278440138865[2] = 0;
   out_7489968278440138865[3] = 0;
   out_7489968278440138865[4] = 1;
   out_7489968278440138865[5] = 0;
   out_7489968278440138865[6] = 0;
   out_7489968278440138865[7] = 0;
   out_7489968278440138865[8] = 0;
}
void h_26(double *state, double *unused, double *out_5217802625401682588) {
   out_5217802625401682588[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3168225381925534114) {
   out_3168225381925534114[0] = 0;
   out_3168225381925534114[1] = 0;
   out_3168225381925534114[2] = 0;
   out_3168225381925534114[3] = 0;
   out_3168225381925534114[4] = 0;
   out_3168225381925534114[5] = 0;
   out_3168225381925534114[6] = 0;
   out_3168225381925534114[7] = 1;
   out_3168225381925534114[8] = 0;
}
void h_27(double *state, double *unused, double *out_6300919553606388947) {
   out_6300919553606388947[0] = state[3];
}
void H_27(double *state, double *unused, double *out_5315204966639713954) {
   out_5315204966639713954[0] = 0;
   out_5315204966639713954[1] = 0;
   out_5315204966639713954[2] = 0;
   out_5315204966639713954[3] = 1;
   out_5315204966639713954[4] = 0;
   out_5315204966639713954[5] = 0;
   out_5315204966639713954[6] = 0;
   out_5315204966639713954[7] = 0;
   out_5315204966639713954[8] = 0;
}
void h_29(double *state, double *unused, double *out_1159639210642499378) {
   out_1159639210642499378[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3601842239770162921) {
   out_3601842239770162921[0] = 0;
   out_3601842239770162921[1] = 1;
   out_3601842239770162921[2] = 0;
   out_3601842239770162921[3] = 0;
   out_3601842239770162921[4] = 0;
   out_3601842239770162921[5] = 0;
   out_3601842239770162921[6] = 0;
   out_3601842239770162921[7] = 0;
   out_3601842239770162921[8] = 0;
}
void h_28(double *state, double *unused, double *out_3920748180972170598) {
   out_3920748180972170598[0] = state[0];
}
void H_28(double *state, double *unused, double *out_1480556777299367653) {
   out_1480556777299367653[0] = 1;
   out_1480556777299367653[1] = 0;
   out_1480556777299367653[2] = 0;
   out_1480556777299367653[3] = 0;
   out_1480556777299367653[4] = 0;
   out_1480556777299367653[5] = 0;
   out_1480556777299367653[6] = 0;
   out_1480556777299367653[7] = 0;
   out_1480556777299367653[8] = 0;
}
void h_31(double *state, double *unused, double *out_2338203242308479048) {
   out_2338203242308479048[0] = state[8];
}
void H_31(double *state, double *unused, double *out_603923898825482538) {
   out_603923898825482538[0] = 0;
   out_603923898825482538[1] = 0;
   out_603923898825482538[2] = 0;
   out_603923898825482538[3] = 0;
   out_603923898825482538[4] = 0;
   out_603923898825482538[5] = 0;
   out_603923898825482538[6] = 0;
   out_603923898825482538[7] = 0;
   out_603923898825482538[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_8266163715671326717) {
  err_fun(nom_x, delta_x, out_8266163715671326717);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7126845649854226755) {
  inv_err_fun(nom_x, true_x, out_7126845649854226755);
}
void car_H_mod_fun(double *state, double *out_4330413888236142730) {
  H_mod_fun(state, out_4330413888236142730);
}
void car_f_fun(double *state, double dt, double *out_5168037528944651468) {
  f_fun(state,  dt, out_5168037528944651468);
}
void car_F_fun(double *state, double dt, double *out_4416067367961397713) {
  F_fun(state,  dt, out_4416067367961397713);
}
void car_h_25(double *state, double *unused, double *out_4710681085674461856) {
  h_25(state, unused, out_4710681085674461856);
}
void car_H_25(double *state, double *unused, double *out_573277936948522110) {
  H_25(state, unused, out_573277936948522110);
}
void car_h_24(double *state, double *unused, double *out_6957739660515425269) {
  h_24(state, unused, out_6957739660515425269);
}
void car_H_24(double *state, double *unused, double *out_7148849743540040211) {
  H_24(state, unused, out_7148849743540040211);
}
void car_h_30(double *state, double *unused, double *out_6161312478303345023) {
  h_30(state, unused, out_6161312478303345023);
}
void car_H_30(double *state, double *unused, double *out_7489968278440138865) {
  H_30(state, unused, out_7489968278440138865);
}
void car_h_26(double *state, double *unused, double *out_5217802625401682588) {
  h_26(state, unused, out_5217802625401682588);
}
void car_H_26(double *state, double *unused, double *out_3168225381925534114) {
  H_26(state, unused, out_3168225381925534114);
}
void car_h_27(double *state, double *unused, double *out_6300919553606388947) {
  h_27(state, unused, out_6300919553606388947);
}
void car_H_27(double *state, double *unused, double *out_5315204966639713954) {
  H_27(state, unused, out_5315204966639713954);
}
void car_h_29(double *state, double *unused, double *out_1159639210642499378) {
  h_29(state, unused, out_1159639210642499378);
}
void car_H_29(double *state, double *unused, double *out_3601842239770162921) {
  H_29(state, unused, out_3601842239770162921);
}
void car_h_28(double *state, double *unused, double *out_3920748180972170598) {
  h_28(state, unused, out_3920748180972170598);
}
void car_H_28(double *state, double *unused, double *out_1480556777299367653) {
  H_28(state, unused, out_1480556777299367653);
}
void car_h_31(double *state, double *unused, double *out_2338203242308479048) {
  h_31(state, unused, out_2338203242308479048);
}
void car_H_31(double *state, double *unused, double *out_603923898825482538) {
  H_31(state, unused, out_603923898825482538);
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
