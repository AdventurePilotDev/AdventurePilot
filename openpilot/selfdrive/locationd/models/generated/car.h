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
void car_err_fun(double *nom_x, double *delta_x, double *out_3208522223344719935);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3256968781919030142);
void car_H_mod_fun(double *state, double *out_8287908987357363226);
void car_f_fun(double *state, double dt, double *out_6824723206088421349);
void car_F_fun(double *state, double dt, double *out_7714489675589964189);
void car_h_25(double *state, double *unused, double *out_4089721760690641625);
void car_H_25(double *state, double *unused, double *out_8854495976160216770);
void car_h_24(double *state, double *unused, double *out_7265493314677083858);
void car_H_24(double *state, double *unused, double *out_6681846377154717204);
void car_h_30(double *state, double *unused, double *out_3932535092339512480);
void car_H_30(double *state, double *unused, double *out_4326799646032608572);
void car_h_26(double *state, double *unused, double *out_5532239809363198371);
void car_H_26(double *state, double *unused, double *out_5112992657286160546);
void car_h_27(double *state, double *unused, double *out_2309319756723207521);
void car_H_27(double *state, double *unused, double *out_6550393717216551789);
void car_h_29(double *state, double *unused, double *out_2466506425074336666);
void car_H_29(double *state, double *unused, double *out_4837030990347000756);
void car_h_28(double *state, double *unused, double *out_1880485542097265759);
void car_H_28(double *state, double *unused, double *out_6800661261912327007);
void car_h_31(double *state, double *unused, double *out_7586187086668559055);
void car_H_31(double *state, double *unused, double *out_8885141938037177198);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}