function parseResponse(responseText) { 
     var response = JSON.parse(responseText); 
     var condition = response.weather[0].main; 
     var city = response.name; 
     var country = response.sys.country; 
     var degK =  response.main.temp; //temperature data is in Kelvin 
     var degF =  1.8*(degK - 273) + 32; //Converting Kelvin to Fahrenheit.  
     degF = Math.floor(degF); //removing decimals 
     var weatherBox = document.getElementById("weather");  
     response = city + ", " + country + " " + degF + "\u{00B0}" + " F " + condition; 
     return response;  
}