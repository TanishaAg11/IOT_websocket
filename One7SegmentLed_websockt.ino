//single addressable led control using previous websocket code 



#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

// #define CLK_PIN   43

#define LED_PIN 26    //for ESP32 S2
#define NUM_LEDS 41   // no. of leds in one segment

char no;
CRGB selectedled[NUM_LEDS];
int leds0[NUM_LEDS] = {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   0,0,0,0,0};
int leds1[NUM_LEDS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   0,0,0,0,0,   0,0,0,0,0};
int leds2[NUM_LEDS] = {1,0,0,0,0,0,1,1,1,1,1,1,1,    1,1,1,1,1,   1,0,0,0,0,0,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};
int leds3[NUM_LEDS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};
int leds4[NUM_LEDS] = {1,1,1,1,1,1,1,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   0,0,0,0,0,   1,1,1,1,1};
int leds5[NUM_LEDS] = {1,1,1,1,1,1,1,0,0,0,0,0,0,    1,1,1,1,1,   1,1,1,1,1,1,1,0,0,0,0,0,1,   1,1,1,1,1,   1,1,1,1,1};
int leds6[NUM_LEDS] = {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,0,0,0,0,0,1,   1,1,1,1,1,   1,1,1,1,1};
int leds7[NUM_LEDS] = {1,0,0,0,0,0,0,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   0,0,0,0,0};
int leds8[NUM_LEDS] = {1,1,1,1,1,1,1,1,1,1,1,1,1,    1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};
int leds9[NUM_LEDS] = {1,1,1,1,1,1,1,0,0,0,0,0,0,    0,0,0,0,0,   1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,   1,1,1,1,1};

void copyArray(CRGB*arr1, int*arr2){
  for(int i = 0; i<41; i++){
    //Serial.print(arr2[i]);
    //Serial.print(" "); 
    if(arr2[i])   
      arr1[i] = 1;
    else
      arr1[i] = 0;
  }
}
#define BRIGHTNESS  96


const char* ssid = "Tani";
const char* password = "tani@123";

//bool ledState = 0;
//const int ledPin = 2;

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
      
      <p><button ndata="0" class="button">0</button></p>
      <p><button ndata="1" class="button">1</button></p>
      <p><button ndata="2" class="button">2</button></p>
      <p><button ndata="3" class="button">3</button></p>
      <p><button ndata="4" class="button">4</button></p>
      <p><button ndata="5" class="button">5</button></p>
      <p><button ndata="6" class="button">6</button></p>
      <p><button ndata="7" class="button">7</button></p>
      <p><button ndata="8" class="button">8</button></p>
      <p><button ndata="9" class="button">9</button></p>

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
  // function toggle(){
  //   websocket.send('toggle');
  // }
function sendnumber(evt){
  let n = evt.target.getAttribute("ndata")
  console.log(n)
   websocket.send(n)
  // let n = element.getAttribute("ndata")
  //   console.log(n)
  
}
function forEachcallback(ele){
  
  // console.log(ele.getAttribute("ndata"))
  ele.onclick = sendnumber

}
  document.querySelectorAll(".button").forEach(forEachcallback);
</script>
</body>
</html>

)rawliteral";

void notifyClients() {
  //ws.textAll(String(ledState));
  ws.textAll(String(no));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "0") == 0) {
      no = '0';
      //notifyClients();
      Serial.println(no);
    }
    else if(strcmp((char*)data, "1") == 0){
      no = '1';
      Serial.println(no);
    }
    else if(strcmp((char*)data, "2") == 0){
      no = '2';
    }
    else if(strcmp((char*)data, "3") == 0){
      no = '3';
    }
    else if(strcmp((char*)data, "4") == 0){
      no = '4';
    }
    else if(strcmp((char*)data, "5") == 0){
      no = '5';
    }
    else if(strcmp((char*)data, "6") == 0){
      no = '6';
    }
    else if(strcmp((char*)data, "7") == 0){
      no = '7';
    }
    else if(strcmp((char*)data, "8") == 0){
      no = '8';
    }
    else if(strcmp((char*)data, "9") == 0){
      no = '9';
    }
    notifyClients();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t*data, size_t len) {
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

// String processor(const String& var){
//   Serial.println(var);
//   if(var == "STATE"){
//     if (ledState){
//       return "ON";
//     }
//     else{
//       return "OFF";
//     }
//   }
//   return String();
// }

void setup(){
  // Serial port for debugging purposes

  FastLED.addLeds<WS2812, LED_PIN, GRB>(selectedled, NUM_LEDS);


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
    request->send_P(200, "text/html", index_html /*processor*/);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  //digitalWrite(ledPin, ledState);

  //no = Serial.read();
    switch(no)
    {
      case '0' :
      copyArray(selectedled, leds0);
      //Serial.println(0);
      break;
      case '1' :
      copyArray(selectedled, leds1);
      Serial.println(1);
      break;
      case '2' :
      copyArray(selectedled, leds2);
      break;
      case '3' :
      copyArray(selectedled, leds3);
      break;
      case '4' :
      copyArray(selectedled, leds4);
      break;
      case '5' :
      copyArray(selectedled, leds5);
      break;
      case '6' :
      copyArray(selectedled, leds6);
      break;
      case '7' :
      copyArray(selectedled, leds7);
      break;
      case '8' :
      copyArray(selectedled, leds8);
      break;
      case '9' :
      copyArray(selectedled, leds9);
      break;
      // default:
      // Serial.println("verify");
      // break;
    }

  FastLED.show();
}
