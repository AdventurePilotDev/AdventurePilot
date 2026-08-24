#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_5594348787172829747);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_7955832377025227799);
void pose_H_mod_fun(double *state, double *out_738947463718714402);
void pose_f_fun(double *state, double dt, double *out_7345501600501038484);
void pose_F_fun(double *state, double dt, double *out_6198689461587572803);
void pose_h_4(double *state, double *unused, double *out_4572986314476450084);
void pose_H_4(double *state, double *unused, double *out_1791217832987638276);
void pose_h_10(double *state, double *unused, double *out_6841094257975008708);
void pose_H_10(double *state, double *unused, double *out_5018362184474345094);
void pose_h_13(double *state, double *unused, double *out_5707469078074511089);
void pose_H_13(double *state, double *unused, double *out_9044895032405212411);
void pose_h_14(double *state, double *unused, double *out_7196792041580805526);
void pose_H_14(double *state, double *unused, double *out_5754458689327122805);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}