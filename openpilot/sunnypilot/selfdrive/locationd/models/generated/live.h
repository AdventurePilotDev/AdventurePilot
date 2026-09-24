#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_3854801932036381166);
void live_err_fun(double *nom_x, double *delta_x, double *out_283869237983750097);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_3934323313185254038);
void live_H_mod_fun(double *state, double *out_3538257475003660008);
void live_f_fun(double *state, double dt, double *out_8304654728393606511);
void live_F_fun(double *state, double dt, double *out_7874498760090357835);
void live_h_4(double *state, double *unused, double *out_4443356793922414925);
void live_H_4(double *state, double *unused, double *out_1891287416943340456);
void live_h_9(double *state, double *unused, double *out_2496714802239098011);
void live_H_9(double *state, double *unused, double *out_9178506352207787926);
void live_h_10(double *state, double *unused, double *out_2122765885074738711);
void live_H_10(double *state, double *unused, double *out_6854699472469941943);
void live_h_12(double *state, double *unused, double *out_603897377849376785);
void live_H_12(double *state, double *unused, double *out_4489970960099392540);
void live_h_35(double *state, double *unused, double *out_3453193634567693925);
void live_H_35(double *state, double *unused, double *out_5257949474315947832);
void live_h_32(double *state, double *unused, double *out_7281324984039621015);
void live_H_32(double *state, double *unused, double *out_341460938551134401);
void live_h_13(double *state, double *unused, double *out_4298561063206584720);
void live_H_13(double *state, double *unused, double *out_3121377717960824433);
void live_h_14(double *state, double *unused, double *out_2496714802239098011);
void live_H_14(double *state, double *unused, double *out_9178506352207787926);
void live_h_33(double *state, double *unused, double *out_8382409076017554228);
void live_H_33(double *state, double *unused, double *out_8408506478954805436);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}