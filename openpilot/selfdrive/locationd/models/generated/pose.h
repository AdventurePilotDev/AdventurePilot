#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_3982225953723843213);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6906535223873576474);
void pose_H_mod_fun(double *state, double *out_8356239334660173247);
void pose_f_fun(double *state, double dt, double *out_94988861296200854);
void pose_F_fun(double *state, double dt, double *out_9024536503228902294);
void pose_h_4(double *state, double *unused, double *out_5642475729519128019);
void pose_H_4(double *state, double *unused, double *out_1146143072991557027);
void pose_h_10(double *state, double *unused, double *out_8808426522592935865);
void pose_H_10(double *state, double *unused, double *out_1070655411699127393);
void pose_h_13(double *state, double *unused, double *out_551055256458384175);
void pose_H_13(double *state, double *unused, double *out_4358416898323889828);
void pose_h_14(double *state, double *unused, double *out_762241609475764552);
void pose_H_14(double *state, double *unused, double *out_5109383929331041556);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}