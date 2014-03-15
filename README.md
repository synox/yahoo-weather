Weather Library for Spark-Core
=============

A library for the yahoo weather api, made for spark core (http://spark.io).

Please read the yahoo [Usage Information and Limits](http://developer.yahoo.com/yql/guide/usage_info_limits.html). 

## Data
Currently the library loads the high and low temperature for today, including a short description. 

It can easily be extended, **please fork and send pull requests!**

## Dependencies
It's using the [HttpClient library](https://github.com/nmattisson/HttpClient) for spark core. Before compiling, add those files to your workspace. If you are building locally add it to `build.mk`. 

## Usage
You have to find your location code "woeid". 

open the following link in your browser and replace the city name: 

http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20geo.places%20where%20text=%22New%20York,US%22

You have to call init with the httpClient and your woeid. 

in setup(): 

```c++
		weather.init("781788", httpClient);
		weatherCache.init(&weather); 
```


in loop(): 

the cache class caches the response for two hours: 
```c++
		weather_response_t resp = weatherCache.lazyUpdate();
```


or you can access the update directly: 
```c++
		weather_response_t resp = weather->update();
```

## Full usage example
```c++

	#include "Adafruit_CharacterOLED.h"
	#include "Weather.h"
	#include "HttpClient.h"
	
	
	unsigned int nextTime = 0;    // next time to contact the server
	
	Weather weather;
	WeatherCache weatherCache;
	HttpClient* httpClient;
	
	void setup() {
		httpClient = new HttpClient();

		lcd = new Adafruit_CharacterOLED(...);
	
		// weather
		weather.init("781788", httpClient);
		weatherCache.init(&weather); 
	}
	
	void loop() {
		if (nextTime > millis()) {
			// keep the same color while waiting
			return;
		}
	 
		// print weather
		weather_response_t resp = weatherCache.lazyUpdate();
		if (lcd != NULL && resp.isSuccess) {
			lcd->setCursor(0, 1);
			lcd->print(resp.temp_low);
			lcd->print("-");
			lcd->print(resp.temp_high);
			lcd->print(" ");
			lcd->print(resp.descr);
		}
	
		// check again in 5 seconds:
		nextTime = millis() + 5000;
	}
``

Have fun! Share!