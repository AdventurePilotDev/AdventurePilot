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
void car_err_fun(double *nom_x, double *delta_x, double *out_7902446637228762316);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8054518576183361196);
void car_H_mod_fun(double *state, double *out_223929158544482531);
void car_f_fun(double *state, double dt, double *out_608826670593016442);
void car_F_fun(double *state, double dt, double *out_4256277468531545359);
void car_h_25(double *state, double *unused, double *out_4488142222450972896);
void car_H_25(double *state, double *unused, double *out_9029626689252831848);
void car_h_24(double *state, double *unused, double *out_203195263897949964);
void car_H_24(double *state, double *unused, double *out_4283117362286322814);
void car_h_30(double *state, double *unused, double *out_7187766624908227784);
void car_H_30(double *state, double *unused, double *out_2500427042965103013);
void car_h_26(double *state, double *unused, double *out_8355510425984369521);
void car_H_26(double *state, double *unused, double *out_6112591414695919167);
void car_h_27(double *state, double *unused, double *out_513531045209872749);
void car_H_27(double *state, double *unused, double *out_4675190354765527924);
void car_h_29(double *state, double *unused, double *out_4375214521638374401);
void car_H_29(double *state, double *unused, double *out_6388553081635078957);
void car_h_28(double *state, double *unused, double *out_67861000273519612);
void car_H_28(double *state, double *unused, double *out_6975791975004942085);
void car_h_31(double *state, double *unused, double *out_7435868212806457857);
void car_H_31(double *state, double *unused, double *out_2340442133944902515);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}