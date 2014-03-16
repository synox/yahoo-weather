Weather Library for Spark-Core
=============

A library for the yahoo weather api, made for spark core (http://spark.io).

Please read the yahoo [Usage Information and Limits](http://developer.yahoo.com/yql/guide/usage_info_limits.html). 

> This library should be considered **alpha** tested software.  It is not ready for use in production without testing and bugfixing. 

## Data
Currently the library loads the high and low temperature for today, including a short description. 

It can easily be extended, **please fork and send pull requests!**

Here is a list of the possible descriptions: http://developer.yahoo.com/weather/#codes

## Dependencies
It's using the [HttpClient library](https://github.com/nmattisson/HttpClient) for spark core. Before compiling, add those files to your workspace. If you are building locally add it to `build.mk`. 

## Known issues
* sometimes it does not connect to yahoo api on first try. after a few tries it should work. 


## Usage
> To find your WOEID, browse or search for your city from the Weather home page. 
> The WOEID is in the URL for the forecast page for that city. 
> -- <cite>from the [yahoo manual](http://developer.yahoo.com/weather/#req)</cite>


You have to call the init() method with your woeid, an instance of httpClient and a boolean for celsius/fahrenheit. 

in setup(): 

```c++
		weather.init("781788", httpClient,true);
		weatherCache.init(&weather); 
```


in loop(): 

the cache class caches the response for two hours: 
```c++
		weather_response_t resp = weatherCache.lazyUpdate();
		if(resp.isSuccess) {
			// print(resp.temp_high);
		}
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
```

Have fun! Share!