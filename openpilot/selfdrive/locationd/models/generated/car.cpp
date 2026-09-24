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
void err_fun(double *nom_x, double *delta_x, double *out_1665288967890009071) {
   out_1665288967890009071[0] = delta_x[0] + nom_x[0];
   out_1665288967890009071[1] = delta_x[1] + nom_x[1];
   out_1665288967890009071[2] = delta_x[2] + nom_x[2];
   out_1665288967890009071[3] = delta_x[3] + nom_x[3];
   out_1665288967890009071[4] = delta_x[4] + nom_x[4];
   out_1665288967890009071[5] = delta_x[5] + nom_x[5];
   out_1665288967890009071[6] = delta_x[6] + nom_x[6];
   out_1665288967890009071[7] = delta_x[7] + nom_x[7];
   out_1665288967890009071[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_164591839466453182) {
   out_164591839466453182[0] = -nom_x[0] + true_x[0];
   out_164591839466453182[1] = -nom_x[1] + true_x[1];
   out_164591839466453182[2] = -nom_x[2] + true_x[2];
   out_164591839466453182[3] = -nom_x[3] + true_x[3];
   out_164591839466453182[4] = -nom_x[4] + true_x[4];
   out_164591839466453182[5] = -nom_x[5] + true_x[5];
   out_164591839466453182[6] = -nom_x[6] + true_x[6];
   out_164591839466453182[7] = -nom_x[7] + true_x[7];
   out_164591839466453182[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_510204997013109862) {
   out_510204997013109862[0] = 1.0;
   out_510204997013109862[1] = 0.0;
   out_510204997013109862[2] = 0.0;
   out_510204997013109862[3] = 0.0;
   out_510204997013109862[4] = 0.0;
   out_510204997013109862[5] = 0.0;
   out_510204997013109862[6] = 0.0;
   out_510204997013109862[7] = 0.0;
   out_510204997013109862[8] = 0.0;
   out_510204997013109862[9] = 0.0;
   out_510204997013109862[10] = 1.0;
   out_510204997013109862[11] = 0.0;
   out_510204997013109862[12] = 0.0;
   out_510204997013109862[13] = 0.0;
   out_510204997013109862[14] = 0.0;
   out_510204997013109862[15] = 0.0;
   out_510204997013109862[16] = 0.0;
   out_510204997013109862[17] = 0.0;
   out_510204997013109862[18] = 0.0;
   out_510204997013109862[19] = 0.0;
   out_510204997013109862[20] = 1.0;
   out_510204997013109862[21] = 0.0;
   out_510204997013109862[22] = 0.0;
   out_510204997013109862[23] = 0.0;
   out_510204997013109862[24] = 0.0;
   out_510204997013109862[25] = 0.0;
   out_510204997013109862[26] = 0.0;
   out_510204997013109862[27] = 0.0;
   out_510204997013109862[28] = 0.0;
   out_510204997013109862[29] = 0.0;
   out_510204997013109862[30] = 1.0;
   out_510204997013109862[31] = 0.0;
   out_510204997013109862[32] = 0.0;
   out_510204997013109862[33] = 0.0;
   out_510204997013109862[34] = 0.0;
   out_510204997013109862[35] = 0.0;
   out_510204997013109862[36] = 0.0;
   out_510204997013109862[37] = 0.0;
   out_510204997013109862[38] = 0.0;
   out_510204997013109862[39] = 0.0;
   out_510204997013109862[40] = 1.0;
   out_510204997013109862[41] = 0.0;
   out_510204997013109862[42] = 0.0;
   out_510204997013109862[43] = 0.0;
   out_510204997013109862[44] = 0.0;
   out_510204997013109862[45] = 0.0;
   out_510204997013109862[46] = 0.0;
   out_510204997013109862[47] = 0.0;
   out_510204997013109862[48] = 0.0;
   out_510204997013109862[49] = 0.0;
   out_510204997013109862[50] = 1.0;
   out_510204997013109862[51] = 0.0;
   out_510204997013109862[52] = 0.0;
   out_510204997013109862[53] = 0.0;
   out_510204997013109862[54] = 0.0;
   out_510204997013109862[55] = 0.0;
   out_510204997013109862[56] = 0.0;
   out_510204997013109862[57] = 0.0;
   out_510204997013109862[58] = 0.0;
   out_510204997013109862[59] = 0.0;
   out_510204997013109862[60] = 1.0;
   out_510204997013109862[61] = 0.0;
   out_510204997013109862[62] = 0.0;
   out_510204997013109862[63] = 0.0;
   out_510204997013109862[64] = 0.0;
   out_510204997013109862[65] = 0.0;
   out_510204997013109862[66] = 0.0;
   out_510204997013109862[67] = 0.0;
   out_510204997013109862[68] = 0.0;
   out_510204997013109862[69] = 0.0;
   out_510204997013109862[70] = 1.0;
   out_510204997013109862[71] = 0.0;
   out_510204997013109862[72] = 0.0;
   out_510204997013109862[73] = 0.0;
   out_510204997013109862[74] = 0.0;
   out_510204997013109862[75] = 0.0;
   out_510204997013109862[76] = 0.0;
   out_510204997013109862[77] = 0.0;
   out_510204997013109862[78] = 0.0;
   out_510204997013109862[79] = 0.0;
   out_510204997013109862[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_7311706861115613821) {
   out_7311706861115613821[0] = state[0];
   out_7311706861115613821[1] = state[1];
   out_7311706861115613821[2] = state[2];
   out_7311706861115613821[3] = state[3];
   out_7311706861115613821[4] = state[4];
   out_7311706861115613821[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_7311706861115613821[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_7311706861115613821[7] = state[7];
   out_7311706861115613821[8] = state[8];
}
void F_fun(double *state, double dt, double *out_271093834509954639) {
   out_271093834509954639[0] = 1;
   out_271093834509954639[1] = 0;
   out_271093834509954639[2] = 0;
   out_271093834509954639[3] = 0;
   out_271093834509954639[4] = 0;
   out_271093834509954639[5] = 0;
   out_271093834509954639[6] = 0;
   out_271093834509954639[7] = 0;
   out_271093834509954639[8] = 0;
   out_271093834509954639[9] = 0;
   out_271093834509954639[10] = 1;
   out_271093834509954639[11] = 0;
   out_271093834509954639[12] = 0;
   out_271093834509954639[13] = 0;
   out_271093834509954639[14] = 0;
   out_271093834509954639[15] = 0;
   out_271093834509954639[16] = 0;
   out_271093834509954639[17] = 0;
   out_271093834509954639[18] = 0;
   out_271093834509954639[19] = 0;
   out_271093834509954639[20] = 1;
   out_271093834509954639[21] = 0;
   out_271093834509954639[22] = 0;
   out_271093834509954639[23] = 0;
   out_271093834509954639[24] = 0;
   out_271093834509954639[25] = 0;
   out_271093834509954639[26] = 0;
   out_271093834509954639[27] = 0;
   out_271093834509954639[28] = 0;
   out_271093834509954639[29] = 0;
   out_271093834509954639[30] = 1;
   out_271093834509954639[31] = 0;
   out_271093834509954639[32] = 0;
   out_271093834509954639[33] = 0;
   out_271093834509954639[34] = 0;
   out_271093834509954639[35] = 0;
   out_271093834509954639[36] = 0;
   out_271093834509954639[37] = 0;
   out_271093834509954639[38] = 0;
   out_271093834509954639[39] = 0;
   out_271093834509954639[40] = 1;
   out_271093834509954639[41] = 0;
   out_271093834509954639[42] = 0;
   out_271093834509954639[43] = 0;
   out_271093834509954639[44] = 0;
   out_271093834509954639[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_271093834509954639[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_271093834509954639[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_271093834509954639[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_271093834509954639[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_271093834509954639[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_271093834509954639[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_271093834509954639[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_271093834509954639[53] = -9.8100000000000005*dt;
   out_271093834509954639[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_271093834509954639[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_271093834509954639[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_271093834509954639[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_271093834509954639[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_271093834509954639[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_271093834509954639[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_271093834509954639[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_271093834509954639[62] = 0;
   out_271093834509954639[63] = 0;
   out_271093834509954639[64] = 0;
   out_271093834509954639[65] = 0;
   out_271093834509954639[66] = 0;
   out_271093834509954639[67] = 0;
   out_271093834509954639[68] = 0;
   out_271093834509954639[69] = 0;
   out_271093834509954639[70] = 1;
   out_271093834509954639[71] = 0;
   out_271093834509954639[72] = 0;
   out_271093834509954639[73] = 0;
   out_271093834509954639[74] = 0;
   out_271093834509954639[75] = 0;
   out_271093834509954639[76] = 0;
   out_271093834509954639[77] = 0;
   out_271093834509954639[78] = 0;
   out_271093834509954639[79] = 0;
   out_271093834509954639[80] = 1;
}
void h_25(double *state, double *unused, double *out_1746172296850582351) {
   out_1746172296850582351[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7007227956903139813) {
   out_7007227956903139813[0] = 0;
   out_7007227956903139813[1] = 0;
   out_7007227956903139813[2] = 0;
   out_7007227956903139813[3] = 0;
   out_7007227956903139813[4] = 0;
   out_7007227956903139813[5] = 0;
   out_7007227956903139813[6] = 1;
   out_7007227956903139813[7] = 0;
   out_7007227956903139813[8] = 0;
}
void h_24(double *state, double *unused, double *out_2857798440202051777) {
   out_2857798440202051777[0] = state[4];
   out_2857798440202051777[1] = state[5];
}
void H_24(double *state, double *unused, double *out_840126053970882249) {
   out_840126053970882249[0] = 0;
   out_840126053970882249[1] = 0;
   out_840126053970882249[2] = 0;
   out_840126053970882249[3] = 0;
   out_840126053970882249[4] = 1;
   out_840126053970882249[5] = 0;
   out_840126053970882249[6] = 0;
   out_840126053970882249[7] = 0;
   out_840126053970882249[8] = 0;
   out_840126053970882249[9] = 0;
   out_840126053970882249[10] = 0;
   out_840126053970882249[11] = 0;
   out_840126053970882249[12] = 0;
   out_840126053970882249[13] = 0;
   out_840126053970882249[14] = 1;
   out_840126053970882249[15] = 0;
   out_840126053970882249[16] = 0;
   out_840126053970882249[17] = 0;
}
void h_30(double *state, double *unused, double *out_5844475595129954215) {
   out_5844475595129954215[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6911819786678803605) {
   out_6911819786678803605[0] = 0;
   out_6911819786678803605[1] = 0;
   out_6911819786678803605[2] = 0;
   out_6911819786678803605[3] = 0;
   out_6911819786678803605[4] = 1;
   out_6911819786678803605[5] = 0;
   out_6911819786678803605[6] = 0;
   out_6911819786678803605[7] = 0;
   out_6911819786678803605[8] = 0;
}
void h_26(double *state, double *unused, double *out_9120323407877410804) {
   out_9120323407877410804[0] = state[7];
}
void H_26(double *state, double *unused, double *out_7698012797932355579) {
   out_7698012797932355579[0] = 0;
   out_7698012797932355579[1] = 0;
   out_7698012797932355579[2] = 0;
   out_7698012797932355579[3] = 0;
   out_7698012797932355579[4] = 0;
   out_7698012797932355579[5] = 0;
   out_7698012797932355579[6] = 0;
   out_7698012797932355579[7] = 1;
   out_7698012797932355579[8] = 0;
}
void h_27(double *state, double *unused, double *out_764088203515284239) {
   out_764088203515284239[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4737056474878378694) {
   out_4737056474878378694[0] = 0;
   out_4737056474878378694[1] = 0;
   out_4737056474878378694[2] = 0;
   out_4737056474878378694[3] = 1;
   out_4737056474878378694[4] = 0;
   out_4737056474878378694[5] = 0;
   out_4737056474878378694[6] = 0;
   out_4737056474878378694[7] = 0;
   out_4737056474878378694[8] = 0;
}
void h_29(double *state, double *unused, double *out_6203226961165748255) {
   out_6203226961165748255[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7422051130993195789) {
   out_7422051130993195789[0] = 0;
   out_7422051130993195789[1] = 1;
   out_7422051130993195789[2] = 0;
   out_7422051130993195789[3] = 0;
   out_7422051130993195789[4] = 0;
   out_7422051130993195789[5] = 0;
   out_7422051130993195789[6] = 0;
   out_7422051130993195789[7] = 0;
   out_7422051130993195789[8] = 0;
}
void h_28(double *state, double *unused, double *out_1931832004591425976) {
   out_1931832004591425976[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2339652113923665215) {
   out_2339652113923665215[0] = 1;
   out_2339652113923665215[1] = 0;
   out_2339652113923665215[2] = 0;
   out_2339652113923665215[3] = 0;
   out_2339652113923665215[4] = 0;
   out_2339652113923665215[5] = 0;
   out_2339652113923665215[6] = 0;
   out_2339652113923665215[7] = 0;
   out_2339652113923665215[8] = 0;
}
void h_31(double *state, double *unused, double *out_8819299126104920080) {
   out_8819299126104920080[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7071804695699004103) {
   out_7071804695699004103[0] = 0;
   out_7071804695699004103[1] = 0;
   out_7071804695699004103[2] = 0;
   out_7071804695699004103[3] = 0;
   out_7071804695699004103[4] = 0;
   out_7071804695699004103[5] = 0;
   out_7071804695699004103[6] = 0;
   out_7071804695699004103[7] = 0;
   out_7071804695699004103[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_1665288967890009071) {
  err_fun(nom_x, delta_x, out_1665288967890009071);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_164591839466453182) {
  inv_err_fun(nom_x, true_x, out_164591839466453182);
}
void car_H_mod_fun(double *state, double *out_510204997013109862) {
  H_mod_fun(state, out_510204997013109862);
}
void car_f_fun(double *state, double dt, double *out_7311706861115613821) {
  f_fun(state,  dt, out_7311706861115613821);
}
void car_F_fun(double *state, double dt, double *out_271093834509954639) {
  F_fun(state,  dt, out_271093834509954639);
}
void car_h_25(double *state, double *unused, double *out_1746172296850582351) {
  h_25(state, unused, out_1746172296850582351);
}
void car_H_25(double *state, double *unused, double *out_7007227956903139813) {
  H_25(state, unused, out_7007227956903139813);
}
void car_h_24(double *state, double *unused, double *out_2857798440202051777) {
  h_24(state, unused, out_2857798440202051777);
}
void car_H_24(double *state, double *unused, double *out_840126053970882249) {
  H_24(state, unused, out_840126053970882249);
}
void car_h_30(double *state, double *unused, double *out_5844475595129954215) {
  h_30(state, unused, out_5844475595129954215);
}
void car_H_30(double *state, double *unused, double *out_6911819786678803605) {
  H_30(state, unused, out_6911819786678803605);
}
void car_h_26(double *state, double *unused, double *out_9120323407877410804) {
  h_26(state, unused, out_9120323407877410804);
}
void car_H_26(double *state, double *unused, double *out_7698012797932355579) {
  H_26(state, unused, out_7698012797932355579);
}
void car_h_27(double *state, double *unused, double *out_764088203515284239) {
  h_27(state, unused, out_764088203515284239);
}
void car_H_27(double *state, double *unused, double *out_4737056474878378694) {
  H_27(state, unused, out_4737056474878378694);
}
void car_h_29(double *state, double *unused, double *out_6203226961165748255) {
  h_29(state, unused, out_6203226961165748255);
}
void car_H_29(double *state, double *unused, double *out_7422051130993195789) {
  H_29(state, unused, out_7422051130993195789);
}
void car_h_28(double *state, double *unused, double *out_1931832004591425976) {
  h_28(state, unused, out_1931832004591425976);
}
void car_H_28(double *state, double *unused, double *out_2339652113923665215) {
  H_28(state, unused, out_2339652113923665215);
}
void car_h_31(double *state, double *unused, double *out_8819299126104920080) {
  h_31(state, unused, out_8819299126104920080);
}
void car_H_31(double *state, double *unused, double *out_7071804695699004103) {
  H_31(state, unused, out_7071804695699004103);
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
