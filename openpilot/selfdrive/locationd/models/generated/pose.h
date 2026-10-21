#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8460591430937244758);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2176108056044053025);
void pose_H_mod_fun(double *state, double *out_7992819708489120391);
void pose_f_fun(double *state, double dt, double *out_4870646846854820424);
void pose_F_fun(double *state, double dt, double *out_8049488768110552288);
void pose_h_4(double *state, double *unused, double *out_4098665418884332867);
void pose_H_4(double *state, double *unused, double *out_951541957568700951);
void pose_h_10(double *state, double *unused, double *out_7546292780274681562);
void pose_H_10(double *state, double *unused, double *out_5588343581066124789);
void pose_h_13(double *state, double *unused, double *out_6823517049288757610);
void pose_H_13(double *state, double *unused, double *out_2260731867763631850);
void pose_h_14(double *state, double *unused, double *out_4599078664097125310);
void pose_H_14(double *state, double *unused, double *out_1386658484213584550);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}