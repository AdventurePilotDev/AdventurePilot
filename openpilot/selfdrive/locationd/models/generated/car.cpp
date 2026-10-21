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
void err_fun(double *nom_x, double *delta_x, double *out_8811977435261191108) {
   out_8811977435261191108[0] = delta_x[0] + nom_x[0];
   out_8811977435261191108[1] = delta_x[1] + nom_x[1];
   out_8811977435261191108[2] = delta_x[2] + nom_x[2];
   out_8811977435261191108[3] = delta_x[3] + nom_x[3];
   out_8811977435261191108[4] = delta_x[4] + nom_x[4];
   out_8811977435261191108[5] = delta_x[5] + nom_x[5];
   out_8811977435261191108[6] = delta_x[6] + nom_x[6];
   out_8811977435261191108[7] = delta_x[7] + nom_x[7];
   out_8811977435261191108[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6946685726979547511) {
   out_6946685726979547511[0] = -nom_x[0] + true_x[0];
   out_6946685726979547511[1] = -nom_x[1] + true_x[1];
   out_6946685726979547511[2] = -nom_x[2] + true_x[2];
   out_6946685726979547511[3] = -nom_x[3] + true_x[3];
   out_6946685726979547511[4] = -nom_x[4] + true_x[4];
   out_6946685726979547511[5] = -nom_x[5] + true_x[5];
   out_6946685726979547511[6] = -nom_x[6] + true_x[6];
   out_6946685726979547511[7] = -nom_x[7] + true_x[7];
   out_6946685726979547511[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1170278188409279072) {
   out_1170278188409279072[0] = 1.0;
   out_1170278188409279072[1] = 0.0;
   out_1170278188409279072[2] = 0.0;
   out_1170278188409279072[3] = 0.0;
   out_1170278188409279072[4] = 0.0;
   out_1170278188409279072[5] = 0.0;
   out_1170278188409279072[6] = 0.0;
   out_1170278188409279072[7] = 0.0;
   out_1170278188409279072[8] = 0.0;
   out_1170278188409279072[9] = 0.0;
   out_1170278188409279072[10] = 1.0;
   out_1170278188409279072[11] = 0.0;
   out_1170278188409279072[12] = 0.0;
   out_1170278188409279072[13] = 0.0;
   out_1170278188409279072[14] = 0.0;
   out_1170278188409279072[15] = 0.0;
   out_1170278188409279072[16] = 0.0;
   out_1170278188409279072[17] = 0.0;
   out_1170278188409279072[18] = 0.0;
   out_1170278188409279072[19] = 0.0;
   out_1170278188409279072[20] = 1.0;
   out_1170278188409279072[21] = 0.0;
   out_1170278188409279072[22] = 0.0;
   out_1170278188409279072[23] = 0.0;
   out_1170278188409279072[24] = 0.0;
   out_1170278188409279072[25] = 0.0;
   out_1170278188409279072[26] = 0.0;
   out_1170278188409279072[27] = 0.0;
   out_1170278188409279072[28] = 0.0;
   out_1170278188409279072[29] = 0.0;
   out_1170278188409279072[30] = 1.0;
   out_1170278188409279072[31] = 0.0;
   out_1170278188409279072[32] = 0.0;
   out_1170278188409279072[33] = 0.0;
   out_1170278188409279072[34] = 0.0;
   out_1170278188409279072[35] = 0.0;
   out_1170278188409279072[36] = 0.0;
   out_1170278188409279072[37] = 0.0;
   out_1170278188409279072[38] = 0.0;
   out_1170278188409279072[39] = 0.0;
   out_1170278188409279072[40] = 1.0;
   out_1170278188409279072[41] = 0.0;
   out_1170278188409279072[42] = 0.0;
   out_1170278188409279072[43] = 0.0;
   out_1170278188409279072[44] = 0.0;
   out_1170278188409279072[45] = 0.0;
   out_1170278188409279072[46] = 0.0;
   out_1170278188409279072[47] = 0.0;
   out_1170278188409279072[48] = 0.0;
   out_1170278188409279072[49] = 0.0;
   out_1170278188409279072[50] = 1.0;
   out_1170278188409279072[51] = 0.0;
   out_1170278188409279072[52] = 0.0;
   out_1170278188409279072[53] = 0.0;
   out_1170278188409279072[54] = 0.0;
   out_1170278188409279072[55] = 0.0;
   out_1170278188409279072[56] = 0.0;
   out_1170278188409279072[57] = 0.0;
   out_1170278188409279072[58] = 0.0;
   out_1170278188409279072[59] = 0.0;
   out_1170278188409279072[60] = 1.0;
   out_1170278188409279072[61] = 0.0;
   out_1170278188409279072[62] = 0.0;
   out_1170278188409279072[63] = 0.0;
   out_1170278188409279072[64] = 0.0;
   out_1170278188409279072[65] = 0.0;
   out_1170278188409279072[66] = 0.0;
   out_1170278188409279072[67] = 0.0;
   out_1170278188409279072[68] = 0.0;
   out_1170278188409279072[69] = 0.0;
   out_1170278188409279072[70] = 1.0;
   out_1170278188409279072[71] = 0.0;
   out_1170278188409279072[72] = 0.0;
   out_1170278188409279072[73] = 0.0;
   out_1170278188409279072[74] = 0.0;
   out_1170278188409279072[75] = 0.0;
   out_1170278188409279072[76] = 0.0;
   out_1170278188409279072[77] = 0.0;
   out_1170278188409279072[78] = 0.0;
   out_1170278188409279072[79] = 0.0;
   out_1170278188409279072[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1916089704721153233) {
   out_1916089704721153233[0] = state[0];
   out_1916089704721153233[1] = state[1];
   out_1916089704721153233[2] = state[2];
   out_1916089704721153233[3] = state[3];
   out_1916089704721153233[4] = state[4];
   out_1916089704721153233[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1916089704721153233[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1916089704721153233[7] = state[7];
   out_1916089704721153233[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4987018272475214840) {
   out_4987018272475214840[0] = 1;
   out_4987018272475214840[1] = 0;
   out_4987018272475214840[2] = 0;
   out_4987018272475214840[3] = 0;
   out_4987018272475214840[4] = 0;
   out_4987018272475214840[5] = 0;
   out_4987018272475214840[6] = 0;
   out_4987018272475214840[7] = 0;
   out_4987018272475214840[8] = 0;
   out_4987018272475214840[9] = 0;
   out_4987018272475214840[10] = 1;
   out_4987018272475214840[11] = 0;
   out_4987018272475214840[12] = 0;
   out_4987018272475214840[13] = 0;
   out_4987018272475214840[14] = 0;
   out_4987018272475214840[15] = 0;
   out_4987018272475214840[16] = 0;
   out_4987018272475214840[17] = 0;
   out_4987018272475214840[18] = 0;
   out_4987018272475214840[19] = 0;
   out_4987018272475214840[20] = 1;
   out_4987018272475214840[21] = 0;
   out_4987018272475214840[22] = 0;
   out_4987018272475214840[23] = 0;
   out_4987018272475214840[24] = 0;
   out_4987018272475214840[25] = 0;
   out_4987018272475214840[26] = 0;
   out_4987018272475214840[27] = 0;
   out_4987018272475214840[28] = 0;
   out_4987018272475214840[29] = 0;
   out_4987018272475214840[30] = 1;
   out_4987018272475214840[31] = 0;
   out_4987018272475214840[32] = 0;
   out_4987018272475214840[33] = 0;
   out_4987018272475214840[34] = 0;
   out_4987018272475214840[35] = 0;
   out_4987018272475214840[36] = 0;
   out_4987018272475214840[37] = 0;
   out_4987018272475214840[38] = 0;
   out_4987018272475214840[39] = 0;
   out_4987018272475214840[40] = 1;
   out_4987018272475214840[41] = 0;
   out_4987018272475214840[42] = 0;
   out_4987018272475214840[43] = 0;
   out_4987018272475214840[44] = 0;
   out_4987018272475214840[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4987018272475214840[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4987018272475214840[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4987018272475214840[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4987018272475214840[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4987018272475214840[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4987018272475214840[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4987018272475214840[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4987018272475214840[53] = -9.8100000000000005*dt;
   out_4987018272475214840[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4987018272475214840[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4987018272475214840[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4987018272475214840[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4987018272475214840[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4987018272475214840[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4987018272475214840[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4987018272475214840[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4987018272475214840[62] = 0;
   out_4987018272475214840[63] = 0;
   out_4987018272475214840[64] = 0;
   out_4987018272475214840[65] = 0;
   out_4987018272475214840[66] = 0;
   out_4987018272475214840[67] = 0;
   out_4987018272475214840[68] = 0;
   out_4987018272475214840[69] = 0;
   out_4987018272475214840[70] = 1;
   out_4987018272475214840[71] = 0;
   out_4987018272475214840[72] = 0;
   out_4987018272475214840[73] = 0;
   out_4987018272475214840[74] = 0;
   out_4987018272475214840[75] = 0;
   out_4987018272475214840[76] = 0;
   out_4987018272475214840[77] = 0;
   out_4987018272475214840[78] = 0;
   out_4987018272475214840[79] = 0;
   out_4987018272475214840[80] = 1;
}
void h_25(double *state, double *unused, double *out_3532977926145256286) {
   out_3532977926145256286[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7849974308245583983) {
   out_7849974308245583983[0] = 0;
   out_7849974308245583983[1] = 0;
   out_7849974308245583983[2] = 0;
   out_7849974308245583983[3] = 0;
   out_7849974308245583983[4] = 0;
   out_7849974308245583983[5] = 0;
   out_7849974308245583983[6] = 1;
   out_7849974308245583983[7] = 0;
   out_7849974308245583983[8] = 0;
}
void h_24(double *state, double *unused, double *out_3307929210016927272) {
   out_3307929210016927272[0] = state[4];
   out_3307929210016927272[1] = state[5];
}
void H_24(double *state, double *unused, double *out_5677324709240084417) {
   out_5677324709240084417[0] = 0;
   out_5677324709240084417[1] = 0;
   out_5677324709240084417[2] = 0;
   out_5677324709240084417[3] = 0;
   out_5677324709240084417[4] = 1;
   out_5677324709240084417[5] = 0;
   out_5677324709240084417[6] = 0;
   out_5677324709240084417[7] = 0;
   out_5677324709240084417[8] = 0;
   out_5677324709240084417[9] = 0;
   out_5677324709240084417[10] = 0;
   out_5677324709240084417[11] = 0;
   out_5677324709240084417[12] = 0;
   out_5677324709240084417[13] = 0;
   out_5677324709240084417[14] = 1;
   out_5677324709240084417[15] = 0;
   out_5677324709240084417[16] = 0;
   out_5677324709240084417[17] = 0;
}
void h_30(double *state, double *unused, double *out_3237857300205094650) {
   out_3237857300205094650[0] = state[4];
}
void H_30(double *state, double *unused, double *out_3322277978117975785) {
   out_3322277978117975785[0] = 0;
   out_3322277978117975785[1] = 0;
   out_3322277978117975785[2] = 0;
   out_3322277978117975785[3] = 0;
   out_3322277978117975785[4] = 1;
   out_3322277978117975785[5] = 0;
   out_3322277978117975785[6] = 0;
   out_3322277978117975785[7] = 0;
   out_3322277978117975785[8] = 0;
}
void h_26(double *state, double *unused, double *out_8239530567865940295) {
   out_8239530567865940295[0] = state[7];
}
void H_26(double *state, double *unused, double *out_4108470989371527759) {
   out_4108470989371527759[0] = 0;
   out_4108470989371527759[1] = 0;
   out_4108470989371527759[2] = 0;
   out_4108470989371527759[3] = 0;
   out_4108470989371527759[4] = 0;
   out_4108470989371527759[5] = 0;
   out_4108470989371527759[6] = 0;
   out_4108470989371527759[7] = 1;
   out_4108470989371527759[8] = 0;
}
void h_27(double *state, double *unused, double *out_2589468073243593851) {
   out_2589468073243593851[0] = state[3];
}
void H_27(double *state, double *unused, double *out_1147514666317550874) {
   out_1147514666317550874[0] = 0;
   out_1147514666317550874[1] = 0;
   out_1147514666317550874[2] = 0;
   out_1147514666317550874[3] = 1;
   out_1147514666317550874[4] = 0;
   out_1147514666317550874[5] = 0;
   out_1147514666317550874[6] = 0;
   out_1147514666317550874[7] = 0;
   out_1147514666317550874[8] = 0;
}
void h_29(double *state, double *unused, double *out_8809845256090607820) {
   out_8809845256090607820[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3832509322432367969) {
   out_3832509322432367969[0] = 0;
   out_3832509322432367969[1] = 1;
   out_3832509322432367969[2] = 0;
   out_3832509322432367969[3] = 0;
   out_3832509322432367969[4] = 0;
   out_3832509322432367969[5] = 0;
   out_3832509322432367969[6] = 0;
   out_3832509322432367969[7] = 0;
   out_3832509322432367969[8] = 0;
}
void h_28(double *state, double *unused, double *out_7591141340904439496) {
   out_7591141340904439496[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5796139593997694220) {
   out_5796139593997694220[0] = 1;
   out_5796139593997694220[1] = 0;
   out_5796139593997694220[2] = 0;
   out_5796139593997694220[3] = 0;
   out_5796139593997694220[4] = 0;
   out_5796139593997694220[5] = 0;
   out_5796139593997694220[6] = 0;
   out_5796139593997694220[7] = 0;
   out_5796139593997694220[8] = 0;
}
void h_31(double *state, double *unused, double *out_6273888670845753446) {
   out_6273888670845753446[0] = state[8];
}
void H_31(double *state, double *unused, double *out_3482262887138176283) {
   out_3482262887138176283[0] = 0;
   out_3482262887138176283[1] = 0;
   out_3482262887138176283[2] = 0;
   out_3482262887138176283[3] = 0;
   out_3482262887138176283[4] = 0;
   out_3482262887138176283[5] = 0;
   out_3482262887138176283[6] = 0;
   out_3482262887138176283[7] = 0;
   out_3482262887138176283[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_8811977435261191108) {
  err_fun(nom_x, delta_x, out_8811977435261191108);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6946685726979547511) {
  inv_err_fun(nom_x, true_x, out_6946685726979547511);
}
void car_H_mod_fun(double *state, double *out_1170278188409279072) {
  H_mod_fun(state, out_1170278188409279072);
}
void car_f_fun(double *state, double dt, double *out_1916089704721153233) {
  f_fun(state,  dt, out_1916089704721153233);
}
void car_F_fun(double *state, double dt, double *out_4987018272475214840) {
  F_fun(state,  dt, out_4987018272475214840);
}
void car_h_25(double *state, double *unused, double *out_3532977926145256286) {
  h_25(state, unused, out_3532977926145256286);
}
void car_H_25(double *state, double *unused, double *out_7849974308245583983) {
  H_25(state, unused, out_7849974308245583983);
}
void car_h_24(double *state, double *unused, double *out_3307929210016927272) {
  h_24(state, unused, out_3307929210016927272);
}
void car_H_24(double *state, double *unused, double *out_5677324709240084417) {
  H_24(state, unused, out_5677324709240084417);
}
void car_h_30(double *state, double *unused, double *out_3237857300205094650) {
  h_30(state, unused, out_3237857300205094650);
}
void car_H_30(double *state, double *unused, double *out_3322277978117975785) {
  H_30(state, unused, out_3322277978117975785);
}
void car_h_26(double *state, double *unused, double *out_8239530567865940295) {
  h_26(state, unused, out_8239530567865940295);
}
void car_H_26(double *state, double *unused, double *out_4108470989371527759) {
  H_26(state, unused, out_4108470989371527759);
}
void car_h_27(double *state, double *unused, double *out_2589468073243593851) {
  h_27(state, unused, out_2589468073243593851);
}
void car_H_27(double *state, double *unused, double *out_1147514666317550874) {
  H_27(state, unused, out_1147514666317550874);
}
void car_h_29(double *state, double *unused, double *out_8809845256090607820) {
  h_29(state, unused, out_8809845256090607820);
}
void car_H_29(double *state, double *unused, double *out_3832509322432367969) {
  H_29(state, unused, out_3832509322432367969);
}
void car_h_28(double *state, double *unused, double *out_7591141340904439496) {
  h_28(state, unused, out_7591141340904439496);
}
void car_H_28(double *state, double *unused, double *out_5796139593997694220) {
  H_28(state, unused, out_5796139593997694220);
}
void car_h_31(double *state, double *unused, double *out_6273888670845753446) {
  h_31(state, unused, out_6273888670845753446);
}
void car_H_31(double *state, double *unused, double *out_3482262887138176283) {
  H_31(state, unused, out_3482262887138176283);
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
