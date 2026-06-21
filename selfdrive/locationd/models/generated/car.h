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
void car_err_fun(double *nom_x, double *delta_x, double *out_8830503593500460991);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8937646500386039140);
void car_H_mod_fun(double *state, double *out_2651136907830874002);
void car_f_fun(double *state, double dt, double *out_391547981499403636);
void car_F_fun(double *state, double dt, double *out_6651794259104444489);
void car_h_25(double *state, double *unused, double *out_989584383030050329);
void car_H_25(double *state, double *unused, double *out_2143693935096995107);
void car_h_24(double *state, double *unused, double *out_3325235274672278762);
void car_H_24(double *state, double *unused, double *out_7158183428661679347);
void car_h_30(double *state, double *unused, double *out_8870857033784099514);
void car_H_30(double *state, double *unused, double *out_374639023410253520);
void car_h_26(double *state, double *unused, double *out_3869183766123253869);
void car_H_26(double *state, double *unused, double *out_5885197253971051331);
void car_h_27(double *state, double *unused, double *out_1592900267706527846);
void car_H_27(double *state, double *unused, double *out_1800124288390171391);
void car_h_29(double *state, double *unused, double *out_5230349795960550196);
void car_H_29(double *state, double *unused, double *out_884870367724645704);
void car_h_28(double *state, double *unused, double *out_649390414804865411);
void car_H_28(double *state, double *unused, double *out_4197528649344884870);
void car_h_31(double *state, double *unused, double *out_8752849639850722770);
void car_H_31(double *state, double *unused, double *out_6511405356204402807);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}