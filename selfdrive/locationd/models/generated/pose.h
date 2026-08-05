#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2659138583199627985);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9150491803641430703);
void pose_H_mod_fun(double *state, double *out_3626402567552656885);
void pose_f_fun(double *state, double dt, double *out_1920499082245897032);
void pose_F_fun(double *state, double dt, double *out_712434411548898420);
void pose_h_4(double *state, double *unused, double *out_3116653058806651788);
void pose_H_4(double *state, double *unused, double *out_988224469376659479);
void pose_h_10(double *state, double *unused, double *out_7618912612446005780);
void pose_H_10(double *state, double *unused, double *out_6151723592301710994);
void pose_h_13(double *state, double *unused, double *out_3058136861535857965);
void pose_H_13(double *state, double *unused, double *out_4200498294708992280);
void pose_h_14(double *state, double *unused, double *out_4264661486828733996);
void pose_H_14(double *state, double *unused, double *out_4951465325716144008);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}