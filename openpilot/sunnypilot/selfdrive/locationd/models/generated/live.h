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
void live_H(double *in_vec, double *out_2250326117719485916);
void live_err_fun(double *nom_x, double *delta_x, double *out_120841737626675689);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_6893340610047731788);
void live_H_mod_fun(double *state, double *out_1511973280889723390);
void live_f_fun(double *state, double dt, double *out_8842419435108912409);
void live_F_fun(double *state, double dt, double *out_3957531060428025279);
void live_h_4(double *state, double *unused, double *out_5124515923286197174);
void live_H_4(double *state, double *unused, double *out_6976269510782622331);
void live_h_9(double *state, double *unused, double *out_5239900982090725494);
void live_H_9(double *state, double *unused, double *out_6735079864153031686);
void live_h_10(double *state, double *unused, double *out_5015347706321607129);
void live_H_10(double *state, double *unused, double *out_4213575601558554971);
void live_h_12(double *state, double *unused, double *out_2205260904833824546);
void live_H_12(double *state, double *unused, double *out_1956813102750660536);
void live_h_35(double *state, double *unused, double *out_4382889436063845136);
void live_H_35(double *state, double *unused, double *out_3609607453410014955);
void live_h_32(double *state, double *unused, double *out_7517443676256919481);
void live_H_32(double *state, double *unused, double *out_476128234686908145);
void live_h_13(double *state, double *unused, double *out_257591906353689808);
void live_H_13(double *state, double *unused, double *out_5409896471783919374);
void live_h_14(double *state, double *unused, double *out_5239900982090725494);
void live_H_14(double *state, double *unused, double *out_6735079864153031686);
void live_h_33(double *state, double *unused, double *out_6827451045648211629);
void live_H_33(double *state, double *unused, double *out_459050448771157351);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}