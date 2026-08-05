#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2659138583199627985) {
   out_2659138583199627985[0] = delta_x[0] + nom_x[0];
   out_2659138583199627985[1] = delta_x[1] + nom_x[1];
   out_2659138583199627985[2] = delta_x[2] + nom_x[2];
   out_2659138583199627985[3] = delta_x[3] + nom_x[3];
   out_2659138583199627985[4] = delta_x[4] + nom_x[4];
   out_2659138583199627985[5] = delta_x[5] + nom_x[5];
   out_2659138583199627985[6] = delta_x[6] + nom_x[6];
   out_2659138583199627985[7] = delta_x[7] + nom_x[7];
   out_2659138583199627985[8] = delta_x[8] + nom_x[8];
   out_2659138583199627985[9] = delta_x[9] + nom_x[9];
   out_2659138583199627985[10] = delta_x[10] + nom_x[10];
   out_2659138583199627985[11] = delta_x[11] + nom_x[11];
   out_2659138583199627985[12] = delta_x[12] + nom_x[12];
   out_2659138583199627985[13] = delta_x[13] + nom_x[13];
   out_2659138583199627985[14] = delta_x[14] + nom_x[14];
   out_2659138583199627985[15] = delta_x[15] + nom_x[15];
   out_2659138583199627985[16] = delta_x[16] + nom_x[16];
   out_2659138583199627985[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_9150491803641430703) {
   out_9150491803641430703[0] = -nom_x[0] + true_x[0];
   out_9150491803641430703[1] = -nom_x[1] + true_x[1];
   out_9150491803641430703[2] = -nom_x[2] + true_x[2];
   out_9150491803641430703[3] = -nom_x[3] + true_x[3];
   out_9150491803641430703[4] = -nom_x[4] + true_x[4];
   out_9150491803641430703[5] = -nom_x[5] + true_x[5];
   out_9150491803641430703[6] = -nom_x[6] + true_x[6];
   out_9150491803641430703[7] = -nom_x[7] + true_x[7];
   out_9150491803641430703[8] = -nom_x[8] + true_x[8];
   out_9150491803641430703[9] = -nom_x[9] + true_x[9];
   out_9150491803641430703[10] = -nom_x[10] + true_x[10];
   out_9150491803641430703[11] = -nom_x[11] + true_x[11];
   out_9150491803641430703[12] = -nom_x[12] + true_x[12];
   out_9150491803641430703[13] = -nom_x[13] + true_x[13];
   out_9150491803641430703[14] = -nom_x[14] + true_x[14];
   out_9150491803641430703[15] = -nom_x[15] + true_x[15];
   out_9150491803641430703[16] = -nom_x[16] + true_x[16];
   out_9150491803641430703[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_3626402567552656885) {
   out_3626402567552656885[0] = 1.0;
   out_3626402567552656885[1] = 0.0;
   out_3626402567552656885[2] = 0.0;
   out_3626402567552656885[3] = 0.0;
   out_3626402567552656885[4] = 0.0;
   out_3626402567552656885[5] = 0.0;
   out_3626402567552656885[6] = 0.0;
   out_3626402567552656885[7] = 0.0;
   out_3626402567552656885[8] = 0.0;
   out_3626402567552656885[9] = 0.0;
   out_3626402567552656885[10] = 0.0;
   out_3626402567552656885[11] = 0.0;
   out_3626402567552656885[12] = 0.0;
   out_3626402567552656885[13] = 0.0;
   out_3626402567552656885[14] = 0.0;
   out_3626402567552656885[15] = 0.0;
   out_3626402567552656885[16] = 0.0;
   out_3626402567552656885[17] = 0.0;
   out_3626402567552656885[18] = 0.0;
   out_3626402567552656885[19] = 1.0;
   out_3626402567552656885[20] = 0.0;
   out_3626402567552656885[21] = 0.0;
   out_3626402567552656885[22] = 0.0;
   out_3626402567552656885[23] = 0.0;
   out_3626402567552656885[24] = 0.0;
   out_3626402567552656885[25] = 0.0;
   out_3626402567552656885[26] = 0.0;
   out_3626402567552656885[27] = 0.0;
   out_3626402567552656885[28] = 0.0;
   out_3626402567552656885[29] = 0.0;
   out_3626402567552656885[30] = 0.0;
   out_3626402567552656885[31] = 0.0;
   out_3626402567552656885[32] = 0.0;
   out_3626402567552656885[33] = 0.0;
   out_3626402567552656885[34] = 0.0;
   out_3626402567552656885[35] = 0.0;
   out_3626402567552656885[36] = 0.0;
   out_3626402567552656885[37] = 0.0;
   out_3626402567552656885[38] = 1.0;
   out_3626402567552656885[39] = 0.0;
   out_3626402567552656885[40] = 0.0;
   out_3626402567552656885[41] = 0.0;
   out_3626402567552656885[42] = 0.0;
   out_3626402567552656885[43] = 0.0;
   out_3626402567552656885[44] = 0.0;
   out_3626402567552656885[45] = 0.0;
   out_3626402567552656885[46] = 0.0;
   out_3626402567552656885[47] = 0.0;
   out_3626402567552656885[48] = 0.0;
   out_3626402567552656885[49] = 0.0;
   out_3626402567552656885[50] = 0.0;
   out_3626402567552656885[51] = 0.0;
   out_3626402567552656885[52] = 0.0;
   out_3626402567552656885[53] = 0.0;
   out_3626402567552656885[54] = 0.0;
   out_3626402567552656885[55] = 0.0;
   out_3626402567552656885[56] = 0.0;
   out_3626402567552656885[57] = 1.0;
   out_3626402567552656885[58] = 0.0;
   out_3626402567552656885[59] = 0.0;
   out_3626402567552656885[60] = 0.0;
   out_3626402567552656885[61] = 0.0;
   out_3626402567552656885[62] = 0.0;
   out_3626402567552656885[63] = 0.0;
   out_3626402567552656885[64] = 0.0;
   out_3626402567552656885[65] = 0.0;
   out_3626402567552656885[66] = 0.0;
   out_3626402567552656885[67] = 0.0;
   out_3626402567552656885[68] = 0.0;
   out_3626402567552656885[69] = 0.0;
   out_3626402567552656885[70] = 0.0;
   out_3626402567552656885[71] = 0.0;
   out_3626402567552656885[72] = 0.0;
   out_3626402567552656885[73] = 0.0;
   out_3626402567552656885[74] = 0.0;
   out_3626402567552656885[75] = 0.0;
   out_3626402567552656885[76] = 1.0;
   out_3626402567552656885[77] = 0.0;
   out_3626402567552656885[78] = 0.0;
   out_3626402567552656885[79] = 0.0;
   out_3626402567552656885[80] = 0.0;
   out_3626402567552656885[81] = 0.0;
   out_3626402567552656885[82] = 0.0;
   out_3626402567552656885[83] = 0.0;
   out_3626402567552656885[84] = 0.0;
   out_3626402567552656885[85] = 0.0;
   out_3626402567552656885[86] = 0.0;
   out_3626402567552656885[87] = 0.0;
   out_3626402567552656885[88] = 0.0;
   out_3626402567552656885[89] = 0.0;
   out_3626402567552656885[90] = 0.0;
   out_3626402567552656885[91] = 0.0;
   out_3626402567552656885[92] = 0.0;
   out_3626402567552656885[93] = 0.0;
   out_3626402567552656885[94] = 0.0;
   out_3626402567552656885[95] = 1.0;
   out_3626402567552656885[96] = 0.0;
   out_3626402567552656885[97] = 0.0;
   out_3626402567552656885[98] = 0.0;
   out_3626402567552656885[99] = 0.0;
   out_3626402567552656885[100] = 0.0;
   out_3626402567552656885[101] = 0.0;
   out_3626402567552656885[102] = 0.0;
   out_3626402567552656885[103] = 0.0;
   out_3626402567552656885[104] = 0.0;
   out_3626402567552656885[105] = 0.0;
   out_3626402567552656885[106] = 0.0;
   out_3626402567552656885[107] = 0.0;
   out_3626402567552656885[108] = 0.0;
   out_3626402567552656885[109] = 0.0;
   out_3626402567552656885[110] = 0.0;
   out_3626402567552656885[111] = 0.0;
   out_3626402567552656885[112] = 0.0;
   out_3626402567552656885[113] = 0.0;
   out_3626402567552656885[114] = 1.0;
   out_3626402567552656885[115] = 0.0;
   out_3626402567552656885[116] = 0.0;
   out_3626402567552656885[117] = 0.0;
   out_3626402567552656885[118] = 0.0;
   out_3626402567552656885[119] = 0.0;
   out_3626402567552656885[120] = 0.0;
   out_3626402567552656885[121] = 0.0;
   out_3626402567552656885[122] = 0.0;
   out_3626402567552656885[123] = 0.0;
   out_3626402567552656885[124] = 0.0;
   out_3626402567552656885[125] = 0.0;
   out_3626402567552656885[126] = 0.0;
   out_3626402567552656885[127] = 0.0;
   out_3626402567552656885[128] = 0.0;
   out_3626402567552656885[129] = 0.0;
   out_3626402567552656885[130] = 0.0;
   out_3626402567552656885[131] = 0.0;
   out_3626402567552656885[132] = 0.0;
   out_3626402567552656885[133] = 1.0;
   out_3626402567552656885[134] = 0.0;
   out_3626402567552656885[135] = 0.0;
   out_3626402567552656885[136] = 0.0;
   out_3626402567552656885[137] = 0.0;
   out_3626402567552656885[138] = 0.0;
   out_3626402567552656885[139] = 0.0;
   out_3626402567552656885[140] = 0.0;
   out_3626402567552656885[141] = 0.0;
   out_3626402567552656885[142] = 0.0;
   out_3626402567552656885[143] = 0.0;
   out_3626402567552656885[144] = 0.0;
   out_3626402567552656885[145] = 0.0;
   out_3626402567552656885[146] = 0.0;
   out_3626402567552656885[147] = 0.0;
   out_3626402567552656885[148] = 0.0;
   out_3626402567552656885[149] = 0.0;
   out_3626402567552656885[150] = 0.0;
   out_3626402567552656885[151] = 0.0;
   out_3626402567552656885[152] = 1.0;
   out_3626402567552656885[153] = 0.0;
   out_3626402567552656885[154] = 0.0;
   out_3626402567552656885[155] = 0.0;
   out_3626402567552656885[156] = 0.0;
   out_3626402567552656885[157] = 0.0;
   out_3626402567552656885[158] = 0.0;
   out_3626402567552656885[159] = 0.0;
   out_3626402567552656885[160] = 0.0;
   out_3626402567552656885[161] = 0.0;
   out_3626402567552656885[162] = 0.0;
   out_3626402567552656885[163] = 0.0;
   out_3626402567552656885[164] = 0.0;
   out_3626402567552656885[165] = 0.0;
   out_3626402567552656885[166] = 0.0;
   out_3626402567552656885[167] = 0.0;
   out_3626402567552656885[168] = 0.0;
   out_3626402567552656885[169] = 0.0;
   out_3626402567552656885[170] = 0.0;
   out_3626402567552656885[171] = 1.0;
   out_3626402567552656885[172] = 0.0;
   out_3626402567552656885[173] = 0.0;
   out_3626402567552656885[174] = 0.0;
   out_3626402567552656885[175] = 0.0;
   out_3626402567552656885[176] = 0.0;
   out_3626402567552656885[177] = 0.0;
   out_3626402567552656885[178] = 0.0;
   out_3626402567552656885[179] = 0.0;
   out_3626402567552656885[180] = 0.0;
   out_3626402567552656885[181] = 0.0;
   out_3626402567552656885[182] = 0.0;
   out_3626402567552656885[183] = 0.0;
   out_3626402567552656885[184] = 0.0;
   out_3626402567552656885[185] = 0.0;
   out_3626402567552656885[186] = 0.0;
   out_3626402567552656885[187] = 0.0;
   out_3626402567552656885[188] = 0.0;
   out_3626402567552656885[189] = 0.0;
   out_3626402567552656885[190] = 1.0;
   out_3626402567552656885[191] = 0.0;
   out_3626402567552656885[192] = 0.0;
   out_3626402567552656885[193] = 0.0;
   out_3626402567552656885[194] = 0.0;
   out_3626402567552656885[195] = 0.0;
   out_3626402567552656885[196] = 0.0;
   out_3626402567552656885[197] = 0.0;
   out_3626402567552656885[198] = 0.0;
   out_3626402567552656885[199] = 0.0;
   out_3626402567552656885[200] = 0.0;
   out_3626402567552656885[201] = 0.0;
   out_3626402567552656885[202] = 0.0;
   out_3626402567552656885[203] = 0.0;
   out_3626402567552656885[204] = 0.0;
   out_3626402567552656885[205] = 0.0;
   out_3626402567552656885[206] = 0.0;
   out_3626402567552656885[207] = 0.0;
   out_3626402567552656885[208] = 0.0;
   out_3626402567552656885[209] = 1.0;
   out_3626402567552656885[210] = 0.0;
   out_3626402567552656885[211] = 0.0;
   out_3626402567552656885[212] = 0.0;
   out_3626402567552656885[213] = 0.0;
   out_3626402567552656885[214] = 0.0;
   out_3626402567552656885[215] = 0.0;
   out_3626402567552656885[216] = 0.0;
   out_3626402567552656885[217] = 0.0;
   out_3626402567552656885[218] = 0.0;
   out_3626402567552656885[219] = 0.0;
   out_3626402567552656885[220] = 0.0;
   out_3626402567552656885[221] = 0.0;
   out_3626402567552656885[222] = 0.0;
   out_3626402567552656885[223] = 0.0;
   out_3626402567552656885[224] = 0.0;
   out_3626402567552656885[225] = 0.0;
   out_3626402567552656885[226] = 0.0;
   out_3626402567552656885[227] = 0.0;
   out_3626402567552656885[228] = 1.0;
   out_3626402567552656885[229] = 0.0;
   out_3626402567552656885[230] = 0.0;
   out_3626402567552656885[231] = 0.0;
   out_3626402567552656885[232] = 0.0;
   out_3626402567552656885[233] = 0.0;
   out_3626402567552656885[234] = 0.0;
   out_3626402567552656885[235] = 0.0;
   out_3626402567552656885[236] = 0.0;
   out_3626402567552656885[237] = 0.0;
   out_3626402567552656885[238] = 0.0;
   out_3626402567552656885[239] = 0.0;
   out_3626402567552656885[240] = 0.0;
   out_3626402567552656885[241] = 0.0;
   out_3626402567552656885[242] = 0.0;
   out_3626402567552656885[243] = 0.0;
   out_3626402567552656885[244] = 0.0;
   out_3626402567552656885[245] = 0.0;
   out_3626402567552656885[246] = 0.0;
   out_3626402567552656885[247] = 1.0;
   out_3626402567552656885[248] = 0.0;
   out_3626402567552656885[249] = 0.0;
   out_3626402567552656885[250] = 0.0;
   out_3626402567552656885[251] = 0.0;
   out_3626402567552656885[252] = 0.0;
   out_3626402567552656885[253] = 0.0;
   out_3626402567552656885[254] = 0.0;
   out_3626402567552656885[255] = 0.0;
   out_3626402567552656885[256] = 0.0;
   out_3626402567552656885[257] = 0.0;
   out_3626402567552656885[258] = 0.0;
   out_3626402567552656885[259] = 0.0;
   out_3626402567552656885[260] = 0.0;
   out_3626402567552656885[261] = 0.0;
   out_3626402567552656885[262] = 0.0;
   out_3626402567552656885[263] = 0.0;
   out_3626402567552656885[264] = 0.0;
   out_3626402567552656885[265] = 0.0;
   out_3626402567552656885[266] = 1.0;
   out_3626402567552656885[267] = 0.0;
   out_3626402567552656885[268] = 0.0;
   out_3626402567552656885[269] = 0.0;
   out_3626402567552656885[270] = 0.0;
   out_3626402567552656885[271] = 0.0;
   out_3626402567552656885[272] = 0.0;
   out_3626402567552656885[273] = 0.0;
   out_3626402567552656885[274] = 0.0;
   out_3626402567552656885[275] = 0.0;
   out_3626402567552656885[276] = 0.0;
   out_3626402567552656885[277] = 0.0;
   out_3626402567552656885[278] = 0.0;
   out_3626402567552656885[279] = 0.0;
   out_3626402567552656885[280] = 0.0;
   out_3626402567552656885[281] = 0.0;
   out_3626402567552656885[282] = 0.0;
   out_3626402567552656885[283] = 0.0;
   out_3626402567552656885[284] = 0.0;
   out_3626402567552656885[285] = 1.0;
   out_3626402567552656885[286] = 0.0;
   out_3626402567552656885[287] = 0.0;
   out_3626402567552656885[288] = 0.0;
   out_3626402567552656885[289] = 0.0;
   out_3626402567552656885[290] = 0.0;
   out_3626402567552656885[291] = 0.0;
   out_3626402567552656885[292] = 0.0;
   out_3626402567552656885[293] = 0.0;
   out_3626402567552656885[294] = 0.0;
   out_3626402567552656885[295] = 0.0;
   out_3626402567552656885[296] = 0.0;
   out_3626402567552656885[297] = 0.0;
   out_3626402567552656885[298] = 0.0;
   out_3626402567552656885[299] = 0.0;
   out_3626402567552656885[300] = 0.0;
   out_3626402567552656885[301] = 0.0;
   out_3626402567552656885[302] = 0.0;
   out_3626402567552656885[303] = 0.0;
   out_3626402567552656885[304] = 1.0;
   out_3626402567552656885[305] = 0.0;
   out_3626402567552656885[306] = 0.0;
   out_3626402567552656885[307] = 0.0;
   out_3626402567552656885[308] = 0.0;
   out_3626402567552656885[309] = 0.0;
   out_3626402567552656885[310] = 0.0;
   out_3626402567552656885[311] = 0.0;
   out_3626402567552656885[312] = 0.0;
   out_3626402567552656885[313] = 0.0;
   out_3626402567552656885[314] = 0.0;
   out_3626402567552656885[315] = 0.0;
   out_3626402567552656885[316] = 0.0;
   out_3626402567552656885[317] = 0.0;
   out_3626402567552656885[318] = 0.0;
   out_3626402567552656885[319] = 0.0;
   out_3626402567552656885[320] = 0.0;
   out_3626402567552656885[321] = 0.0;
   out_3626402567552656885[322] = 0.0;
   out_3626402567552656885[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_1920499082245897032) {
   out_1920499082245897032[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_1920499082245897032[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_1920499082245897032[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_1920499082245897032[3] = dt*state[12] + state[3];
   out_1920499082245897032[4] = dt*state[13] + state[4];
   out_1920499082245897032[5] = dt*state[14] + state[5];
   out_1920499082245897032[6] = state[6];
   out_1920499082245897032[7] = state[7];
   out_1920499082245897032[8] = state[8];
   out_1920499082245897032[9] = state[9];
   out_1920499082245897032[10] = state[10];
   out_1920499082245897032[11] = state[11];
   out_1920499082245897032[12] = state[12];
   out_1920499082245897032[13] = state[13];
   out_1920499082245897032[14] = state[14];
   out_1920499082245897032[15] = state[15];
   out_1920499082245897032[16] = state[16];
   out_1920499082245897032[17] = state[17];
}
void F_fun(double *state, double dt, double *out_712434411548898420) {
   out_712434411548898420[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_712434411548898420[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_712434411548898420[2] = 0;
   out_712434411548898420[3] = 0;
   out_712434411548898420[4] = 0;
   out_712434411548898420[5] = 0;
   out_712434411548898420[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_712434411548898420[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_712434411548898420[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_712434411548898420[9] = 0;
   out_712434411548898420[10] = 0;
   out_712434411548898420[11] = 0;
   out_712434411548898420[12] = 0;
   out_712434411548898420[13] = 0;
   out_712434411548898420[14] = 0;
   out_712434411548898420[15] = 0;
   out_712434411548898420[16] = 0;
   out_712434411548898420[17] = 0;
   out_712434411548898420[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_712434411548898420[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_712434411548898420[20] = 0;
   out_712434411548898420[21] = 0;
   out_712434411548898420[22] = 0;
   out_712434411548898420[23] = 0;
   out_712434411548898420[24] = 0;
   out_712434411548898420[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_712434411548898420[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_712434411548898420[27] = 0;
   out_712434411548898420[28] = 0;
   out_712434411548898420[29] = 0;
   out_712434411548898420[30] = 0;
   out_712434411548898420[31] = 0;
   out_712434411548898420[32] = 0;
   out_712434411548898420[33] = 0;
   out_712434411548898420[34] = 0;
   out_712434411548898420[35] = 0;
   out_712434411548898420[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_712434411548898420[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_712434411548898420[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_712434411548898420[39] = 0;
   out_712434411548898420[40] = 0;
   out_712434411548898420[41] = 0;
   out_712434411548898420[42] = 0;
   out_712434411548898420[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_712434411548898420[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_712434411548898420[45] = 0;
   out_712434411548898420[46] = 0;
   out_712434411548898420[47] = 0;
   out_712434411548898420[48] = 0;
   out_712434411548898420[49] = 0;
   out_712434411548898420[50] = 0;
   out_712434411548898420[51] = 0;
   out_712434411548898420[52] = 0;
   out_712434411548898420[53] = 0;
   out_712434411548898420[54] = 0;
   out_712434411548898420[55] = 0;
   out_712434411548898420[56] = 0;
   out_712434411548898420[57] = 1;
   out_712434411548898420[58] = 0;
   out_712434411548898420[59] = 0;
   out_712434411548898420[60] = 0;
   out_712434411548898420[61] = 0;
   out_712434411548898420[62] = 0;
   out_712434411548898420[63] = 0;
   out_712434411548898420[64] = 0;
   out_712434411548898420[65] = 0;
   out_712434411548898420[66] = dt;
   out_712434411548898420[67] = 0;
   out_712434411548898420[68] = 0;
   out_712434411548898420[69] = 0;
   out_712434411548898420[70] = 0;
   out_712434411548898420[71] = 0;
   out_712434411548898420[72] = 0;
   out_712434411548898420[73] = 0;
   out_712434411548898420[74] = 0;
   out_712434411548898420[75] = 0;
   out_712434411548898420[76] = 1;
   out_712434411548898420[77] = 0;
   out_712434411548898420[78] = 0;
   out_712434411548898420[79] = 0;
   out_712434411548898420[80] = 0;
   out_712434411548898420[81] = 0;
   out_712434411548898420[82] = 0;
   out_712434411548898420[83] = 0;
   out_712434411548898420[84] = 0;
   out_712434411548898420[85] = dt;
   out_712434411548898420[86] = 0;
   out_712434411548898420[87] = 0;
   out_712434411548898420[88] = 0;
   out_712434411548898420[89] = 0;
   out_712434411548898420[90] = 0;
   out_712434411548898420[91] = 0;
   out_712434411548898420[92] = 0;
   out_712434411548898420[93] = 0;
   out_712434411548898420[94] = 0;
   out_712434411548898420[95] = 1;
   out_712434411548898420[96] = 0;
   out_712434411548898420[97] = 0;
   out_712434411548898420[98] = 0;
   out_712434411548898420[99] = 0;
   out_712434411548898420[100] = 0;
   out_712434411548898420[101] = 0;
   out_712434411548898420[102] = 0;
   out_712434411548898420[103] = 0;
   out_712434411548898420[104] = dt;
   out_712434411548898420[105] = 0;
   out_712434411548898420[106] = 0;
   out_712434411548898420[107] = 0;
   out_712434411548898420[108] = 0;
   out_712434411548898420[109] = 0;
   out_712434411548898420[110] = 0;
   out_712434411548898420[111] = 0;
   out_712434411548898420[112] = 0;
   out_712434411548898420[113] = 0;
   out_712434411548898420[114] = 1;
   out_712434411548898420[115] = 0;
   out_712434411548898420[116] = 0;
   out_712434411548898420[117] = 0;
   out_712434411548898420[118] = 0;
   out_712434411548898420[119] = 0;
   out_712434411548898420[120] = 0;
   out_712434411548898420[121] = 0;
   out_712434411548898420[122] = 0;
   out_712434411548898420[123] = 0;
   out_712434411548898420[124] = 0;
   out_712434411548898420[125] = 0;
   out_712434411548898420[126] = 0;
   out_712434411548898420[127] = 0;
   out_712434411548898420[128] = 0;
   out_712434411548898420[129] = 0;
   out_712434411548898420[130] = 0;
   out_712434411548898420[131] = 0;
   out_712434411548898420[132] = 0;
   out_712434411548898420[133] = 1;
   out_712434411548898420[134] = 0;
   out_712434411548898420[135] = 0;
   out_712434411548898420[136] = 0;
   out_712434411548898420[137] = 0;
   out_712434411548898420[138] = 0;
   out_712434411548898420[139] = 0;
   out_712434411548898420[140] = 0;
   out_712434411548898420[141] = 0;
   out_712434411548898420[142] = 0;
   out_712434411548898420[143] = 0;
   out_712434411548898420[144] = 0;
   out_712434411548898420[145] = 0;
   out_712434411548898420[146] = 0;
   out_712434411548898420[147] = 0;
   out_712434411548898420[148] = 0;
   out_712434411548898420[149] = 0;
   out_712434411548898420[150] = 0;
   out_712434411548898420[151] = 0;
   out_712434411548898420[152] = 1;
   out_712434411548898420[153] = 0;
   out_712434411548898420[154] = 0;
   out_712434411548898420[155] = 0;
   out_712434411548898420[156] = 0;
   out_712434411548898420[157] = 0;
   out_712434411548898420[158] = 0;
   out_712434411548898420[159] = 0;
   out_712434411548898420[160] = 0;
   out_712434411548898420[161] = 0;
   out_712434411548898420[162] = 0;
   out_712434411548898420[163] = 0;
   out_712434411548898420[164] = 0;
   out_712434411548898420[165] = 0;
   out_712434411548898420[166] = 0;
   out_712434411548898420[167] = 0;
   out_712434411548898420[168] = 0;
   out_712434411548898420[169] = 0;
   out_712434411548898420[170] = 0;
   out_712434411548898420[171] = 1;
   out_712434411548898420[172] = 0;
   out_712434411548898420[173] = 0;
   out_712434411548898420[174] = 0;
   out_712434411548898420[175] = 0;
   out_712434411548898420[176] = 0;
   out_712434411548898420[177] = 0;
   out_712434411548898420[178] = 0;
   out_712434411548898420[179] = 0;
   out_712434411548898420[180] = 0;
   out_712434411548898420[181] = 0;
   out_712434411548898420[182] = 0;
   out_712434411548898420[183] = 0;
   out_712434411548898420[184] = 0;
   out_712434411548898420[185] = 0;
   out_712434411548898420[186] = 0;
   out_712434411548898420[187] = 0;
   out_712434411548898420[188] = 0;
   out_712434411548898420[189] = 0;
   out_712434411548898420[190] = 1;
   out_712434411548898420[191] = 0;
   out_712434411548898420[192] = 0;
   out_712434411548898420[193] = 0;
   out_712434411548898420[194] = 0;
   out_712434411548898420[195] = 0;
   out_712434411548898420[196] = 0;
   out_712434411548898420[197] = 0;
   out_712434411548898420[198] = 0;
   out_712434411548898420[199] = 0;
   out_712434411548898420[200] = 0;
   out_712434411548898420[201] = 0;
   out_712434411548898420[202] = 0;
   out_712434411548898420[203] = 0;
   out_712434411548898420[204] = 0;
   out_712434411548898420[205] = 0;
   out_712434411548898420[206] = 0;
   out_712434411548898420[207] = 0;
   out_712434411548898420[208] = 0;
   out_712434411548898420[209] = 1;
   out_712434411548898420[210] = 0;
   out_712434411548898420[211] = 0;
   out_712434411548898420[212] = 0;
   out_712434411548898420[213] = 0;
   out_712434411548898420[214] = 0;
   out_712434411548898420[215] = 0;
   out_712434411548898420[216] = 0;
   out_712434411548898420[217] = 0;
   out_712434411548898420[218] = 0;
   out_712434411548898420[219] = 0;
   out_712434411548898420[220] = 0;
   out_712434411548898420[221] = 0;
   out_712434411548898420[222] = 0;
   out_712434411548898420[223] = 0;
   out_712434411548898420[224] = 0;
   out_712434411548898420[225] = 0;
   out_712434411548898420[226] = 0;
   out_712434411548898420[227] = 0;
   out_712434411548898420[228] = 1;
   out_712434411548898420[229] = 0;
   out_712434411548898420[230] = 0;
   out_712434411548898420[231] = 0;
   out_712434411548898420[232] = 0;
   out_712434411548898420[233] = 0;
   out_712434411548898420[234] = 0;
   out_712434411548898420[235] = 0;
   out_712434411548898420[236] = 0;
   out_712434411548898420[237] = 0;
   out_712434411548898420[238] = 0;
   out_712434411548898420[239] = 0;
   out_712434411548898420[240] = 0;
   out_712434411548898420[241] = 0;
   out_712434411548898420[242] = 0;
   out_712434411548898420[243] = 0;
   out_712434411548898420[244] = 0;
   out_712434411548898420[245] = 0;
   out_712434411548898420[246] = 0;
   out_712434411548898420[247] = 1;
   out_712434411548898420[248] = 0;
   out_712434411548898420[249] = 0;
   out_712434411548898420[250] = 0;
   out_712434411548898420[251] = 0;
   out_712434411548898420[252] = 0;
   out_712434411548898420[253] = 0;
   out_712434411548898420[254] = 0;
   out_712434411548898420[255] = 0;
   out_712434411548898420[256] = 0;
   out_712434411548898420[257] = 0;
   out_712434411548898420[258] = 0;
   out_712434411548898420[259] = 0;
   out_712434411548898420[260] = 0;
   out_712434411548898420[261] = 0;
   out_712434411548898420[262] = 0;
   out_712434411548898420[263] = 0;
   out_712434411548898420[264] = 0;
   out_712434411548898420[265] = 0;
   out_712434411548898420[266] = 1;
   out_712434411548898420[267] = 0;
   out_712434411548898420[268] = 0;
   out_712434411548898420[269] = 0;
   out_712434411548898420[270] = 0;
   out_712434411548898420[271] = 0;
   out_712434411548898420[272] = 0;
   out_712434411548898420[273] = 0;
   out_712434411548898420[274] = 0;
   out_712434411548898420[275] = 0;
   out_712434411548898420[276] = 0;
   out_712434411548898420[277] = 0;
   out_712434411548898420[278] = 0;
   out_712434411548898420[279] = 0;
   out_712434411548898420[280] = 0;
   out_712434411548898420[281] = 0;
   out_712434411548898420[282] = 0;
   out_712434411548898420[283] = 0;
   out_712434411548898420[284] = 0;
   out_712434411548898420[285] = 1;
   out_712434411548898420[286] = 0;
   out_712434411548898420[287] = 0;
   out_712434411548898420[288] = 0;
   out_712434411548898420[289] = 0;
   out_712434411548898420[290] = 0;
   out_712434411548898420[291] = 0;
   out_712434411548898420[292] = 0;
   out_712434411548898420[293] = 0;
   out_712434411548898420[294] = 0;
   out_712434411548898420[295] = 0;
   out_712434411548898420[296] = 0;
   out_712434411548898420[297] = 0;
   out_712434411548898420[298] = 0;
   out_712434411548898420[299] = 0;
   out_712434411548898420[300] = 0;
   out_712434411548898420[301] = 0;
   out_712434411548898420[302] = 0;
   out_712434411548898420[303] = 0;
   out_712434411548898420[304] = 1;
   out_712434411548898420[305] = 0;
   out_712434411548898420[306] = 0;
   out_712434411548898420[307] = 0;
   out_712434411548898420[308] = 0;
   out_712434411548898420[309] = 0;
   out_712434411548898420[310] = 0;
   out_712434411548898420[311] = 0;
   out_712434411548898420[312] = 0;
   out_712434411548898420[313] = 0;
   out_712434411548898420[314] = 0;
   out_712434411548898420[315] = 0;
   out_712434411548898420[316] = 0;
   out_712434411548898420[317] = 0;
   out_712434411548898420[318] = 0;
   out_712434411548898420[319] = 0;
   out_712434411548898420[320] = 0;
   out_712434411548898420[321] = 0;
   out_712434411548898420[322] = 0;
   out_712434411548898420[323] = 1;
}
void h_4(double *state, double *unused, double *out_3116653058806651788) {
   out_3116653058806651788[0] = state[6] + state[9];
   out_3116653058806651788[1] = state[7] + state[10];
   out_3116653058806651788[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_988224469376659479) {
   out_988224469376659479[0] = 0;
   out_988224469376659479[1] = 0;
   out_988224469376659479[2] = 0;
   out_988224469376659479[3] = 0;
   out_988224469376659479[4] = 0;
   out_988224469376659479[5] = 0;
   out_988224469376659479[6] = 1;
   out_988224469376659479[7] = 0;
   out_988224469376659479[8] = 0;
   out_988224469376659479[9] = 1;
   out_988224469376659479[10] = 0;
   out_988224469376659479[11] = 0;
   out_988224469376659479[12] = 0;
   out_988224469376659479[13] = 0;
   out_988224469376659479[14] = 0;
   out_988224469376659479[15] = 0;
   out_988224469376659479[16] = 0;
   out_988224469376659479[17] = 0;
   out_988224469376659479[18] = 0;
   out_988224469376659479[19] = 0;
   out_988224469376659479[20] = 0;
   out_988224469376659479[21] = 0;
   out_988224469376659479[22] = 0;
   out_988224469376659479[23] = 0;
   out_988224469376659479[24] = 0;
   out_988224469376659479[25] = 1;
   out_988224469376659479[26] = 0;
   out_988224469376659479[27] = 0;
   out_988224469376659479[28] = 1;
   out_988224469376659479[29] = 0;
   out_988224469376659479[30] = 0;
   out_988224469376659479[31] = 0;
   out_988224469376659479[32] = 0;
   out_988224469376659479[33] = 0;
   out_988224469376659479[34] = 0;
   out_988224469376659479[35] = 0;
   out_988224469376659479[36] = 0;
   out_988224469376659479[37] = 0;
   out_988224469376659479[38] = 0;
   out_988224469376659479[39] = 0;
   out_988224469376659479[40] = 0;
   out_988224469376659479[41] = 0;
   out_988224469376659479[42] = 0;
   out_988224469376659479[43] = 0;
   out_988224469376659479[44] = 1;
   out_988224469376659479[45] = 0;
   out_988224469376659479[46] = 0;
   out_988224469376659479[47] = 1;
   out_988224469376659479[48] = 0;
   out_988224469376659479[49] = 0;
   out_988224469376659479[50] = 0;
   out_988224469376659479[51] = 0;
   out_988224469376659479[52] = 0;
   out_988224469376659479[53] = 0;
}
void h_10(double *state, double *unused, double *out_7618912612446005780) {
   out_7618912612446005780[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7618912612446005780[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7618912612446005780[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_6151723592301710994) {
   out_6151723592301710994[0] = 0;
   out_6151723592301710994[1] = 9.8100000000000005*cos(state[1]);
   out_6151723592301710994[2] = 0;
   out_6151723592301710994[3] = 0;
   out_6151723592301710994[4] = -state[8];
   out_6151723592301710994[5] = state[7];
   out_6151723592301710994[6] = 0;
   out_6151723592301710994[7] = state[5];
   out_6151723592301710994[8] = -state[4];
   out_6151723592301710994[9] = 0;
   out_6151723592301710994[10] = 0;
   out_6151723592301710994[11] = 0;
   out_6151723592301710994[12] = 1;
   out_6151723592301710994[13] = 0;
   out_6151723592301710994[14] = 0;
   out_6151723592301710994[15] = 1;
   out_6151723592301710994[16] = 0;
   out_6151723592301710994[17] = 0;
   out_6151723592301710994[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_6151723592301710994[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_6151723592301710994[20] = 0;
   out_6151723592301710994[21] = state[8];
   out_6151723592301710994[22] = 0;
   out_6151723592301710994[23] = -state[6];
   out_6151723592301710994[24] = -state[5];
   out_6151723592301710994[25] = 0;
   out_6151723592301710994[26] = state[3];
   out_6151723592301710994[27] = 0;
   out_6151723592301710994[28] = 0;
   out_6151723592301710994[29] = 0;
   out_6151723592301710994[30] = 0;
   out_6151723592301710994[31] = 1;
   out_6151723592301710994[32] = 0;
   out_6151723592301710994[33] = 0;
   out_6151723592301710994[34] = 1;
   out_6151723592301710994[35] = 0;
   out_6151723592301710994[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_6151723592301710994[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_6151723592301710994[38] = 0;
   out_6151723592301710994[39] = -state[7];
   out_6151723592301710994[40] = state[6];
   out_6151723592301710994[41] = 0;
   out_6151723592301710994[42] = state[4];
   out_6151723592301710994[43] = -state[3];
   out_6151723592301710994[44] = 0;
   out_6151723592301710994[45] = 0;
   out_6151723592301710994[46] = 0;
   out_6151723592301710994[47] = 0;
   out_6151723592301710994[48] = 0;
   out_6151723592301710994[49] = 0;
   out_6151723592301710994[50] = 1;
   out_6151723592301710994[51] = 0;
   out_6151723592301710994[52] = 0;
   out_6151723592301710994[53] = 1;
}
void h_13(double *state, double *unused, double *out_3058136861535857965) {
   out_3058136861535857965[0] = state[3];
   out_3058136861535857965[1] = state[4];
   out_3058136861535857965[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4200498294708992280) {
   out_4200498294708992280[0] = 0;
   out_4200498294708992280[1] = 0;
   out_4200498294708992280[2] = 0;
   out_4200498294708992280[3] = 1;
   out_4200498294708992280[4] = 0;
   out_4200498294708992280[5] = 0;
   out_4200498294708992280[6] = 0;
   out_4200498294708992280[7] = 0;
   out_4200498294708992280[8] = 0;
   out_4200498294708992280[9] = 0;
   out_4200498294708992280[10] = 0;
   out_4200498294708992280[11] = 0;
   out_4200498294708992280[12] = 0;
   out_4200498294708992280[13] = 0;
   out_4200498294708992280[14] = 0;
   out_4200498294708992280[15] = 0;
   out_4200498294708992280[16] = 0;
   out_4200498294708992280[17] = 0;
   out_4200498294708992280[18] = 0;
   out_4200498294708992280[19] = 0;
   out_4200498294708992280[20] = 0;
   out_4200498294708992280[21] = 0;
   out_4200498294708992280[22] = 1;
   out_4200498294708992280[23] = 0;
   out_4200498294708992280[24] = 0;
   out_4200498294708992280[25] = 0;
   out_4200498294708992280[26] = 0;
   out_4200498294708992280[27] = 0;
   out_4200498294708992280[28] = 0;
   out_4200498294708992280[29] = 0;
   out_4200498294708992280[30] = 0;
   out_4200498294708992280[31] = 0;
   out_4200498294708992280[32] = 0;
   out_4200498294708992280[33] = 0;
   out_4200498294708992280[34] = 0;
   out_4200498294708992280[35] = 0;
   out_4200498294708992280[36] = 0;
   out_4200498294708992280[37] = 0;
   out_4200498294708992280[38] = 0;
   out_4200498294708992280[39] = 0;
   out_4200498294708992280[40] = 0;
   out_4200498294708992280[41] = 1;
   out_4200498294708992280[42] = 0;
   out_4200498294708992280[43] = 0;
   out_4200498294708992280[44] = 0;
   out_4200498294708992280[45] = 0;
   out_4200498294708992280[46] = 0;
   out_4200498294708992280[47] = 0;
   out_4200498294708992280[48] = 0;
   out_4200498294708992280[49] = 0;
   out_4200498294708992280[50] = 0;
   out_4200498294708992280[51] = 0;
   out_4200498294708992280[52] = 0;
   out_4200498294708992280[53] = 0;
}
void h_14(double *state, double *unused, double *out_4264661486828733996) {
   out_4264661486828733996[0] = state[6];
   out_4264661486828733996[1] = state[7];
   out_4264661486828733996[2] = state[8];
}
void H_14(double *state, double *unused, double *out_4951465325716144008) {
   out_4951465325716144008[0] = 0;
   out_4951465325716144008[1] = 0;
   out_4951465325716144008[2] = 0;
   out_4951465325716144008[3] = 0;
   out_4951465325716144008[4] = 0;
   out_4951465325716144008[5] = 0;
   out_4951465325716144008[6] = 1;
   out_4951465325716144008[7] = 0;
   out_4951465325716144008[8] = 0;
   out_4951465325716144008[9] = 0;
   out_4951465325716144008[10] = 0;
   out_4951465325716144008[11] = 0;
   out_4951465325716144008[12] = 0;
   out_4951465325716144008[13] = 0;
   out_4951465325716144008[14] = 0;
   out_4951465325716144008[15] = 0;
   out_4951465325716144008[16] = 0;
   out_4951465325716144008[17] = 0;
   out_4951465325716144008[18] = 0;
   out_4951465325716144008[19] = 0;
   out_4951465325716144008[20] = 0;
   out_4951465325716144008[21] = 0;
   out_4951465325716144008[22] = 0;
   out_4951465325716144008[23] = 0;
   out_4951465325716144008[24] = 0;
   out_4951465325716144008[25] = 1;
   out_4951465325716144008[26] = 0;
   out_4951465325716144008[27] = 0;
   out_4951465325716144008[28] = 0;
   out_4951465325716144008[29] = 0;
   out_4951465325716144008[30] = 0;
   out_4951465325716144008[31] = 0;
   out_4951465325716144008[32] = 0;
   out_4951465325716144008[33] = 0;
   out_4951465325716144008[34] = 0;
   out_4951465325716144008[35] = 0;
   out_4951465325716144008[36] = 0;
   out_4951465325716144008[37] = 0;
   out_4951465325716144008[38] = 0;
   out_4951465325716144008[39] = 0;
   out_4951465325716144008[40] = 0;
   out_4951465325716144008[41] = 0;
   out_4951465325716144008[42] = 0;
   out_4951465325716144008[43] = 0;
   out_4951465325716144008[44] = 1;
   out_4951465325716144008[45] = 0;
   out_4951465325716144008[46] = 0;
   out_4951465325716144008[47] = 0;
   out_4951465325716144008[48] = 0;
   out_4951465325716144008[49] = 0;
   out_4951465325716144008[50] = 0;
   out_4951465325716144008[51] = 0;
   out_4951465325716144008[52] = 0;
   out_4951465325716144008[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_2659138583199627985) {
  err_fun(nom_x, delta_x, out_2659138583199627985);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9150491803641430703) {
  inv_err_fun(nom_x, true_x, out_9150491803641430703);
}
void pose_H_mod_fun(double *state, double *out_3626402567552656885) {
  H_mod_fun(state, out_3626402567552656885);
}
void pose_f_fun(double *state, double dt, double *out_1920499082245897032) {
  f_fun(state,  dt, out_1920499082245897032);
}
void pose_F_fun(double *state, double dt, double *out_712434411548898420) {
  F_fun(state,  dt, out_712434411548898420);
}
void pose_h_4(double *state, double *unused, double *out_3116653058806651788) {
  h_4(state, unused, out_3116653058806651788);
}
void pose_H_4(double *state, double *unused, double *out_988224469376659479) {
  H_4(state, unused, out_988224469376659479);
}
void pose_h_10(double *state, double *unused, double *out_7618912612446005780) {
  h_10(state, unused, out_7618912612446005780);
}
void pose_H_10(double *state, double *unused, double *out_6151723592301710994) {
  H_10(state, unused, out_6151723592301710994);
}
void pose_h_13(double *state, double *unused, double *out_3058136861535857965) {
  h_13(state, unused, out_3058136861535857965);
}
void pose_H_13(double *state, double *unused, double *out_4200498294708992280) {
  H_13(state, unused, out_4200498294708992280);
}
void pose_h_14(double *state, double *unused, double *out_4264661486828733996) {
  h_14(state, unused, out_4264661486828733996);
}
void pose_H_14(double *state, double *unused, double *out_4951465325716144008) {
  H_14(state, unused, out_4951465325716144008);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
