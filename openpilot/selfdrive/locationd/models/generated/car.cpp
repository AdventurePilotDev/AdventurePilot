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
void err_fun(double *nom_x, double *delta_x, double *out_3208522223344719935) {
   out_3208522223344719935[0] = delta_x[0] + nom_x[0];
   out_3208522223344719935[1] = delta_x[1] + nom_x[1];
   out_3208522223344719935[2] = delta_x[2] + nom_x[2];
   out_3208522223344719935[3] = delta_x[3] + nom_x[3];
   out_3208522223344719935[4] = delta_x[4] + nom_x[4];
   out_3208522223344719935[5] = delta_x[5] + nom_x[5];
   out_3208522223344719935[6] = delta_x[6] + nom_x[6];
   out_3208522223344719935[7] = delta_x[7] + nom_x[7];
   out_3208522223344719935[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3256968781919030142) {
   out_3256968781919030142[0] = -nom_x[0] + true_x[0];
   out_3256968781919030142[1] = -nom_x[1] + true_x[1];
   out_3256968781919030142[2] = -nom_x[2] + true_x[2];
   out_3256968781919030142[3] = -nom_x[3] + true_x[3];
   out_3256968781919030142[4] = -nom_x[4] + true_x[4];
   out_3256968781919030142[5] = -nom_x[5] + true_x[5];
   out_3256968781919030142[6] = -nom_x[6] + true_x[6];
   out_3256968781919030142[7] = -nom_x[7] + true_x[7];
   out_3256968781919030142[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_8287908987357363226) {
   out_8287908987357363226[0] = 1.0;
   out_8287908987357363226[1] = 0.0;
   out_8287908987357363226[2] = 0.0;
   out_8287908987357363226[3] = 0.0;
   out_8287908987357363226[4] = 0.0;
   out_8287908987357363226[5] = 0.0;
   out_8287908987357363226[6] = 0.0;
   out_8287908987357363226[7] = 0.0;
   out_8287908987357363226[8] = 0.0;
   out_8287908987357363226[9] = 0.0;
   out_8287908987357363226[10] = 1.0;
   out_8287908987357363226[11] = 0.0;
   out_8287908987357363226[12] = 0.0;
   out_8287908987357363226[13] = 0.0;
   out_8287908987357363226[14] = 0.0;
   out_8287908987357363226[15] = 0.0;
   out_8287908987357363226[16] = 0.0;
   out_8287908987357363226[17] = 0.0;
   out_8287908987357363226[18] = 0.0;
   out_8287908987357363226[19] = 0.0;
   out_8287908987357363226[20] = 1.0;
   out_8287908987357363226[21] = 0.0;
   out_8287908987357363226[22] = 0.0;
   out_8287908987357363226[23] = 0.0;
   out_8287908987357363226[24] = 0.0;
   out_8287908987357363226[25] = 0.0;
   out_8287908987357363226[26] = 0.0;
   out_8287908987357363226[27] = 0.0;
   out_8287908987357363226[28] = 0.0;
   out_8287908987357363226[29] = 0.0;
   out_8287908987357363226[30] = 1.0;
   out_8287908987357363226[31] = 0.0;
   out_8287908987357363226[32] = 0.0;
   out_8287908987357363226[33] = 0.0;
   out_8287908987357363226[34] = 0.0;
   out_8287908987357363226[35] = 0.0;
   out_8287908987357363226[36] = 0.0;
   out_8287908987357363226[37] = 0.0;
   out_8287908987357363226[38] = 0.0;
   out_8287908987357363226[39] = 0.0;
   out_8287908987357363226[40] = 1.0;
   out_8287908987357363226[41] = 0.0;
   out_8287908987357363226[42] = 0.0;
   out_8287908987357363226[43] = 0.0;
   out_8287908987357363226[44] = 0.0;
   out_8287908987357363226[45] = 0.0;
   out_8287908987357363226[46] = 0.0;
   out_8287908987357363226[47] = 0.0;
   out_8287908987357363226[48] = 0.0;
   out_8287908987357363226[49] = 0.0;
   out_8287908987357363226[50] = 1.0;
   out_8287908987357363226[51] = 0.0;
   out_8287908987357363226[52] = 0.0;
   out_8287908987357363226[53] = 0.0;
   out_8287908987357363226[54] = 0.0;
   out_8287908987357363226[55] = 0.0;
   out_8287908987357363226[56] = 0.0;
   out_8287908987357363226[57] = 0.0;
   out_8287908987357363226[58] = 0.0;
   out_8287908987357363226[59] = 0.0;
   out_8287908987357363226[60] = 1.0;
   out_8287908987357363226[61] = 0.0;
   out_8287908987357363226[62] = 0.0;
   out_8287908987357363226[63] = 0.0;
   out_8287908987357363226[64] = 0.0;
   out_8287908987357363226[65] = 0.0;
   out_8287908987357363226[66] = 0.0;
   out_8287908987357363226[67] = 0.0;
   out_8287908987357363226[68] = 0.0;
   out_8287908987357363226[69] = 0.0;
   out_8287908987357363226[70] = 1.0;
   out_8287908987357363226[71] = 0.0;
   out_8287908987357363226[72] = 0.0;
   out_8287908987357363226[73] = 0.0;
   out_8287908987357363226[74] = 0.0;
   out_8287908987357363226[75] = 0.0;
   out_8287908987357363226[76] = 0.0;
   out_8287908987357363226[77] = 0.0;
   out_8287908987357363226[78] = 0.0;
   out_8287908987357363226[79] = 0.0;
   out_8287908987357363226[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_6824723206088421349) {
   out_6824723206088421349[0] = state[0];
   out_6824723206088421349[1] = state[1];
   out_6824723206088421349[2] = state[2];
   out_6824723206088421349[3] = state[3];
   out_6824723206088421349[4] = state[4];
   out_6824723206088421349[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_6824723206088421349[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_6824723206088421349[7] = state[7];
   out_6824723206088421349[8] = state[8];
}
void F_fun(double *state, double dt, double *out_7714489675589964189) {
   out_7714489675589964189[0] = 1;
   out_7714489675589964189[1] = 0;
   out_7714489675589964189[2] = 0;
   out_7714489675589964189[3] = 0;
   out_7714489675589964189[4] = 0;
   out_7714489675589964189[5] = 0;
   out_7714489675589964189[6] = 0;
   out_7714489675589964189[7] = 0;
   out_7714489675589964189[8] = 0;
   out_7714489675589964189[9] = 0;
   out_7714489675589964189[10] = 1;
   out_7714489675589964189[11] = 0;
   out_7714489675589964189[12] = 0;
   out_7714489675589964189[13] = 0;
   out_7714489675589964189[14] = 0;
   out_7714489675589964189[15] = 0;
   out_7714489675589964189[16] = 0;
   out_7714489675589964189[17] = 0;
   out_7714489675589964189[18] = 0;
   out_7714489675589964189[19] = 0;
   out_7714489675589964189[20] = 1;
   out_7714489675589964189[21] = 0;
   out_7714489675589964189[22] = 0;
   out_7714489675589964189[23] = 0;
   out_7714489675589964189[24] = 0;
   out_7714489675589964189[25] = 0;
   out_7714489675589964189[26] = 0;
   out_7714489675589964189[27] = 0;
   out_7714489675589964189[28] = 0;
   out_7714489675589964189[29] = 0;
   out_7714489675589964189[30] = 1;
   out_7714489675589964189[31] = 0;
   out_7714489675589964189[32] = 0;
   out_7714489675589964189[33] = 0;
   out_7714489675589964189[34] = 0;
   out_7714489675589964189[35] = 0;
   out_7714489675589964189[36] = 0;
   out_7714489675589964189[37] = 0;
   out_7714489675589964189[38] = 0;
   out_7714489675589964189[39] = 0;
   out_7714489675589964189[40] = 1;
   out_7714489675589964189[41] = 0;
   out_7714489675589964189[42] = 0;
   out_7714489675589964189[43] = 0;
   out_7714489675589964189[44] = 0;
   out_7714489675589964189[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_7714489675589964189[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_7714489675589964189[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7714489675589964189[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7714489675589964189[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_7714489675589964189[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_7714489675589964189[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_7714489675589964189[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_7714489675589964189[53] = -9.8100000000000005*dt;
   out_7714489675589964189[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_7714489675589964189[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_7714489675589964189[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7714489675589964189[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7714489675589964189[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_7714489675589964189[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_7714489675589964189[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_7714489675589964189[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7714489675589964189[62] = 0;
   out_7714489675589964189[63] = 0;
   out_7714489675589964189[64] = 0;
   out_7714489675589964189[65] = 0;
   out_7714489675589964189[66] = 0;
   out_7714489675589964189[67] = 0;
   out_7714489675589964189[68] = 0;
   out_7714489675589964189[69] = 0;
   out_7714489675589964189[70] = 1;
   out_7714489675589964189[71] = 0;
   out_7714489675589964189[72] = 0;
   out_7714489675589964189[73] = 0;
   out_7714489675589964189[74] = 0;
   out_7714489675589964189[75] = 0;
   out_7714489675589964189[76] = 0;
   out_7714489675589964189[77] = 0;
   out_7714489675589964189[78] = 0;
   out_7714489675589964189[79] = 0;
   out_7714489675589964189[80] = 1;
}
void h_25(double *state, double *unused, double *out_4089721760690641625) {
   out_4089721760690641625[0] = state[6];
}
void H_25(double *state, double *unused, double *out_8854495976160216770) {
   out_8854495976160216770[0] = 0;
   out_8854495976160216770[1] = 0;
   out_8854495976160216770[2] = 0;
   out_8854495976160216770[3] = 0;
   out_8854495976160216770[4] = 0;
   out_8854495976160216770[5] = 0;
   out_8854495976160216770[6] = 1;
   out_8854495976160216770[7] = 0;
   out_8854495976160216770[8] = 0;
}
void h_24(double *state, double *unused, double *out_7265493314677083858) {
   out_7265493314677083858[0] = state[4];
   out_7265493314677083858[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6681846377154717204) {
   out_6681846377154717204[0] = 0;
   out_6681846377154717204[1] = 0;
   out_6681846377154717204[2] = 0;
   out_6681846377154717204[3] = 0;
   out_6681846377154717204[4] = 1;
   out_6681846377154717204[5] = 0;
   out_6681846377154717204[6] = 0;
   out_6681846377154717204[7] = 0;
   out_6681846377154717204[8] = 0;
   out_6681846377154717204[9] = 0;
   out_6681846377154717204[10] = 0;
   out_6681846377154717204[11] = 0;
   out_6681846377154717204[12] = 0;
   out_6681846377154717204[13] = 0;
   out_6681846377154717204[14] = 1;
   out_6681846377154717204[15] = 0;
   out_6681846377154717204[16] = 0;
   out_6681846377154717204[17] = 0;
}
void h_30(double *state, double *unused, double *out_3932535092339512480) {
   out_3932535092339512480[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4326799646032608572) {
   out_4326799646032608572[0] = 0;
   out_4326799646032608572[1] = 0;
   out_4326799646032608572[2] = 0;
   out_4326799646032608572[3] = 0;
   out_4326799646032608572[4] = 1;
   out_4326799646032608572[5] = 0;
   out_4326799646032608572[6] = 0;
   out_4326799646032608572[7] = 0;
   out_4326799646032608572[8] = 0;
}
void h_26(double *state, double *unused, double *out_5532239809363198371) {
   out_5532239809363198371[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5112992657286160546) {
   out_5112992657286160546[0] = 0;
   out_5112992657286160546[1] = 0;
   out_5112992657286160546[2] = 0;
   out_5112992657286160546[3] = 0;
   out_5112992657286160546[4] = 0;
   out_5112992657286160546[5] = 0;
   out_5112992657286160546[6] = 0;
   out_5112992657286160546[7] = 1;
   out_5112992657286160546[8] = 0;
}
void h_27(double *state, double *unused, double *out_2309319756723207521) {
   out_2309319756723207521[0] = state[3];
}
void H_27(double *state, double *unused, double *out_6550393717216551789) {
   out_6550393717216551789[0] = 0;
   out_6550393717216551789[1] = 0;
   out_6550393717216551789[2] = 0;
   out_6550393717216551789[3] = 1;
   out_6550393717216551789[4] = 0;
   out_6550393717216551789[5] = 0;
   out_6550393717216551789[6] = 0;
   out_6550393717216551789[7] = 0;
   out_6550393717216551789[8] = 0;
}
void h_29(double *state, double *unused, double *out_2466506425074336666) {
   out_2466506425074336666[0] = state[1];
}
void H_29(double *state, double *unused, double *out_4837030990347000756) {
   out_4837030990347000756[0] = 0;
   out_4837030990347000756[1] = 1;
   out_4837030990347000756[2] = 0;
   out_4837030990347000756[3] = 0;
   out_4837030990347000756[4] = 0;
   out_4837030990347000756[5] = 0;
   out_4837030990347000756[6] = 0;
   out_4837030990347000756[7] = 0;
   out_4837030990347000756[8] = 0;
}
void h_28(double *state, double *unused, double *out_1880485542097265759) {
   out_1880485542097265759[0] = state[0];
}
void H_28(double *state, double *unused, double *out_6800661261912327007) {
   out_6800661261912327007[0] = 1;
   out_6800661261912327007[1] = 0;
   out_6800661261912327007[2] = 0;
   out_6800661261912327007[3] = 0;
   out_6800661261912327007[4] = 0;
   out_6800661261912327007[5] = 0;
   out_6800661261912327007[6] = 0;
   out_6800661261912327007[7] = 0;
   out_6800661261912327007[8] = 0;
}
void h_31(double *state, double *unused, double *out_7586187086668559055) {
   out_7586187086668559055[0] = state[8];
}
void H_31(double *state, double *unused, double *out_8885141938037177198) {
   out_8885141938037177198[0] = 0;
   out_8885141938037177198[1] = 0;
   out_8885141938037177198[2] = 0;
   out_8885141938037177198[3] = 0;
   out_8885141938037177198[4] = 0;
   out_8885141938037177198[5] = 0;
   out_8885141938037177198[6] = 0;
   out_8885141938037177198[7] = 0;
   out_8885141938037177198[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_3208522223344719935) {
  err_fun(nom_x, delta_x, out_3208522223344719935);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3256968781919030142) {
  inv_err_fun(nom_x, true_x, out_3256968781919030142);
}
void car_H_mod_fun(double *state, double *out_8287908987357363226) {
  H_mod_fun(state, out_8287908987357363226);
}
void car_f_fun(double *state, double dt, double *out_6824723206088421349) {
  f_fun(state,  dt, out_6824723206088421349);
}
void car_F_fun(double *state, double dt, double *out_7714489675589964189) {
  F_fun(state,  dt, out_7714489675589964189);
}
void car_h_25(double *state, double *unused, double *out_4089721760690641625) {
  h_25(state, unused, out_4089721760690641625);
}
void car_H_25(double *state, double *unused, double *out_8854495976160216770) {
  H_25(state, unused, out_8854495976160216770);
}
void car_h_24(double *state, double *unused, double *out_7265493314677083858) {
  h_24(state, unused, out_7265493314677083858);
}
void car_H_24(double *state, double *unused, double *out_6681846377154717204) {
  H_24(state, unused, out_6681846377154717204);
}
void car_h_30(double *state, double *unused, double *out_3932535092339512480) {
  h_30(state, unused, out_3932535092339512480);
}
void car_H_30(double *state, double *unused, double *out_4326799646032608572) {
  H_30(state, unused, out_4326799646032608572);
}
void car_h_26(double *state, double *unused, double *out_5532239809363198371) {
  h_26(state, unused, out_5532239809363198371);
}
void car_H_26(double *state, double *unused, double *out_5112992657286160546) {
  H_26(state, unused, out_5112992657286160546);
}
void car_h_27(double *state, double *unused, double *out_2309319756723207521) {
  h_27(state, unused, out_2309319756723207521);
}
void car_H_27(double *state, double *unused, double *out_6550393717216551789) {
  H_27(state, unused, out_6550393717216551789);
}
void car_h_29(double *state, double *unused, double *out_2466506425074336666) {
  h_29(state, unused, out_2466506425074336666);
}
void car_H_29(double *state, double *unused, double *out_4837030990347000756) {
  H_29(state, unused, out_4837030990347000756);
}
void car_h_28(double *state, double *unused, double *out_1880485542097265759) {
  h_28(state, unused, out_1880485542097265759);
}
void car_H_28(double *state, double *unused, double *out_6800661261912327007) {
  H_28(state, unused, out_6800661261912327007);
}
void car_h_31(double *state, double *unused, double *out_7586187086668559055) {
  h_31(state, unused, out_7586187086668559055);
}
void car_H_31(double *state, double *unused, double *out_8885141938037177198) {
  H_31(state, unused, out_8885141938037177198);
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
