#include "rtc.h"

/**
 * @brief Get actual internal RTC date in UNIX format.
 * 
 * @return UNIX date.
 */
int32_t RTC::get_unix()
{
	time_t now = 0;
	time(&now);
	return now+(3600*tz);
}

/**
 * @brief Get actual internal RTC date with specific string format.
 * 
 * This special function can be used to output formatted date (string)
 * with any format (specified by strftime(), to more details, Google it!)
 * 
 * Default format is "DAY/MONTH/YEAR HOUR:MINUTES:SECONDS"
 * 
 * @attention Destiny char array need to be >= 18B for default format.
 * 
 * @param [*dst]: Destiny char_array which will contain the current date.
 * @param [*format]: Specific formatted string output date.
 */
void RTC::get_string(char *dst, const char *format="%d/%m/%y %H:%M:%S")
{
	struct tm *stm;

	time_t now = get_unix();
	stm = gmtime(&now);

	strftime(dst, 18, format, stm);
}


/**
 * @brief Set actual internal RTC date with UNIX number.
 * 
 * @param [time]: UNIX to be set.
 */
void RTC::set_unix(int32_t time)
{
	struct timeval stv;
	stv.tv_sec = time;
	stv.tv_usec = 0;
	settimeofday(&stv, NULL);
}

/**
 * @brief Set actual internal RTC date with one-by-one parameter.
 * 
 * @param [day]: Day. (1-31)
 * @param [month]: Month. (1-12)
 * @param [year]: Year. Eg: 2020.
 * @param [hour]: Hour. (0-23)
 * @param [min]: Minutes. (0-59)
 * @param [sec]: Seconds. (0-59)
 */
void RTC::set_simple(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t min, uint8_t sec)
{
	struct tm stm;
	struct timeval stv;
	time_t date;

	stm.tm_mday = day;
	stm.tm_mon  = month-1;
	stm.tm_year = year-1900;
	stm.tm_hour = hour;
	stm.tm_min  = min;
	stm.tm_sec  = sec;
	date = mktime(&stm);

	stv.tv_sec = date;
	stv.tv_usec = 0;
	settimeofday(&stv, NULL);
}

/**
 * @brief Set actual internal RTC date automatic by NTP.
 * 
 * @attention This function will block code (task) for max 15secs.
 * @attention WiFi with internet connection is needed.
 * 
 * 
 * @param [*server]: NTP server to get date. Eg: "a.ntp.br"
 * @param [timezone]: Timezone to apply with NTP successful. (because NTP return in UTC)
 * 
 * @return 0: Sync fail.
 * @return 1: Sync sucess.
 */
uint8_t RTC::set_ntp(const char *server, int8_t timezone=0)
{
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, (char*)server);
	sntp_init();

	for (uint8_t i = 0; i < 60; i++)//15sec timeout
	{
		vTaskDelay(pdMS_TO_TICKS(250));
		if (get_unix() > 100000) {break;}
	}

	if (get_unix() > 100000)//Sucess
	{
		tz = timezone;
		return 1;
	}
	else//Fail
	{
		sntp_stop();
		return 0;
	}
}
