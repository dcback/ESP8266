#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ssid      "Kiet_iptime"     // WiFi SSID
#define password  "1700note"        // WiFi password

const uint8_t GPIOPIN[4] = {D4, D5, D6, D7}; // Led
float   tempValue,  pretempValue;
float   humiValue,  prehumiValue;
float   co2Value,   preco2Value;
String  ledGpio[4]  = {"OFF", "OFF", "OFF", "OFF"};
int     cnt         = 0;

// Creating objects / create Objects
ESP8266WebServer server ( 80 );

String getPage() {
    String page = "<html lang='kr'>";
    page += "<head>";
    page +=     "<meta http-equiv='refresh' content='60'>";
    page +=     "<name='viewport' content='width=device-width, initial-scale=1'>";
    page +=         "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
    page +=         "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script>";
    page +=         "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
    page +=         "<title>ESP8266 Demo</title>";
    page += "</head>";
    page += "<body>";   
    page +=     "<div class='container-fluid'>";     
    page +=         "<div class='row'>";
    page +=             "<div class='col-md-12'>";
    page +=                 "<h1>Demo Webserver ESP8266</h1>";
    page +=                 "<h3>Sensing & Control Mini station </h3>";
    page +=                 "<ul class='nav nav-pills'>";
    page +=                     "<li class='active'>";
    page +=                         "<a href='#'> <span class='badge pull-right'>"; 
    page +=                         tempValue; 
    page +=                         " &deg;C</span> Temperature</a>";
    page +=                     "</li>";
    page +=                     "<li class='active'>";
    page +=                         "<a href='#'> <span class='badge pull-right'>"; 
    page +=                         humiValue; 
    page +=                         " %</span> Humidity</a>";
    page +=                     "</li>";
    page +=                     "<li class='active'>";
    page +=                         "<a href='#'> <span class='badge pull-right'>"; 
    page +=                         co2Value;
    page +=                         " ppm</span> Value of CO2</a>";
    page +=                     "</li>";
    page +=                 "</ul>";
    page +=                 "<table class='table'>";
    page +=                     "<thead>";
    page +=                         "<tr>";
    page +=                             "<th>Sensor</th>";
    page +=                             "<th>Measure</th>";  
    page +=                             "<th>Value</th>"; 
    page +=                             "<th>Previous Value</th>";  
    page +=                         "<tr>";
    page +=                     "</thead> ";
    page +=                     "<tbody>";
    page +=                         "<tr>";
    page +=                             "<td>BME280</td>";
    page +=                             "<td>Temperature</td>";
    page +=                             "<td>"; 
    page +=                                 tempValue; 
    page +=                                 " &deg;C</td>";
    page +=                             "<td>";
    page +=                                 pretempValue; 
    page +=                                 " &deg;C</td>";
    page +=                         "</tr>";
    page +=                         "<tr class='active'>";
    page +=                             "<td>BME280</td>";
    page +=                             "<td>Humidity</td>"; 
    page +=                             "<td>"; 
    page +=                                 humiValue; 
    page +=                                 " %</td>";
    page +=                             "<td>"; 
    page +=                                 prehumiValue; 
    page +=                                 " %</td>";
    page +=                         "</tr>";
    page +=                         "<tr>";
    page +=                             "<td>MH-Z16</td>";
    page +=                             "<td>CO2</td>";
    page +=                             "<td>"; 
    page +=                                 co2Value; 
    page +=                                 " ppm</td>";
    page +=                             "<td>"; 
    page +=                                 preco2Value; 
    page +=                                 " ppm</td>";
    page +=                         "</tr>";
    page +=                     "</tbody>";
    page +=                 "</table>";
    page +=                 "<h3>GPIO</h3>";
    page +=                 "<table>";
    page +=                     "<tr>"; 
    page +=                         "<td style='width:200px'><h4 class='text-left'>D4 <span class='badge'>"; 
    page +=                             ledGpio[0]; 
    page +=                             "</span></h4></td>"; 
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D4' value='1'";
    page +=                             "class='btn btn-success btn-lg'>ON</button></form></td>";                 
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D4' value='0'";
    page +=                             "class='btn btn-danger btn-lg'>OFF</button></form></td>";  
    page +=                     "</tr>"; 
    page +=                     "<tr>";
    page +=                         "<td style='width:200px'><h4 class='text-left'>D5 <span class='badge'>";
    page +=                             ledGpio[1]; 
    page +=                             "</span></h4></td>";          
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D5' value='1'";
    page +=                         "class='btn btn-success btn-lg'>ON</button></form></td>"; 
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D5' value='0'";
    page +=                         "class='btn btn-danger btn-lg'>OFF</button></td>"; 
    page +=                     "</tr>";
    page +=                     "<tr>";
    page +=                         "<td style='width:200px'><h4 class='text-left'>D6 <span class='badge'>"; 
    page +=                             ledGpio[2]; 
    page +=                             "</span></h4></td>";
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D6' value='1'";
    page +=                             "class='btn btn-success btn-lg'>ON</button></form></td>";
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D6' value='0'";
    page +=                             "class='btn btn-danger btn-lg'>OFF</button></form></td>";
    page +=                     "</tr>";
    page +=                     "<tr>";
    page +=                         "<td style='width:200px'><h4 class='text-left'>D7 <span class='badge'>"; 
    page +=                             ledGpio[3]; 
    page +=                             "</span></h4></td>";
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D7' value='1'";
    page +=                             "class='btn btn-success btn-lg'>ON</button></form></td>"; 
    page +=                         "<td style='width:200px'><form action='/' method='POST'><button type='button submit' name='D7' value='0'";
    page +=                             "class='btn btn-danger btn-lg'>OFF</button></form></td>"; 
    page +=                     "</tr>";
    page +=                 "</table>";
    page +=                 "<p><a href='https://github.com/dcback/ESP8266/blob/master/Example/WebServer/esp8266_Server4.ino'target='_blank'>GithubLink</p>";
    page +=             "</div>";
    page +=         "</div>";
    page +=        "</div>";
    page +=     "</body>";
    page += "</html>";
    return page;
}

