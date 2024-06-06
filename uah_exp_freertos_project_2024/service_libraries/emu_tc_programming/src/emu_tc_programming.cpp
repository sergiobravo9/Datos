/*
 * emu_tc_programming.cpp
 *
 *  Created on: Jan 13, 2017
 *      Author: user
 */

#include <public/emu_hw_timecode_drv_v1.h>
#include <public/emu_sc_channel_drv_v1.h>
#include <public/emu_gss_v1.h>


#define FT_UAH_EXP_SERV_129_00XX
#define FT_SOLO_EPD_ICU_SERV_3_UPDATE_INTERVAL_TC_0050

//TODO


#ifdef FT_UAH_EXP_SERV_129_00XX


EmuGSS_TCProgram129_1 prog_FT_00XX_step_0(UNITIME_AFTER_POWER_ON + 5,
					"FT_UAH_EXP_SERV_129_00XX step 0, Set Kp", 0.2, 0.2);

EmuGSS_TCProgram129_2 prog_FT_00XX_step_0(UNITIME_AFTER_POWER_ON + 5,
					"FT_UAH_EXP_SERV_129_00XX step 0, Set Command Vx and Vy "
					, 0, 0);
#endif

#ifdef FT_SOLO_EPD_ICU_SERV_3_UPDATE_INTERVAL_TC_0050



#define FT_SOLO_EPD_ICU_SERV_3_0050_TIME_step0 (UNITIME_AFTER_POWER_ON)
#define FT_SOLO_EPD_ICU_SERV_3_0050_TIME_step1 (FT_SOLO_EPD_ICU_SERV_3_0050_TIME_step0 + 10)

EmuGSS_TCProgram3_31 prog_FT_0050_step_0(FT_SOLO_EPD_ICU_SERV_3_0050_TIME_step0,
		"FT_SOLO_EPD_ICU_SERV_3_0050 step 0, Update SID 10 Rate to 5",10,5);

EmuGSS_TCProgram3_5 prog_FT_0050_step_1(FT_SOLO_EPD_ICU_SERV_3_0050_TIME_step1,
		"FT_SOLO_EPD_ICU_SERV_3_0050 step 2, Enable SID 10 ",10);
#endif


