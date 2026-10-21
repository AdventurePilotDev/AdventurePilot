#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_5042717407786303213);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1689930166948831822);
void pose_H_mod_fun(double *state, double *out_3910683721715691775);
void pose_f_fun(double *state, double dt, double *out_6246311212408209668);
void pose_F_fun(double *state, double dt, double *out_3355224291126807009);
void pose_h_4(double *state, double *unused, double *out_3011476903633263124);
void pose_H_4(double *state, double *unused, double *out_4955044292825461911);
void pose_h_10(double *state, double *unused, double *out_4355309041220408349);
void pose_H_10(double *state, double *unused, double *out_3909620274325923014);
void pose_h_13(double *state, double *unused, double *out_6750839247096495864);
void pose_H_13(double *state, double *unused, double *out_2655586915491239018);
void pose_h_14(double *state, double *unused, double *out_1463673518145024401);
void pose_H_14(double *state, double *unused, double *out_991803436485977382);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}