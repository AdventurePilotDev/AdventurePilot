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
void live_H(double *in_vec, double *out_7344903313308335368);
void live_err_fun(double *nom_x, double *delta_x, double *out_483535319970010394);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2312348505638815752);
void live_H_mod_fun(double *state, double *out_369260046103698050);
void live_f_fun(double *state, double dt, double *out_5620265792811989448);
void live_F_fun(double *state, double dt, double *out_1362815459814933636);
void live_h_4(double *state, double *unused, double *out_2465419025313171764);
void live_H_4(double *state, double *unused, double *out_1106606712560341340);
void live_h_9(double *state, double *unused, double *out_7845372137913552359);
void live_H_9(double *state, double *unused, double *out_1347796359189931985);
void live_h_10(double *state, double *unused, double *out_5454824912221518491);
void live_H_10(double *state, double *unused, double *out_8455219895512735044);
void live_h_12(double *state, double *unused, double *out_2989188601855968574);
void live_H_12(double *state, double *unused, double *out_6126063120592303135);
void live_h_35(double *state, double *unused, double *out_8880685408376834569);
void live_H_35(double *state, double *unused, double *out_4473268769932948716);
void live_h_32(double *state, double *unused, double *out_4786026413549388349);
void live_H_32(double *state, double *unused, double *out_6414107107265957517);
void live_h_13(double *state, double *unused, double *out_2147946318169503518);
void live_H_13(double *state, double *unused, double *out_1859147905438193815);
void live_h_14(double *state, double *unused, double *out_7845372137913552359);
void live_H_14(double *state, double *unused, double *out_1347796359189931985);
void live_h_33(double *state, double *unused, double *out_4654283979618042763);
void live_H_33(double *state, double *unused, double *out_7623825774571806320);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}