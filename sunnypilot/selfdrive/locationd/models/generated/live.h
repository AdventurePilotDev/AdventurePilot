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
void live_H(double *in_vec, double *out_148756306077141358);
void live_err_fun(double *nom_x, double *delta_x, double *out_2694765942971368716);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2503703578000368343);
void live_H_mod_fun(double *state, double *out_4411336113252524624);
void live_f_fun(double *state, double dt, double *out_2317685740812028446);
void live_F_fun(double *state, double dt, double *out_5891409664242203513);
void live_h_4(double *state, double *unused, double *out_7529653384922748197);
void live_H_4(double *state, double *unused, double *out_8313045078567038360);
void live_h_9(double *state, double *unused, double *out_3063271929447938741);
void live_H_9(double *state, double *unused, double *out_8071855431937447715);
void live_h_10(double *state, double *unused, double *out_7349615753171587534);
void live_H_10(double *state, double *unused, double *out_1886387235108659432);
void live_h_12(double *state, double *unused, double *out_2449105307522013031);
void live_H_12(double *state, double *unused, double *out_3293588670535076565);
void live_h_35(double *state, double *unused, double *out_9113711929196351085);
void live_H_35(double *state, double *unused, double *out_4946383021194430984);
void live_h_32(double *state, double *unused, double *out_1723851860166063319);
void live_H_32(double *state, double *unused, double *out_7832864444042885480);
void live_h_13(double *state, double *unused, double *out_6742435844861107703);
void live_H_13(double *state, double *unused, double *out_8343832325847279692);
void live_h_14(double *state, double *unused, double *out_3063271929447938741);
void live_H_14(double *state, double *unused, double *out_8071855431937447715);
void live_h_33(double *state, double *unused, double *out_6293104590192938576);
void live_H_33(double *state, double *unused, double *out_1795826016555573380);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}