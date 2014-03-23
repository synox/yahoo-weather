Weather Library for Spark-Core
=============

A library for the yahoo weather api, made for spark core (http://spark.io).

Please read the yahoo [Usage Information and Limits](http://developer.yahoo.com/yql/guide/usage_info_limits.html). 

> This library should be considered **alpha** tested software.  It is not ready for use in production without testing and bugfixing. 

## Weather data
Currently the library loads the high and low temperature for today, including a short description. example: 

	{
	 "high": "16",
	 "low": "1",
	 "text": "Mostly Cloudy"
	}

(Here is a list of the possible "text" values: http://developer.yahoo.com/weather/#codes )

### How to include other data?
It can easily be extended, you can fork this project and send pull requests!

1. open in your browser:  http://developer.yahoo.com/yql/console/?q=select%20item%20from%20weather.forecast%20where%20u%3D'c'%20and%20woeid%3D%20781788
2. press ``Test``, enable "Tree View".
3. read the output and choose wich data you need. e.g. atmosphere-humidity
4. limit your query to only select what you need. (it's a tree.) example: ``select atmosphere.humidity, wind.speed  from weather.forecast where woeid=2502265``
5. select "json" and copy the URL labeled "THE REST QUERY". Test it in your browser or wget/curl, keep the output as a reference for implementation. 
6. paste the URL into the ``weather.cpp`` to ``request.path = "/v1/...``
7. adjust the method``Weather::parse()`` to respect the other json parameters. Compare with the json output, watch out for spaces. Add more variables to the ``weather_response_t`` struct. 
8. make tests, deploy, leave feedback


## Known issues
* sometimes it does not connect to yahoo api on first try. after a few tries it should work. 

## Dependencies
It's using the [HttpClient library](https://github.com/nmattisson/HttpClient) for spark core. Before compiling, add those files to your workspace. If you are building locally add it to `build.mk`. 


## Usage
> To find your WOEID, browse or search for your city from the [Yahoo Weather home page](http://weather.yahoo.com/). 
> The WOEID is in the URL for the forecast page for that city. 
> (from the [yahoo manual](http://developer.yahoo.com/weather/#req) )


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
	#include "yahoo_weather.h"
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
