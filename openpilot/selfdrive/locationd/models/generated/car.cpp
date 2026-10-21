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
void err_fun(double *nom_x, double *delta_x, double *out_7902446637228762316) {
   out_7902446637228762316[0] = delta_x[0] + nom_x[0];
   out_7902446637228762316[1] = delta_x[1] + nom_x[1];
   out_7902446637228762316[2] = delta_x[2] + nom_x[2];
   out_7902446637228762316[3] = delta_x[3] + nom_x[3];
   out_7902446637228762316[4] = delta_x[4] + nom_x[4];
   out_7902446637228762316[5] = delta_x[5] + nom_x[5];
   out_7902446637228762316[6] = delta_x[6] + nom_x[6];
   out_7902446637228762316[7] = delta_x[7] + nom_x[7];
   out_7902446637228762316[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8054518576183361196) {
   out_8054518576183361196[0] = -nom_x[0] + true_x[0];
   out_8054518576183361196[1] = -nom_x[1] + true_x[1];
   out_8054518576183361196[2] = -nom_x[2] + true_x[2];
   out_8054518576183361196[3] = -nom_x[3] + true_x[3];
   out_8054518576183361196[4] = -nom_x[4] + true_x[4];
   out_8054518576183361196[5] = -nom_x[5] + true_x[5];
   out_8054518576183361196[6] = -nom_x[6] + true_x[6];
   out_8054518576183361196[7] = -nom_x[7] + true_x[7];
   out_8054518576183361196[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_223929158544482531) {
   out_223929158544482531[0] = 1.0;
   out_223929158544482531[1] = 0.0;
   out_223929158544482531[2] = 0.0;
   out_223929158544482531[3] = 0.0;
   out_223929158544482531[4] = 0.0;
   out_223929158544482531[5] = 0.0;
   out_223929158544482531[6] = 0.0;
   out_223929158544482531[7] = 0.0;
   out_223929158544482531[8] = 0.0;
   out_223929158544482531[9] = 0.0;
   out_223929158544482531[10] = 1.0;
   out_223929158544482531[11] = 0.0;
   out_223929158544482531[12] = 0.0;
   out_223929158544482531[13] = 0.0;
   out_223929158544482531[14] = 0.0;
   out_223929158544482531[15] = 0.0;
   out_223929158544482531[16] = 0.0;
   out_223929158544482531[17] = 0.0;
   out_223929158544482531[18] = 0.0;
   out_223929158544482531[19] = 0.0;
   out_223929158544482531[20] = 1.0;
   out_223929158544482531[21] = 0.0;
   out_223929158544482531[22] = 0.0;
   out_223929158544482531[23] = 0.0;
   out_223929158544482531[24] = 0.0;
   out_223929158544482531[25] = 0.0;
   out_223929158544482531[26] = 0.0;
   out_223929158544482531[27] = 0.0;
   out_223929158544482531[28] = 0.0;
   out_223929158544482531[29] = 0.0;
   out_223929158544482531[30] = 1.0;
   out_223929158544482531[31] = 0.0;
   out_223929158544482531[32] = 0.0;
   out_223929158544482531[33] = 0.0;
   out_223929158544482531[34] = 0.0;
   out_223929158544482531[35] = 0.0;
   out_223929158544482531[36] = 0.0;
   out_223929158544482531[37] = 0.0;
   out_223929158544482531[38] = 0.0;
   out_223929158544482531[39] = 0.0;
   out_223929158544482531[40] = 1.0;
   out_223929158544482531[41] = 0.0;
   out_223929158544482531[42] = 0.0;
   out_223929158544482531[43] = 0.0;
   out_223929158544482531[44] = 0.0;
   out_223929158544482531[45] = 0.0;
   out_223929158544482531[46] = 0.0;
   out_223929158544482531[47] = 0.0;
   out_223929158544482531[48] = 0.0;
   out_223929158544482531[49] = 0.0;
   out_223929158544482531[50] = 1.0;
   out_223929158544482531[51] = 0.0;
   out_223929158544482531[52] = 0.0;
   out_223929158544482531[53] = 0.0;
   out_223929158544482531[54] = 0.0;
   out_223929158544482531[55] = 0.0;
   out_223929158544482531[56] = 0.0;
   out_223929158544482531[57] = 0.0;
   out_223929158544482531[58] = 0.0;
   out_223929158544482531[59] = 0.0;
   out_223929158544482531[60] = 1.0;
   out_223929158544482531[61] = 0.0;
   out_223929158544482531[62] = 0.0;
   out_223929158544482531[63] = 0.0;
   out_223929158544482531[64] = 0.0;
   out_223929158544482531[65] = 0.0;
   out_223929158544482531[66] = 0.0;
   out_223929158544482531[67] = 0.0;
   out_223929158544482531[68] = 0.0;
   out_223929158544482531[69] = 0.0;
   out_223929158544482531[70] = 1.0;
   out_223929158544482531[71] = 0.0;
   out_223929158544482531[72] = 0.0;
   out_223929158544482531[73] = 0.0;
   out_223929158544482531[74] = 0.0;
   out_223929158544482531[75] = 0.0;
   out_223929158544482531[76] = 0.0;
   out_223929158544482531[77] = 0.0;
   out_223929158544482531[78] = 0.0;
   out_223929158544482531[79] = 0.0;
   out_223929158544482531[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_608826670593016442) {
   out_608826670593016442[0] = state[0];
   out_608826670593016442[1] = state[1];
   out_608826670593016442[2] = state[2];
   out_608826670593016442[3] = state[3];
   out_608826670593016442[4] = state[4];
   out_608826670593016442[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_608826670593016442[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_608826670593016442[7] = state[7];
   out_608826670593016442[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4256277468531545359) {
   out_4256277468531545359[0] = 1;
   out_4256277468531545359[1] = 0;
   out_4256277468531545359[2] = 0;
   out_4256277468531545359[3] = 0;
   out_4256277468531545359[4] = 0;
   out_4256277468531545359[5] = 0;
   out_4256277468531545359[6] = 0;
   out_4256277468531545359[7] = 0;
   out_4256277468531545359[8] = 0;
   out_4256277468531545359[9] = 0;
   out_4256277468531545359[10] = 1;
   out_4256277468531545359[11] = 0;
   out_4256277468531545359[12] = 0;
   out_4256277468531545359[13] = 0;
   out_4256277468531545359[14] = 0;
   out_4256277468531545359[15] = 0;
   out_4256277468531545359[16] = 0;
   out_4256277468531545359[17] = 0;
   out_4256277468531545359[18] = 0;
   out_4256277468531545359[19] = 0;
   out_4256277468531545359[20] = 1;
   out_4256277468531545359[21] = 0;
   out_4256277468531545359[22] = 0;
   out_4256277468531545359[23] = 0;
   out_4256277468531545359[24] = 0;
   out_4256277468531545359[25] = 0;
   out_4256277468531545359[26] = 0;
   out_4256277468531545359[27] = 0;
   out_4256277468531545359[28] = 0;
   out_4256277468531545359[29] = 0;
   out_4256277468531545359[30] = 1;
   out_4256277468531545359[31] = 0;
   out_4256277468531545359[32] = 0;
   out_4256277468531545359[33] = 0;
   out_4256277468531545359[34] = 0;
   out_4256277468531545359[35] = 0;
   out_4256277468531545359[36] = 0;
   out_4256277468531545359[37] = 0;
   out_4256277468531545359[38] = 0;
   out_4256277468531545359[39] = 0;
   out_4256277468531545359[40] = 1;
   out_4256277468531545359[41] = 0;
   out_4256277468531545359[42] = 0;
   out_4256277468531545359[43] = 0;
   out_4256277468531545359[44] = 0;
   out_4256277468531545359[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4256277468531545359[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4256277468531545359[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4256277468531545359[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4256277468531545359[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4256277468531545359[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4256277468531545359[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4256277468531545359[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4256277468531545359[53] = -9.8100000000000005*dt;
   out_4256277468531545359[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4256277468531545359[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4256277468531545359[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4256277468531545359[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4256277468531545359[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4256277468531545359[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4256277468531545359[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4256277468531545359[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4256277468531545359[62] = 0;
   out_4256277468531545359[63] = 0;
   out_4256277468531545359[64] = 0;
   out_4256277468531545359[65] = 0;
   out_4256277468531545359[66] = 0;
   out_4256277468531545359[67] = 0;
   out_4256277468531545359[68] = 0;
   out_4256277468531545359[69] = 0;
   out_4256277468531545359[70] = 1;
   out_4256277468531545359[71] = 0;
   out_4256277468531545359[72] = 0;
   out_4256277468531545359[73] = 0;
   out_4256277468531545359[74] = 0;
   out_4256277468531545359[75] = 0;
   out_4256277468531545359[76] = 0;
   out_4256277468531545359[77] = 0;
   out_4256277468531545359[78] = 0;
   out_4256277468531545359[79] = 0;
   out_4256277468531545359[80] = 1;
}
void h_25(double *state, double *unused, double *out_4488142222450972896) {
   out_4488142222450972896[0] = state[6];
}
void H_25(double *state, double *unused, double *out_9029626689252831848) {
   out_9029626689252831848[0] = 0;
   out_9029626689252831848[1] = 0;
   out_9029626689252831848[2] = 0;
   out_9029626689252831848[3] = 0;
   out_9029626689252831848[4] = 0;
   out_9029626689252831848[5] = 0;
   out_9029626689252831848[6] = 1;
   out_9029626689252831848[7] = 0;
   out_9029626689252831848[8] = 0;
}
void h_24(double *state, double *unused, double *out_203195263897949964) {
   out_203195263897949964[0] = state[4];
   out_203195263897949964[1] = state[5];
}
void H_24(double *state, double *unused, double *out_4283117362286322814) {
   out_4283117362286322814[0] = 0;
   out_4283117362286322814[1] = 0;
   out_4283117362286322814[2] = 0;
   out_4283117362286322814[3] = 0;
   out_4283117362286322814[4] = 1;
   out_4283117362286322814[5] = 0;
   out_4283117362286322814[6] = 0;
   out_4283117362286322814[7] = 0;
   out_4283117362286322814[8] = 0;
   out_4283117362286322814[9] = 0;
   out_4283117362286322814[10] = 0;
   out_4283117362286322814[11] = 0;
   out_4283117362286322814[12] = 0;
   out_4283117362286322814[13] = 0;
   out_4283117362286322814[14] = 1;
   out_4283117362286322814[15] = 0;
   out_4283117362286322814[16] = 0;
   out_4283117362286322814[17] = 0;
}
void h_30(double *state, double *unused, double *out_7187766624908227784) {
   out_7187766624908227784[0] = state[4];
}
void H_30(double *state, double *unused, double *out_2500427042965103013) {
   out_2500427042965103013[0] = 0;
   out_2500427042965103013[1] = 0;
   out_2500427042965103013[2] = 0;
   out_2500427042965103013[3] = 0;
   out_2500427042965103013[4] = 1;
   out_2500427042965103013[5] = 0;
   out_2500427042965103013[6] = 0;
   out_2500427042965103013[7] = 0;
   out_2500427042965103013[8] = 0;
}
void h_26(double *state, double *unused, double *out_8355510425984369521) {
   out_8355510425984369521[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6112591414695919167) {
   out_6112591414695919167[0] = 0;
   out_6112591414695919167[1] = 0;
   out_6112591414695919167[2] = 0;
   out_6112591414695919167[3] = 0;
   out_6112591414695919167[4] = 0;
   out_6112591414695919167[5] = 0;
   out_6112591414695919167[6] = 0;
   out_6112591414695919167[7] = 1;
   out_6112591414695919167[8] = 0;
}
void h_27(double *state, double *unused, double *out_513531045209872749) {
   out_513531045209872749[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4675190354765527924) {
   out_4675190354765527924[0] = 0;
   out_4675190354765527924[1] = 0;
   out_4675190354765527924[2] = 0;
   out_4675190354765527924[3] = 1;
   out_4675190354765527924[4] = 0;
   out_4675190354765527924[5] = 0;
   out_4675190354765527924[6] = 0;
   out_4675190354765527924[7] = 0;
   out_4675190354765527924[8] = 0;
}
void h_29(double *state, double *unused, double *out_4375214521638374401) {
   out_4375214521638374401[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6388553081635078957) {
   out_6388553081635078957[0] = 0;
   out_6388553081635078957[1] = 1;
   out_6388553081635078957[2] = 0;
   out_6388553081635078957[3] = 0;
   out_6388553081635078957[4] = 0;
   out_6388553081635078957[5] = 0;
   out_6388553081635078957[6] = 0;
   out_6388553081635078957[7] = 0;
   out_6388553081635078957[8] = 0;
}
void h_28(double *state, double *unused, double *out_67861000273519612) {
   out_67861000273519612[0] = state[0];
}
void H_28(double *state, double *unused, double *out_6975791975004942085) {
   out_6975791975004942085[0] = 1;
   out_6975791975004942085[1] = 0;
   out_6975791975004942085[2] = 0;
   out_6975791975004942085[3] = 0;
   out_6975791975004942085[4] = 0;
   out_6975791975004942085[5] = 0;
   out_6975791975004942085[6] = 0;
   out_6975791975004942085[7] = 0;
   out_6975791975004942085[8] = 0;
}
void h_31(double *state, double *unused, double *out_7435868212806457857) {
   out_7435868212806457857[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2340442133944902515) {
   out_2340442133944902515[0] = 0;
   out_2340442133944902515[1] = 0;
   out_2340442133944902515[2] = 0;
   out_2340442133944902515[3] = 0;
   out_2340442133944902515[4] = 0;
   out_2340442133944902515[5] = 0;
   out_2340442133944902515[6] = 0;
   out_2340442133944902515[7] = 0;
   out_2340442133944902515[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7902446637228762316) {
  err_fun(nom_x, delta_x, out_7902446637228762316);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8054518576183361196) {
  inv_err_fun(nom_x, true_x, out_8054518576183361196);
}
void car_H_mod_fun(double *state, double *out_223929158544482531) {
  H_mod_fun(state, out_223929158544482531);
}
void car_f_fun(double *state, double dt, double *out_608826670593016442) {
  f_fun(state,  dt, out_608826670593016442);
}
void car_F_fun(double *state, double dt, double *out_4256277468531545359) {
  F_fun(state,  dt, out_4256277468531545359);
}
void car_h_25(double *state, double *unused, double *out_4488142222450972896) {
  h_25(state, unused, out_4488142222450972896);
}
void car_H_25(double *state, double *unused, double *out_9029626689252831848) {
  H_25(state, unused, out_9029626689252831848);
}
void car_h_24(double *state, double *unused, double *out_203195263897949964) {
  h_24(state, unused, out_203195263897949964);
}
void car_H_24(double *state, double *unused, double *out_4283117362286322814) {
  H_24(state, unused, out_4283117362286322814);
}
void car_h_30(double *state, double *unused, double *out_7187766624908227784) {
  h_30(state, unused, out_7187766624908227784);
}
void car_H_30(double *state, double *unused, double *out_2500427042965103013) {
  H_30(state, unused, out_2500427042965103013);
}
void car_h_26(double *state, double *unused, double *out_8355510425984369521) {
  h_26(state, unused, out_8355510425984369521);
}
void car_H_26(double *state, double *unused, double *out_6112591414695919167) {
  H_26(state, unused, out_6112591414695919167);
}
void car_h_27(double *state, double *unused, double *out_513531045209872749) {
  h_27(state, unused, out_513531045209872749);
}
void car_H_27(double *state, double *unused, double *out_4675190354765527924) {
  H_27(state, unused, out_4675190354765527924);
}
void car_h_29(double *state, double *unused, double *out_4375214521638374401) {
  h_29(state, unused, out_4375214521638374401);
}
void car_H_29(double *state, double *unused, double *out_6388553081635078957) {
  H_29(state, unused, out_6388553081635078957);
}
void car_h_28(double *state, double *unused, double *out_67861000273519612) {
  h_28(state, unused, out_67861000273519612);
}
void car_H_28(double *state, double *unused, double *out_6975791975004942085) {
  H_28(state, unused, out_6975791975004942085);
}
void car_h_31(double *state, double *unused, double *out_7435868212806457857) {
  h_31(state, unused, out_7435868212806457857);
}
void car_H_31(double *state, double *unused, double *out_2340442133944902515) {
  H_31(state, unused, out_2340442133944902515);
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
