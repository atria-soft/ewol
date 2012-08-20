/**
 *******************************************************************************
 * @file ewol/os/Fps.h
 * @brief fps counter and displayer (Header)
 * @author Edouard DUPIN
 * @date 19/08/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary :
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_FPS_H__
#define __EWOL_FPS_H__

namespace ewol
{
	/**
	 * @brief This class is designed to count the number of frame per second in the main renderer system
	 * @param ---
	 * @return ---
	 */
	class Fps
	{
		// display every second ...
		#define DISPLAY_PERIODE_US       (1000000)
		private:
			int64_t startTime;
			int64_t nbCallTime;
			int64_t nbDisplayTime;
			int64_t min;
			int64_t avg;
			int64_t max;
			int64_t min_idle;
			int64_t avg_idle;
			int64_t max_idle;
			int64_t ticTime;
			bool display;
			bool drwingDone;
		public:
			/**
			 * @brief Constructor
			 * @param ---
			 * @return ---
			 */
			Fps(void)
			{
				startTime = -1;
				nbCallTime = 0;
				nbDisplayTime = 0;
				min = 99999999999999LL;
				avg = 0;
				max = 0;
				min_idle = 99999999999999LL;
				avg_idle = 0;
				max_idle = 0;
				ticTime = 0;
				display = false;
				drwingDone = false;
			}
			/**
			 * @brief Destructor
			 * @param ---
			 * @return ---
			 */
			~Fps(void)
			{
				
			}
			/**
			 * @brief this might be call every time a diplay start
			 * @param ---
			 * @return ---
			 */
			void Tic(void)
			{
				int64_t currentTime = ewol::GetTime();
				ticTime = currentTime;
				nbCallTime++;
				if (startTime<0) {
					startTime = currentTime;
				}
				//EWOL_DEBUG("current : " << currentTime << "time    diff : " << (currentTime - startTime));
				if ( (currentTime - startTime) > DISPLAY_PERIODE_US) {
					display = true;
				}
			}
			/**
			 * @brief this might be call every time a diplay stop, it do the display every second
			 * @param ---
			 * @return ---
			 */
			void Toc(void)
			{
				int64_t currentTime = ewol::GetTime();
				int64_t processTimeLocal = (currentTime - ticTime);
				if (drwingDone) {
					min = etk_min(min, processTimeLocal);
					max = etk_max(max, processTimeLocal);
					avg += processTimeLocal;
					drwingDone = false;
				} else {
					min_idle = etk_min(min_idle, processTimeLocal);
					max_idle = etk_max(max_idle, processTimeLocal);
					avg_idle += processTimeLocal;
				}
				if (true == display) {
					EWOL_DEBUG("display property : " << nbDisplayTime << "/" << nbCallTime << "fps");
					if (nbDisplayTime>0) {
						EWOL_DEBUG("Time Drawind : " << (float)((float)min / 1000.0) << "ms "
						                             << (float)((float)avg/(float)nbDisplayTime / 1000.0) << "ms "
						                             << (float)((float)max / 1000.0) << "ms ");
					}
					if (nbCallTime-nbDisplayTime>0) {
						EWOL_DEBUG("Time idle    : " << (float)((float)min_idle / 1000.0) << "ms "
						                             << (float)((float)avg_idle/(float)(nbCallTime-nbDisplayTime) / 1000.0) << "ms "
						                             << (float)((float)max_idle / 1000.0) << "ms ");
					}
					max = 0;
					min = 99999999999999LL;
					avg = 0;
					max_idle = 0;
					min_idle = 99999999999999LL;
					avg_idle = 0;
					nbCallTime = 0;
					nbDisplayTime = 0;
					startTime = -1;
					display = false;
				}
			}
			/**
			 * @brief this might be call when a display is really done
			 * @param ---
			 * @return ---
			 */
			void IncrementCounter(void)
			{
				nbDisplayTime++;
				drwingDone = true;
			}
	};
};

/**
 * @brief This fuction display and calculate the curent frame per second of the display
 * @note this function must be call 2 time the first at the start of the display and the second at the end of this one
 * @param[in] Represent the mode of the counter
 * @param[in] the curent system time
 * @ return ---
 */

#endif