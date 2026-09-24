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
void car_err_fun(double *nom_x, double *delta_x, double *out_8266163715671326717);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7126845649854226755);
void car_H_mod_fun(double *state, double *out_4330413888236142730);
void car_f_fun(double *state, double dt, double *out_5168037528944651468);
void car_F_fun(double *state, double dt, double *out_4416067367961397713);
void car_h_25(double *state, double *unused, double *out_4710681085674461856);
void car_H_25(double *state, double *unused, double *out_573277936948522110);
void car_h_24(double *state, double *unused, double *out_6957739660515425269);
void car_H_24(double *state, double *unused, double *out_7148849743540040211);
void car_h_30(double *state, double *unused, double *out_6161312478303345023);
void car_H_30(double *state, double *unused, double *out_7489968278440138865);
void car_h_26(double *state, double *unused, double *out_5217802625401682588);
void car_H_26(double *state, double *unused, double *out_3168225381925534114);
void car_h_27(double *state, double *unused, double *out_6300919553606388947);
void car_H_27(double *state, double *unused, double *out_5315204966639713954);
void car_h_29(double *state, double *unused, double *out_1159639210642499378);
void car_H_29(double *state, double *unused, double *out_3601842239770162921);
void car_h_28(double *state, double *unused, double *out_3920748180972170598);
void car_H_28(double *state, double *unused, double *out_1480556777299367653);
void car_h_31(double *state, double *unused, double *out_2338203242308479048);
void car_H_31(double *state, double *unused, double *out_603923898825482538);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}