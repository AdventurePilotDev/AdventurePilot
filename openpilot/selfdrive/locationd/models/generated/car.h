#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_7399913957597549850);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2224482965536493774);
void car_H_mod_fun(double *state, double *out_5001309090524474604);
void car_f_fun(double *state, double dt, double *out_3375412619810637560);
void car_F_fun(double *state, double dt, double *out_5556820103353452989);
void car_h_25(double *state, double *unused, double *out_171733956696665727);
void car_H_25(double *state, double *unused, double *out_4126000346185471624);
void car_h_24(double *state, double *unused, double *out_490024875657836088);
void car_H_24(double *state, double *unused, double *out_4966432130367516616);
void car_h_30(double *state, double *unused, double *out_7632292720945554052);
void car_H_30(double *state, double *unused, double *out_1607667387678222997);
void car_h_26(double *state, double *unused, double *out_5812778085103151919);
void car_H_26(double *state, double *unused, double *out_7867503665059527848);
void car_h_27(double *state, double *unused, double *out_4829939310964179918);
void car_H_27(double *state, double *unused, double *out_3782430699478647908);
void car_h_29(double *state, double *unused, double *out_7777665301319664879);
void car_H_29(double *state, double *unused, double *out_1097436043363830813);
void car_h_28(double *state, double *unused, double *out_3886429458062517483);
void car_H_28(double *state, double *unused, double *out_6179835060433361387);
void car_h_31(double *state, double *unused, double *out_14547288345536582);
void car_H_31(double *state, double *unused, double *out_8493711767292879324);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}