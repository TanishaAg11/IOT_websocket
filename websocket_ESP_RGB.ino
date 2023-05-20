// Onboard rgb led on off code using websocket
// ESP32S2 Dev Module & COM 7 is selected
// if esp not connetcting to wifi ....press RST button just once

#include <Adafruit_NeoPixel.h>

#define PIN 18 

Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 75 

#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Tani";
const char* password = "tani@123";

bool showPixel;
int colorCount = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 2</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
</script>
</body>
</html>
)rawliteral";

void notifyClients() {
  ws.textAll(String(showPixel));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      showPixel = !showPixel;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (showPixel){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

uint32_t Wheel1(byte WheelPos)
{
    //Assume the wheel value is less than 85, if so Green value is 0
    uint32_t returnColor = Adafruit_NeoPixel::Color((byte)(255 - (WheelPos * 5)), 0, (byte)(WheelPos * 5));

    //If we are greater than 170 Red value is 0
    if (WheelPos > 50 && WheelPos < 102)
    {
        WheelPos -= 51;
        returnColor = Adafruit_NeoPixel::Color(0, (byte)(WheelPos * 5), (byte)(255 - WheelPos * 5));
    }
    //Finally above 170 and Blue value is 0
    else if (WheelPos >= 102 && WheelPos < 153)
    {
        WheelPos -= 102;
        returnColor = Adafruit_NeoPixel::Color((byte)(WheelPos * 5), (byte)(255 - WheelPos * 5), 0);
    }

    else if (WheelPos >= 153 && WheelPos < 204)
    {
        WheelPos -= 153;
        returnColor = Adafruit_NeoPixel::Color((byte)(255 - WheelPos * 5), (byte)(255 - WheelPos * 5), (byte)(255 - WheelPos * 5));
    }

    else if (WheelPos >= 204)
    {
        WheelPos -= 204;
        returnColor = Adafruit_NeoPixel::Color((byte)(WheelPos * 5), (byte)(WheelPos * 5), (byte)(WheelPos * 5));
    }
    return returnColor;
}
uint32_t Wheel2(byte WheelPos)
{
    uint32_t returnColor = Adafruit_NeoPixel::Color(0, 0, 0);
    return returnColor;
}

void setup(){

  pixels.setBrightness(10);
  pixels.begin();


  // Serial port for debugging purposes
  Serial.begin(115200);

  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  //digitalWrite(ledPin, ledState);
    if(showPixel == true){
      pixels.setPixelColor(0, Wheel1(colorCount++));
          pixels.show();   
          if (colorCount > 255)
              colorCount = 0;
          delay(DELAYVAL); 
    }
    else{
      pixels.setPixelColor(0, Wheel2(colorCount));
      pixels.show();
    }
}