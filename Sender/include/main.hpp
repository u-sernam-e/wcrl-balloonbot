// #include <ArduinoJson.h>

// Access Point Credentials
/*
const char* AP_SSID = "Meatloaf";
const char* AP_PASS = "password";

// Pins
#define STAT 2
#define PWMA 21
#define PWMB 3
#define AIN1 6
#define AIN2 7
#define BIN1 5
#define BIN2 4
#define ESC 10
*/
/*
// HTML + JavaScript UI embedded in Flash Memory
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32 Motor Control</title>
</head>
<body>
    <h2>ESP32 Motor Control Interface</h2>
    <button onclick="sendCommand(1)">LED on</button>
    <button onclick="sendCommand(0)">LED off</button>
    <script>
        // Connect to WebSocket server running on Port 81 of the gateway IP
        var gateway = `ws://${window.location.hostname}:81/`;
        var websocket;

        function initWebSocket() {
            websocket = new WebSocket(gateway);
            websocket.onopen = function(evt) { console.log("WebSocket Connected"); };
            websocket.onclose = function(evt) { setTimeout(initWebSocket, 2000); };
        }

        function sendCommand(state) {
            if (websocket.readyState === WebSocket.OPEN) {
                websocket.send(JSON.stringify({ LED: state }));
            }
        }

        document.addEventListener('keydown', (event) => {
            if (event.repeat) return;
            if (event.key === 'w') {
                sendCommand(1);
            }
        });

        document.addEventListener('keyup', (event) => {
            if (event.key === 'w') {
                sendCommand(0);
            }
        });

        window.onload = initWebSocket;
    </script>
</body>
</html>
)rawliteral";
*/