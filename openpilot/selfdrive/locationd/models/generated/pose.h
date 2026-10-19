#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_3502113927026605761);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4448786092888764286);
void pose_H_mod_fun(double *state, double *out_3056675913014331784);
void pose_f_fun(double *state, double dt, double *out_1826304795513048853);
void pose_F_fun(double *state, double dt, double *out_2679369155394544549);
void pose_h_4(double *state, double *unused, double *out_4714243789752105753);
void pose_H_4(double *state, double *unused, double *out_5444406278175342083);
void pose_h_10(double *state, double *unused, double *out_6341543449718086925);
void pose_H_10(double *state, double *unused, double *out_7615405668400164669);
void pose_h_13(double *state, double *unused, double *out_4272667523865022176);
void pose_H_13(double *state, double *unused, double *out_9168582332231685509);
void pose_h_14(double *state, double *unused, double *out_4516626462834453539);
void pose_H_14(double *state, double *unused, double *out_8527194710470714379);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}