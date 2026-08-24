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
void err_fun(double *nom_x, double *delta_x, double *out_7224272908344467420) {
   out_7224272908344467420[0] = delta_x[0] + nom_x[0];
   out_7224272908344467420[1] = delta_x[1] + nom_x[1];
   out_7224272908344467420[2] = delta_x[2] + nom_x[2];
   out_7224272908344467420[3] = delta_x[3] + nom_x[3];
   out_7224272908344467420[4] = delta_x[4] + nom_x[4];
   out_7224272908344467420[5] = delta_x[5] + nom_x[5];
   out_7224272908344467420[6] = delta_x[6] + nom_x[6];
   out_7224272908344467420[7] = delta_x[7] + nom_x[7];
   out_7224272908344467420[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2253777461209007309) {
   out_2253777461209007309[0] = -nom_x[0] + true_x[0];
   out_2253777461209007309[1] = -nom_x[1] + true_x[1];
   out_2253777461209007309[2] = -nom_x[2] + true_x[2];
   out_2253777461209007309[3] = -nom_x[3] + true_x[3];
   out_2253777461209007309[4] = -nom_x[4] + true_x[4];
   out_2253777461209007309[5] = -nom_x[5] + true_x[5];
   out_2253777461209007309[6] = -nom_x[6] + true_x[6];
   out_2253777461209007309[7] = -nom_x[7] + true_x[7];
   out_2253777461209007309[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_6173701301729052914) {
   out_6173701301729052914[0] = 1.0;
   out_6173701301729052914[1] = 0.0;
   out_6173701301729052914[2] = 0.0;
   out_6173701301729052914[3] = 0.0;
   out_6173701301729052914[4] = 0.0;
   out_6173701301729052914[5] = 0.0;
   out_6173701301729052914[6] = 0.0;
   out_6173701301729052914[7] = 0.0;
   out_6173701301729052914[8] = 0.0;
   out_6173701301729052914[9] = 0.0;
   out_6173701301729052914[10] = 1.0;
   out_6173701301729052914[11] = 0.0;
   out_6173701301729052914[12] = 0.0;
   out_6173701301729052914[13] = 0.0;
   out_6173701301729052914[14] = 0.0;
   out_6173701301729052914[15] = 0.0;
   out_6173701301729052914[16] = 0.0;
   out_6173701301729052914[17] = 0.0;
   out_6173701301729052914[18] = 0.0;
   out_6173701301729052914[19] = 0.0;
   out_6173701301729052914[20] = 1.0;
   out_6173701301729052914[21] = 0.0;
   out_6173701301729052914[22] = 0.0;
   out_6173701301729052914[23] = 0.0;
   out_6173701301729052914[24] = 0.0;
   out_6173701301729052914[25] = 0.0;
   out_6173701301729052914[26] = 0.0;
   out_6173701301729052914[27] = 0.0;
   out_6173701301729052914[28] = 0.0;
   out_6173701301729052914[29] = 0.0;
   out_6173701301729052914[30] = 1.0;
   out_6173701301729052914[31] = 0.0;
   out_6173701301729052914[32] = 0.0;
   out_6173701301729052914[33] = 0.0;
   out_6173701301729052914[34] = 0.0;
   out_6173701301729052914[35] = 0.0;
   out_6173701301729052914[36] = 0.0;
   out_6173701301729052914[37] = 0.0;
   out_6173701301729052914[38] = 0.0;
   out_6173701301729052914[39] = 0.0;
   out_6173701301729052914[40] = 1.0;
   out_6173701301729052914[41] = 0.0;
   out_6173701301729052914[42] = 0.0;
   out_6173701301729052914[43] = 0.0;
   out_6173701301729052914[44] = 0.0;
   out_6173701301729052914[45] = 0.0;
   out_6173701301729052914[46] = 0.0;
   out_6173701301729052914[47] = 0.0;
   out_6173701301729052914[48] = 0.0;
   out_6173701301729052914[49] = 0.0;
   out_6173701301729052914[50] = 1.0;
   out_6173701301729052914[51] = 0.0;
   out_6173701301729052914[52] = 0.0;
   out_6173701301729052914[53] = 0.0;
   out_6173701301729052914[54] = 0.0;
   out_6173701301729052914[55] = 0.0;
   out_6173701301729052914[56] = 0.0;
   out_6173701301729052914[57] = 0.0;
   out_6173701301729052914[58] = 0.0;
   out_6173701301729052914[59] = 0.0;
   out_6173701301729052914[60] = 1.0;
   out_6173701301729052914[61] = 0.0;
   out_6173701301729052914[62] = 0.0;
   out_6173701301729052914[63] = 0.0;
   out_6173701301729052914[64] = 0.0;
   out_6173701301729052914[65] = 0.0;
   out_6173701301729052914[66] = 0.0;
   out_6173701301729052914[67] = 0.0;
   out_6173701301729052914[68] = 0.0;
   out_6173701301729052914[69] = 0.0;
   out_6173701301729052914[70] = 1.0;
   out_6173701301729052914[71] = 0.0;
   out_6173701301729052914[72] = 0.0;
   out_6173701301729052914[73] = 0.0;
   out_6173701301729052914[74] = 0.0;
   out_6173701301729052914[75] = 0.0;
   out_6173701301729052914[76] = 0.0;
   out_6173701301729052914[77] = 0.0;
   out_6173701301729052914[78] = 0.0;
   out_6173701301729052914[79] = 0.0;
   out_6173701301729052914[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1182746567630340128) {
   out_1182746567630340128[0] = state[0];
   out_1182746567630340128[1] = state[1];
   out_1182746567630340128[2] = state[2];
   out_1182746567630340128[3] = state[3];
   out_1182746567630340128[4] = state[4];
   out_1182746567630340128[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1182746567630340128[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1182746567630340128[7] = state[7];
   out_1182746567630340128[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2889563820348071536) {
   out_2889563820348071536[0] = 1;
   out_2889563820348071536[1] = 0;
   out_2889563820348071536[2] = 0;
   out_2889563820348071536[3] = 0;
   out_2889563820348071536[4] = 0;
   out_2889563820348071536[5] = 0;
   out_2889563820348071536[6] = 0;
   out_2889563820348071536[7] = 0;
   out_2889563820348071536[8] = 0;
   out_2889563820348071536[9] = 0;
   out_2889563820348071536[10] = 1;
   out_2889563820348071536[11] = 0;
   out_2889563820348071536[12] = 0;
   out_2889563820348071536[13] = 0;
   out_2889563820348071536[14] = 0;
   out_2889563820348071536[15] = 0;
   out_2889563820348071536[16] = 0;
   out_2889563820348071536[17] = 0;
   out_2889563820348071536[18] = 0;
   out_2889563820348071536[19] = 0;
   out_2889563820348071536[20] = 1;
   out_2889563820348071536[21] = 0;
   out_2889563820348071536[22] = 0;
   out_2889563820348071536[23] = 0;
   out_2889563820348071536[24] = 0;
   out_2889563820348071536[25] = 0;
   out_2889563820348071536[26] = 0;
   out_2889563820348071536[27] = 0;
   out_2889563820348071536[28] = 0;
   out_2889563820348071536[29] = 0;
   out_2889563820348071536[30] = 1;
   out_2889563820348071536[31] = 0;
   out_2889563820348071536[32] = 0;
   out_2889563820348071536[33] = 0;
   out_2889563820348071536[34] = 0;
   out_2889563820348071536[35] = 0;
   out_2889563820348071536[36] = 0;
   out_2889563820348071536[37] = 0;
   out_2889563820348071536[38] = 0;
   out_2889563820348071536[39] = 0;
   out_2889563820348071536[40] = 1;
   out_2889563820348071536[41] = 0;
   out_2889563820348071536[42] = 0;
   out_2889563820348071536[43] = 0;
   out_2889563820348071536[44] = 0;
   out_2889563820348071536[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2889563820348071536[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2889563820348071536[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2889563820348071536[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2889563820348071536[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2889563820348071536[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2889563820348071536[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2889563820348071536[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2889563820348071536[53] = -9.8100000000000005*dt;
   out_2889563820348071536[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2889563820348071536[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2889563820348071536[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2889563820348071536[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2889563820348071536[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2889563820348071536[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2889563820348071536[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2889563820348071536[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2889563820348071536[62] = 0;
   out_2889563820348071536[63] = 0;
   out_2889563820348071536[64] = 0;
   out_2889563820348071536[65] = 0;
   out_2889563820348071536[66] = 0;
   out_2889563820348071536[67] = 0;
   out_2889563820348071536[68] = 0;
   out_2889563820348071536[69] = 0;
   out_2889563820348071536[70] = 1;
   out_2889563820348071536[71] = 0;
   out_2889563820348071536[72] = 0;
   out_2889563820348071536[73] = 0;
   out_2889563820348071536[74] = 0;
   out_2889563820348071536[75] = 0;
   out_2889563820348071536[76] = 0;
   out_2889563820348071536[77] = 0;
   out_2889563820348071536[78] = 0;
   out_2889563820348071536[79] = 0;
   out_2889563820348071536[80] = 1;
}
void h_25(double *state, double *unused, double *out_8024768959011990554) {
   out_8024768959011990554[0] = state[6];
}
void H_25(double *state, double *unused, double *out_8212170773881242654) {
   out_8212170773881242654[0] = 0;
   out_8212170773881242654[1] = 0;
   out_8212170773881242654[2] = 0;
   out_8212170773881242654[3] = 0;
   out_8212170773881242654[4] = 0;
   out_8212170773881242654[5] = 0;
   out_8212170773881242654[6] = 1;
   out_8212170773881242654[7] = 0;
   out_8212170773881242654[8] = 0;
}
void h_24(double *state, double *unused, double *out_7546482466903643346) {
   out_7546482466903643346[0] = state[4];
   out_7546482466903643346[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6524354338012138463) {
   out_6524354338012138463[0] = 0;
   out_6524354338012138463[1] = 0;
   out_6524354338012138463[2] = 0;
   out_6524354338012138463[3] = 0;
   out_6524354338012138463[4] = 1;
   out_6524354338012138463[5] = 0;
   out_6524354338012138463[6] = 0;
   out_6524354338012138463[7] = 0;
   out_6524354338012138463[8] = 0;
   out_6524354338012138463[9] = 0;
   out_6524354338012138463[10] = 0;
   out_6524354338012138463[11] = 0;
   out_6524354338012138463[12] = 0;
   out_6524354338012138463[13] = 0;
   out_6524354338012138463[14] = 1;
   out_6524354338012138463[15] = 0;
   out_6524354338012138463[16] = 0;
   out_6524354338012138463[17] = 0;
}
void h_30(double *state, double *unused, double *out_1057453794330958060) {
   out_1057453794330958060[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5706876969700700764) {
   out_5706876969700700764[0] = 0;
   out_5706876969700700764[1] = 0;
   out_5706876969700700764[2] = 0;
   out_5706876969700700764[3] = 0;
   out_5706876969700700764[4] = 1;
   out_5706876969700700764[5] = 0;
   out_5706876969700700764[6] = 0;
   out_5706876969700700764[7] = 0;
   out_5706876969700700764[8] = 0;
}
void h_26(double *state, double *unused, double *out_110290006745183677) {
   out_110290006745183677[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6493069980954252738) {
   out_6493069980954252738[0] = 0;
   out_6493069980954252738[1] = 0;
   out_6493069980954252738[2] = 0;
   out_6493069980954252738[3] = 0;
   out_6493069980954252738[4] = 0;
   out_6493069980954252738[5] = 0;
   out_6493069980954252738[6] = 0;
   out_6493069980954252738[7] = 1;
   out_6493069980954252738[8] = 0;
}
void h_27(double *state, double *unused, double *out_4022933597283711916) {
   out_4022933597283711916[0] = state[3];
}
void H_27(double *state, double *unused, double *out_3532113657900275853) {
   out_3532113657900275853[0] = 0;
   out_3532113657900275853[1] = 0;
   out_3532113657900275853[2] = 0;
   out_3532113657900275853[3] = 1;
   out_3532113657900275853[4] = 0;
   out_3532113657900275853[5] = 0;
   out_3532113657900275853[6] = 0;
   out_3532113657900275853[7] = 0;
   out_3532113657900275853[8] = 0;
}
void h_29(double *state, double *unused, double *out_4473727083980028076) {
   out_4473727083980028076[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6217108314015092948) {
   out_6217108314015092948[0] = 0;
   out_6217108314015092948[1] = 1;
   out_6217108314015092948[2] = 0;
   out_6217108314015092948[3] = 0;
   out_6217108314015092948[4] = 0;
   out_6217108314015092948[5] = 0;
   out_6217108314015092948[6] = 0;
   out_6217108314015092948[7] = 0;
   out_6217108314015092948[8] = 0;
}
void h_28(double *state, double *unused, double *out_8781080605344882865) {
   out_8781080605344882865[0] = state[0];
}
void H_28(double *state, double *unused, double *out_1134709296945562374) {
   out_1134709296945562374[0] = 1;
   out_1134709296945562374[1] = 0;
   out_1134709296945562374[2] = 0;
   out_1134709296945562374[3] = 0;
   out_1134709296945562374[4] = 0;
   out_1134709296945562374[5] = 0;
   out_1134709296945562374[6] = 0;
   out_1134709296945562374[7] = 0;
   out_1134709296945562374[8] = 0;
}
void h_31(double *state, double *unused, double *out_6784525586443538712) {
   out_6784525586443538712[0] = state[8];
}
void H_31(double *state, double *unused, double *out_5866861878720901262) {
   out_5866861878720901262[0] = 0;
   out_5866861878720901262[1] = 0;
   out_5866861878720901262[2] = 0;
   out_5866861878720901262[3] = 0;
   out_5866861878720901262[4] = 0;
   out_5866861878720901262[5] = 0;
   out_5866861878720901262[6] = 0;
   out_5866861878720901262[7] = 0;
   out_5866861878720901262[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7224272908344467420) {
  err_fun(nom_x, delta_x, out_7224272908344467420);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2253777461209007309) {
  inv_err_fun(nom_x, true_x, out_2253777461209007309);
}
void car_H_mod_fun(double *state, double *out_6173701301729052914) {
  H_mod_fun(state, out_6173701301729052914);
}
void car_f_fun(double *state, double dt, double *out_1182746567630340128) {
  f_fun(state,  dt, out_1182746567630340128);
}
void car_F_fun(double *state, double dt, double *out_2889563820348071536) {
  F_fun(state,  dt, out_2889563820348071536);
}
void car_h_25(double *state, double *unused, double *out_8024768959011990554) {
  h_25(state, unused, out_8024768959011990554);
}
void car_H_25(double *state, double *unused, double *out_8212170773881242654) {
  H_25(state, unused, out_8212170773881242654);
}
void car_h_24(double *state, double *unused, double *out_7546482466903643346) {
  h_24(state, unused, out_7546482466903643346);
}
void car_H_24(double *state, double *unused, double *out_6524354338012138463) {
  H_24(state, unused, out_6524354338012138463);
}
void car_h_30(double *state, double *unused, double *out_1057453794330958060) {
  h_30(state, unused, out_1057453794330958060);
}
void car_H_30(double *state, double *unused, double *out_5706876969700700764) {
  H_30(state, unused, out_5706876969700700764);
}
void car_h_26(double *state, double *unused, double *out_110290006745183677) {
  h_26(state, unused, out_110290006745183677);
}
void car_H_26(double *state, double *unused, double *out_6493069980954252738) {
  H_26(state, unused, out_6493069980954252738);
}
void car_h_27(double *state, double *unused, double *out_4022933597283711916) {
  h_27(state, unused, out_4022933597283711916);
}
void car_H_27(double *state, double *unused, double *out_3532113657900275853) {
  H_27(state, unused, out_3532113657900275853);
}
void car_h_29(double *state, double *unused, double *out_4473727083980028076) {
  h_29(state, unused, out_4473727083980028076);
}
void car_H_29(double *state, double *unused, double *out_6217108314015092948) {
  H_29(state, unused, out_6217108314015092948);
}
void car_h_28(double *state, double *unused, double *out_8781080605344882865) {
  h_28(state, unused, out_8781080605344882865);
}
void car_H_28(double *state, double *unused, double *out_1134709296945562374) {
  H_28(state, unused, out_1134709296945562374);
}
void car_h_31(double *state, double *unused, double *out_6784525586443538712) {
  h_31(state, unused, out_6784525586443538712);
}
void car_H_31(double *state, double *unused, double *out_5866861878720901262) {
  H_31(state, unused, out_5866861878720901262);
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
