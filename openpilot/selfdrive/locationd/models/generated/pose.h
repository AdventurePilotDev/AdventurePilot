#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_7182192695751697228);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_800007061037745391);
void pose_H_mod_fun(double *state, double *out_4496961729871771833);
void pose_f_fun(double *state, double dt, double *out_7612049814342810855);
void pose_F_fun(double *state, double dt, double *out_4578642212278889444);
void pose_h_4(double *state, double *unused, double *out_3436790236773195671);
void pose_H_4(double *state, double *unused, double *out_5251122731926484440);
void pose_h_10(double *state, double *unused, double *out_7778470094055120042);
void pose_H_10(double *state, double *unused, double *out_4797712383092980417);
void pose_h_13(double *state, double *unused, double *out_6994430400069285445);
void pose_H_13(double *state, double *unused, double *out_5584990133466366247);
void pose_h_14(double *state, double *unused, double *out_3179950759880466578);
void pose_H_14(double *state, double *unused, double *out_9214363588265968969);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}