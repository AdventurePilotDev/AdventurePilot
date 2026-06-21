#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_4876828158992611850);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4991901881103411206);
void pose_H_mod_fun(double *state, double *out_8109048779199487053);
void pose_f_fun(double *state, double dt, double *out_4885069504534895976);
void pose_F_fun(double *state, double dt, double *out_283482483053796941);
void pose_h_4(double *state, double *unused, double *out_113153149952726241);
void pose_H_4(double *state, double *unused, double *out_9139747812627166459);
void pose_h_10(double *state, double *unused, double *out_8729829078688731131);
void pose_H_10(double *state, double *unused, double *out_1254535187907276211);
void pose_h_13(double *state, double *unused, double *out_9102617023895648854);
void pose_H_13(double *state, double *unused, double *out_5473240797779861133);
void pose_h_14(double *state, double *unused, double *out_5520898165787292420);
void pose_H_14(double *state, double *unused, double *out_6224207828787012861);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}