#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include "esp_camera.h"

// WiFi credentials
const char* ssid = "RC_Car";
const char* password = "12345678";
const char* hostname = "rccar";

// Web server
WebServer server(80);

// Camera configuration for OV7670
#define PWDN_GPIO_NUM     19
#define RESET_GPIO_NUM    18
#define XCLK_GPIO_NUM      9
#define SIOD_GPIO_NUM      6
#define SIOC_GPIO_NUM      7
#define Y9_GPIO_NUM       10
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       13
#define Y5_GPIO_NUM       14
#define Y4_GPIO_NUM       15
#define Y3_GPIO_NUM       16
#define Y2_GPIO_NUM       17
#define VSYNC_GPIO_NUM     4
#define HREF_GPIO_NUM      5
#define PCLK_GPIO_NUM      8

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 44, 43); // RX, TX for Arduino communication
  
  // Initialize camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  // Configure WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  
  // Start mDNS
  if (MDNS.begin(hostname)) {
    Serial.println("MDNS responder started");
  }
  
  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/control", HTTP_POST, handleControl);
  server.on("/stream", HTTP_GET, handleStream);
  server.on("/capture", HTTP_GET, handleCapture);
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  server.handleClient();
}

void handleControl() {
  if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");
    Serial2.write(cmd[0]);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleCapture() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }
  
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.sendHeader("Content-Length", String(fb->len));
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  
  esp_camera_fb_return(fb);
}

void handleStream() {
  WiFiClient client = server.client();
  
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  client.print(response);
  
  while (client.connected()) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      break;
    }
    
    client.print("--frame\r\n");
    client.print("Content-Type: image/jpeg\r\n\r\n");
    client.write(fb->buf, fb->len);
    client.print("\r\n");
    
    esp_camera_fb_return(fb);
  }
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>RC Car Control Center</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background: #f0f0f0;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        }
        .video-container {
            width: 100%;
            max-width: 640px;
            margin: 20px auto;
            background: #000;
            border-radius: 5px;
            overflow: hidden;
        }
        #videoFeed {
            width: 100%;
            height: auto;
        }
        .control-panel {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 10px;
            max-width: 300px;
            margin: 20px auto;
        }
        .btn {
            padding: 15px;
            border: none;
            border-radius: 5px;
            background: #007bff;
            color: white;
            cursor: pointer;
            font-size: 16px;
            transition: background 0.3s;
        }
        .btn:hover {
            background: #0056b3;
        }
        .btn:active {
            background: #004085;
        }
        .gripper-controls {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 10px;
            max-width: 200px;
            margin: 20px auto;
        }
        .status {
            text-align: center;
            margin: 10px 0;
            color: #666;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1 style="text-align: center;">RC Car Control Center</h1>
        
        <div class="video-container">
            <img id="videoFeed" src="/stream" alt="Camera Feed">
        </div>
        
        <div class="status" id="status">Connected</div>
        
        <div class="control-panel">
            <button class="btn" onclick="sendCommand('L')">&larr;</button>
            <button class="btn" onclick="sendCommand('F')">&uarr;</button>
            <button class="btn" onclick="sendCommand('R')">&rarr;</button>
            <button class="btn" onclick="sendCommand('S')">Stop</button>
            <button class="btn" onclick="sendCommand('B')">&darr;</button>
            <button class="btn" onclick="sendCommand('S')">Stop</button>
        </div>
        
        <h2 style="text-align: center;">Gripper Controls</h2>
        <div class="gripper-controls">
            <button class="btn" onclick="sendCommand('U')">Base Up</button>
            <button class="btn" onclick="sendCommand('D')">Base Down</button>
            <button class="btn" onclick="sendCommand('G')">Grip</button>
            <button class="btn" onclick="sendCommand('O')">Release</button>
            <button class="btn" onclick="sendCommand('N')" style="grid-column: span 2;">Neutral</button>
        </div>
        
        <div style="text-align: center; margin-top: 20px;">
            <button class="btn" onclick="capturePhoto()">Take Photo</button>
        </div>
    </div>

    <script>
        function sendCommand(cmd) {
            fetch('/control', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: 'cmd=' + cmd
            })
            .catch(error => {
                console.error('Error:', error);
                document.getElementById('status').textContent = 'Connection Lost';
                document.getElementById('status').style.color = '#ff0000';
            });
        }
        
        function capturePhoto() {
            window.open('/capture', '_blank');
        }
        
        // Keyboard controls
        document.addEventListener('keydown', function(event) {
            switch(event.key) {
                case 'ArrowUp': sendCommand('F'); break;
                case 'ArrowDown': sendCommand('B'); break;
                case 'ArrowLeft': sendCommand('L'); break;
                case 'ArrowRight': sendCommand('R'); break;
                case ' ': sendCommand('S'); break;
                case 'g': sendCommand('G'); break;
                case 'o': sendCommand('O'); break;
                case 'u': sendCommand('U'); break;
                case 'd': sendCommand('D'); break;
                case 'n': sendCommand('N'); break;
            }
        });
        
        // Handle video stream errors
        document.getElementById('videoFeed').onerror = function() {
            this.src = '/stream?' + new Date().getTime();
            document.getElementById('status').textContent = 'Reconnecting...';
            document.getElementById('status').style.color = '#ff9900';
        };
        
        // Update connection status
        setInterval(() => {
            fetch('/control', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: 'cmd=P'  // Ping command
            })
            .then(() => {
                document.getElementById('status').textContent = 'Connected';
                document.getElementById('status').style.color = '#008000';
            })
            .catch(() => {
                document.getElementById('status').textContent = 'Connection Lost';
                document.getElementById('status').style.color = '#ff0000';
            });
        }, 5000);
    </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}