#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2812259937135269323);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7702513841393801961);
void pose_H_mod_fun(double *state, double *out_8578061460798452048);
void pose_f_fun(double *state, double dt, double *out_6960018690742428862);
void pose_F_fun(double *state, double dt, double *out_1981298987176351576);
void pose_h_4(double *state, double *unused, double *out_8862304430070200672);
void pose_H_4(double *state, double *unused, double *out_924320946853278226);
void pose_h_10(double *state, double *unused, double *out_2002575884586459867);
void pose_H_10(double *state, double *unused, double *out_9217408551414011913);
void pose_h_13(double *state, double *unused, double *out_942466340260082110);
void pose_H_13(double *state, double *unused, double *out_2909434516449245798);
void pose_h_14(double *state, double *unused, double *out_4372130999795494903);
void pose_H_14(double *state, double *unused, double *out_2158467485442094070);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}