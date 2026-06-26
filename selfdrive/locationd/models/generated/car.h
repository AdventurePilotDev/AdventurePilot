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
void car_err_fun(double *nom_x, double *delta_x, double *out_6297902908854341065);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2377905572268182998);
void car_H_mod_fun(double *state, double *out_2002636523246204337);
void car_f_fun(double *state, double dt, double *out_4249707811255504312);
void car_F_fun(double *state, double dt, double *out_2194004734568915691);
void car_h_25(double *state, double *unused, double *out_4303200061467699148);
void car_H_25(double *state, double *unused, double *out_4677059596590100574);
void car_h_24(double *state, double *unused, double *out_3831534366280123034);
void car_H_24(double *state, double *unused, double *out_2504409997584601008);
void car_h_30(double *state, double *unused, double *out_542041218645691241);
void car_H_30(double *state, double *unused, double *out_7195392555097349201);
void car_h_26(double *state, double *unused, double *out_1524125311980989353);
void car_H_26(double *state, double *unused, double *out_935556277716044350);
void car_h_27(double *state, double *unused, double *out_8701152142749328160);
void car_H_27(double *state, double *unused, double *out_9027757447428259198);
void car_h_29(double *state, double *unused, double *out_5504421216326984473);
void car_H_29(double *state, double *unused, double *out_7705623899411741385);
void car_h_28(double *state, double *unused, double *out_424033824712314497);
void car_H_28(double *state, double *unused, double *out_2623224882342210811);
void car_h_31(double *state, double *unused, double *out_3578072589286350037);
void car_H_31(double *state, double *unused, double *out_309348175482692874);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}