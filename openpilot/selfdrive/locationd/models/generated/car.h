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
void car_err_fun(double *nom_x, double *delta_x, double *out_1063409489421528348);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2822415354599312061);
void car_H_mod_fun(double *state, double *out_3690717380999379952);
void car_f_fun(double *state, double dt, double *out_178324546167347638);
void car_F_fun(double *state, double dt, double *out_4049207523162442899);
void car_h_25(double *state, double *unused, double *out_73534705887621250);
void car_H_25(double *state, double *unused, double *out_6266657020481598421);
void car_h_24(double *state, double *unused, double *out_2670913920335148060);
void car_H_24(double *state, double *unused, double *out_1151734613195328472);
void car_h_30(double *state, double *unused, double *out_7966474200772144609);
void car_H_30(double *state, double *unused, double *out_6395995967624838491);
void car_h_26(double *state, double *unused, double *out_6606846669762774781);
void car_H_26(double *state, double *unused, double *out_8438583734353896971);
void car_h_27(double *state, double *unused, double *out_3383926617122783931);
void car_H_27(double *state, double *unused, double *out_8570759279425263402);
void car_h_29(double *state, double *unused, double *out_2964800933111298964);
void car_H_29(double *state, double *unused, double *out_5885764623310446307);
void car_h_28(double *state, double *unused, double *out_1617746650538061714);
void car_H_28(double *state, double *unused, double *out_8320491734729488184);
void car_h_31(double *state, double *unused, double *out_846568576137436437);
void car_H_31(double *state, double *unused, double *out_6236011058604637993);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}