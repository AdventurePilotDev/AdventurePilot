#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_1665288967890009071);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_164591839466453182);
void car_H_mod_fun(double *state, double *out_510204997013109862);
void car_f_fun(double *state, double dt, double *out_7311706861115613821);
void car_F_fun(double *state, double dt, double *out_271093834509954639);
void car_h_25(double *state, double *unused, double *out_1746172296850582351);
void car_H_25(double *state, double *unused, double *out_7007227956903139813);
void car_h_24(double *state, double *unused, double *out_2857798440202051777);
void car_H_24(double *state, double *unused, double *out_840126053970882249);
void car_h_30(double *state, double *unused, double *out_5844475595129954215);
void car_H_30(double *state, double *unused, double *out_6911819786678803605);
void car_h_26(double *state, double *unused, double *out_9120323407877410804);
void car_H_26(double *state, double *unused, double *out_7698012797932355579);
void car_h_27(double *state, double *unused, double *out_764088203515284239);
void car_H_27(double *state, double *unused, double *out_4737056474878378694);
void car_h_29(double *state, double *unused, double *out_6203226961165748255);
void car_H_29(double *state, double *unused, double *out_7422051130993195789);
void car_h_28(double *state, double *unused, double *out_1931832004591425976);
void car_H_28(double *state, double *unused, double *out_2339652113923665215);
void car_h_31(double *state, double *unused, double *out_8819299126104920080);
void car_H_31(double *state, double *unused, double *out_7071804695699004103);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}