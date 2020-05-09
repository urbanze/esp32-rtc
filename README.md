# ESP32 IDF RTC library
All functions have comments. WiFi Library used: [WiFi](https://github.com/urbanze/esp32-wifi)\
Attention: if you use set_ntp(), need start WiFi/Internet before.\
With minor changes, this library will work in any system with POSIX time functions.

## Simple example to set/get date
```
RTC rtc;
rtc.set_unix(1577836800); //Set date to 01/01/2020 00:00:00
//rtc.set_simple(1, 1, 2020, 12, 00, 00); //Set date to 01/01/2020 12:00:00

while (1)
{
    //Default format string will use 18 chars.
    char date[18] = {0};
    rtc.get_string(date);

    ESP_LOGI(__func__, "UNIX: %d, Formatted: %s", rtc.get_unix(), date);
    vTaskDelay(pdMS_TO_TICKS(1000));
}
```

## Simple example to set/get date with NTP
```
WF wifi;
RTC rtc;

wifi.sta_connect("wifi", "1234567890"); //Connect in WiFi

//Start and wait for NTP sync with -3 hour timezone (BR)
if (rtc.set_ntp("a.ntp.br", -3))
{
    ESP_LOGI(__func__, "NTP Sucess");
}
else
{
    //If fail, set manually

    ESP_LOGE(__func__, "NTP Fail");
    //rtc.set_unix(1577836800); //Set date to 01/01/2020 00:00:00
    rtc.set_simple(1, 1, 2020, 12, 00, 00); //Set date to 01/01/2020 12:00:00
}

while (1)
{
    //Default format string will use 18 chars.
    char date[18] = {0};
    rtc.get_string(date);

    ESP_LOGI(__func__, "UNIX: %d, Formatted: %s", rtc.get_unix(), date);
    vTaskDelay(pdMS_TO_TICKS(1000));
}
```