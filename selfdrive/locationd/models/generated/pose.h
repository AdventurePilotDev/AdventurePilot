#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_4955557050137928755);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3781966990875519323);
void pose_H_mod_fun(double *state, double *out_4552347542280292738);
void pose_f_fun(double *state, double dt, double *out_2861912360858063342);
void pose_F_fun(double *state, double dt, double *out_8378829832132664204);
void pose_h_4(double *state, double *unused, double *out_7327194707407096308);
void pose_H_4(double *state, double *unused, double *out_4185848689802696400);
void pose_h_10(double *state, double *unused, double *out_8976729366753643307);
void pose_H_10(double *state, double *unused, double *out_1279619994472514045);
void pose_h_13(double *state, double *unused, double *out_5013469042968699666);
void pose_H_13(double *state, double *unused, double *out_7398122515135029201);
void pose_h_14(double *state, double *unused, double *out_2265009778651912904);
void pose_H_14(double *state, double *unused, double *out_8149089546142180929);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}