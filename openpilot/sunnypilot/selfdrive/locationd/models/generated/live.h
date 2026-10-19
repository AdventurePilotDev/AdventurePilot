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
void live_H(double *in_vec, double *out_8241596145839982091);
void live_err_fun(double *nom_x, double *delta_x, double *out_5410225479877336093);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_3835141630993920447);
void live_H_mod_fun(double *state, double *out_4298098101826018956);
void live_f_fun(double *state, double dt, double *out_3528623282705283150);
void live_F_fun(double *state, double dt, double *out_2197031599451942766);
void live_h_4(double *state, double *unused, double *out_4015823116481760703);
void live_H_4(double *state, double *unused, double *out_5312008688099291876);
void live_h_9(double *state, double *unused, double *out_4704618901501087646);
void live_H_9(double *state, double *unused, double *out_5847516450345812270);
void live_h_10(double *state, double *unused, double *out_3252932325432063749);
void live_H_10(double *state, double *unused, double *out_5617933756059262287);
void live_h_12(double *state, double *unused, double *out_5867697527364270010);
void live_H_12(double *state, double *unused, double *out_1069249688943441120);
void live_h_35(double *state, double *unused, double *out_7276552309046433565);
void live_H_35(double *state, double *unused, double *out_2722044039602795539);
void live_h_32(double *state, double *unused, double *out_6583895959340944069);
void live_H_32(double *state, double *unused, double *out_8809835530412639721);
void live_h_13(double *state, double *unused, double *out_67266343396907495);
void live_H_13(double *state, double *unused, double *out_7749796733159041478);
void live_h_14(double *state, double *unused, double *out_4704618901501087646);
void live_H_14(double *state, double *unused, double *out_5847516450345812270);
void live_h_33(double *state, double *unused, double *out_5411125201876215915);
void live_H_33(double *state, double *unused, double *out_428512965036062065);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}