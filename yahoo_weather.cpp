// see yahoo_weather.h for description
#include "yahoo_weather.h"

void Weather::init(String id, HttpClient* client) {
	this->woeid = id;
	this->client = client;
}

weather_response_t Weather::update() {
	Serial.println("loading weather");
	http_request_t request;
	request.path = "/v1/public/yql?q=select%20%20item.forecast.text%2C%20item.forecast.high%2C%20item.forecast.low%20from%20weather.forecast%20where%20u%3D'c'%20and%20woeid%3D%20"+woeid+"&format=json&callback=";
	request.body = "";
	Serial.println(request.path);

	this->client->init("query.yahooapis.com", 80);
	http_response_t response = this->client->get(request);
	if (response.status == 200) {
		Serial.print("weather response: ");
		Serial.println(response.body);
		return parse(response.body);
	} else {
		Serial.print("weather request failed ");
		weather_response_t responseFailed;
		return responseFailed;
	}
}

String Weather::readData(String s, String needle, String endMark) {
	int begin = s.indexOf(needle) + needle.length();
	int end = s.indexOf(endMark, begin+1);
	Serial.print(s);
	Serial.print(begin);
	Serial.print(end);
	String result= s.substring(begin, end );
	Serial.println(result);
	return result;
}

long Weather::parseInt(String s) {
	if (s != NULL) {
		return s.toInt();
	} else {
		return -99;
	}
}

weather_response_t Weather::parse(String jsonData) {
	weather_response_t response;
	response.temp_high = parseInt(readData(jsonData, "high\":\"", "\""));
	response.temp_low = parseInt(readData(jsonData, "low\":\"", "\""));
	response.descr = readData(jsonData, "text\":\"", "\"");
	response.isSuccess = true;
	return response;
}

void WeatherCache::init(Weather* w) {
	this->weather = w;
	this->lastsync = 0;
	this->weather_sync_interval = 1000 * 3600 * 2; // milliseconds
}

weather_response_t WeatherCache::lazyUpdate() {
	if( lastsync == 0 || (lastsync + weather_sync_interval) < millis() ) {
		lastReponse  = weather->update();
		if(lastReponse.isSuccess) {
			lastsync = millis();
		}
	} else {
		Serial.println("using cached weather");
	}
	return lastReponse;
}


// example:

// http://query.yahooapis.com/v1/public/yql?q=select%20%20item.forecast.text%2C%20item.forecast.high%2C%20item.forecast.low%20from%20weather.forecast%20where%20u%3D'c'%20and%20woeid%3D%20781788&format=json&callback=

/*{
 "query": {
  "count": 5,
  "created": "2014-03-13T20:43:34Z",
  "lang": "en-US",
  "results": {
   "channel": [
    {
     "item": {
      "forecast": {
       "high": "16",
       "low": "1",
       "text": "Clear"
      }
     }
    },
    {
     "item": {
      "forecast": {
       "high": "17",
       "low": "3",
       "text": "Sunny"
      }
     }
    },
    {
     "item": {
      "forecast": {
       "high": "16",
       "low": "2",
       "text": "Mostly Cloudy"
      }
     }
    },
    {
     "item": {
      "forecast": {
       "high": "16",
       "low": "1",
       "text": "Partly Cloudy"
      }
     }
    },
    {
     "item": {
      "forecast": {
       "high": "19",
       "low": "2",
       "text": "Sunny"
      }
     }
    }
   ]
  }
 }
} */
