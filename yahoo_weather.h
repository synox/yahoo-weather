/**
 * This is a library for the yahoo weather api, made for spark core (http://spark.io).
 *
 * It uses the Yahoo Query Language (YQL). It allows you to configure the data and the format.
 *
 * How to change the data query:
 *
 * 1. open in your browser:  http://developer.yahoo.com/yql/console/?q=select%20%20item.forecast.text%2C%20item.forecast.high%2C%20item.forecast.low%20from%20weather.forecast%20where%20u%3D'c'%20and%20woeid%3D%20781788
 * 2. configure the parameters,
 * 	e.g. "select  item from weather.forecast where u='c' and woeid= 781788" would return all weather data,
 * 	but the response would be too to handle.
 * 3. select "json" and copy the URL labeled "THE REST QUERY".
 * 4. paste it to the weather.cpp to the query address
 */

#ifndef _SPARKWEATHER
#define _SPARKWEATHER

#include "application.h"
#include "HttpClient.h"

// configure link at:

typedef struct {
	int temp_high = 99;
	int temp_low = 99;
	String descr;
	bool isSuccess = false;
} weather_response_t;


class Weather {
	public :
	void init(String woeid, HttpClient* client, bool isDegreeCelsius);
	weather_response_t update();

	private :
	String woeid;
	String unitsForTemperature;
	HttpClient* client;
	weather_response_t response;
	weather_response_t parse(String json);
	String readData(String s, String needle, String endMark);

	long parseInt(String s);

};

class WeatherCache {
public:
	void init(Weather* w);
	weather_response_t lazyUpdate();
	private:
	unsigned long weather_sync_interval;
	Weather* weather;
	unsigned long lastsync;
	weather_response_t lastReponse;
};

#endif