void handleRoot() {
    if ( server.hasArg("D4") ) {
        handleD4();
    } else if ( server.hasArg("D5") ) {
        handleD5();
    } else if ( server.hasArg("D6") ) {
        handleD6();
    } else if ( server.hasArg("D7") ) {
        handleD7();
    } else {
        server.send ( 200, "text/html", getPage() );
    }
}

void handleD4() {
    String D4Value;
    updateGPIO(0, server.arg("D4"));
}

void handleD5() {
    String D5Value;
    updateGPIO(1, server.arg("D5"));
}

void handleD6() {
    String D6Value;
    updateGPIO(2, server.arg("D6"));
}

void handleD7() {
    String D7Value;
    updateGPIO(3, server.arg("D7"));
}

void updateGPIO(int gpio, String DxValue) {
    Serial.println("");
    Serial.println("Update GPIO "); Serial.print(GPIOPIN[gpio]); Serial.print(" -> "); Serial.println(DxValue);

    if ( DxValue == "1" ) {
        digitalWrite(GPIOPIN[gpio], LOW);       // default HIGH
        ledGpio[gpio] = "On";
        server.send ( 200, "text/html", getPage() );
    } else if ( DxValue == "0" ) {
        digitalWrite(GPIOPIN[gpio], HIGH);      // default LOW
        ledGpio[gpio] = "Off";
        server.send ( 200, "text/html", getPage() );
    } else {
        Serial.println("Err Led Value");
    }
}

void setup() {
    for ( int x = 0 ; x < 5 ; x++ ) {
        pinMode(GPIOPIN[x], OUTPUT);
        digitalWrite(GPIOPIN[x], HIGH); // HIGH = OFF, LOW = ON
    }
    Serial.begin ( 115200 );

    WiFi.begin ( ssid, password );
    // Waiting for the WiFi connection / Wait for connection
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 ); Serial.print ( "." );
    }
    // WiFi connection established / WiFi connection is OK
    Serial.println ( "" );
    Serial.print ( "Connected to " ); Serial.println ( ssid );
    Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

    // Connect the function that manages the first page / link to the function that manage launch page
    server.on ( "/", handleRoot );

    server.begin();
    Serial.println ( "HTTP server started" );
}

void loop() {
    // put your main code here, to run repeatedly:
    server.handleClient();

    pretempValue = 25.0 + cnt;
    prehumiValue = 50.0 + cnt;
    preco2Value = 1000.0 + cnt;

    cnt++;
    
    tempValue = 25.0 + cnt;
    humiValue = 50.0 + cnt;
    co2Value = 1000.0 + cnt;
    
    if ( cnt == 10 ) cnt = 0;
    Serial.print("T : "); Serial.println(tempValue);
    Serial.print("H : "); Serial.println(humiValue);
    Serial.print("CO2 : "); Serial.println(co2Value);
    delay(1000);
}
