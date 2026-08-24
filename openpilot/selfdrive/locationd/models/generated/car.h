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
void car_err_fun(double *nom_x, double *delta_x, double *out_7224272908344467420);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2253777461209007309);
void car_H_mod_fun(double *state, double *out_6173701301729052914);
void car_f_fun(double *state, double dt, double *out_1182746567630340128);
void car_F_fun(double *state, double dt, double *out_2889563820348071536);
void car_h_25(double *state, double *unused, double *out_8024768959011990554);
void car_H_25(double *state, double *unused, double *out_8212170773881242654);
void car_h_24(double *state, double *unused, double *out_7546482466903643346);
void car_H_24(double *state, double *unused, double *out_6524354338012138463);
void car_h_30(double *state, double *unused, double *out_1057453794330958060);
void car_H_30(double *state, double *unused, double *out_5706876969700700764);
void car_h_26(double *state, double *unused, double *out_110290006745183677);
void car_H_26(double *state, double *unused, double *out_6493069980954252738);
void car_h_27(double *state, double *unused, double *out_4022933597283711916);
void car_H_27(double *state, double *unused, double *out_3532113657900275853);
void car_h_29(double *state, double *unused, double *out_4473727083980028076);
void car_H_29(double *state, double *unused, double *out_6217108314015092948);
void car_h_28(double *state, double *unused, double *out_8781080605344882865);
void car_H_28(double *state, double *unused, double *out_1134709296945562374);
void car_h_31(double *state, double *unused, double *out_6784525586443538712);
void car_H_31(double *state, double *unused, double *out_5866861878720901262);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}