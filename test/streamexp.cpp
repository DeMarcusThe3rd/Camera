//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 01 ESP32 Cam Test Servo
/*
 * Reference :
 * - ESP32Servo (Author: Kevin Harrington,John K. Bennett) : - https://www.arduino.cc/reference/en/libraries/esp32servo/
 *                                                           - https://github.com/madhephaestus/ESP32Servo
 * - Timer on ESP32 : https://openlabpro.com/guide/timer-on-esp32/
 * 
 * and from various other sources.
 */

/* ======================================== Including the libraries. */
#include <ESP32Servo.h>
/* ======================================== */

/* ======================================== SERVOs GPIO */
#define servo_LeftRight_Pin   14
#define servo_UpDown_Pin      15
/* ======================================== */

/* ======================================== Variables declaration */
// Variable for servo position
int servo_LeftRight_Pos = 75;
int servo_UpDown_Pos = 75;
/* ======================================== */

/* ======================================== Initialize servos */
/*
 * Maybe in this section you are a little confused, why did I initialize 4 servos, even though in this project only 2 servos are used. Below is a brief explanation.
 * 
 * The ESP32 has 4 hardware timers (please correct me if I'm wrong). If only initialize 2 servos, then automatically based on servo library, 
 * then servo_1 will use timer_0 (channel_0) and servo_2 will use timer_1 (channel_1). 
 * That would be a problem because timer_0 (channel_0) and timer_1 (channel_1) are already in use by the camera.
 * 
 * Therefore, initialization of 4 servos is used, so that the 2 servos that are actually used will shift to timer_2 (channel_2) and timer_3 (channel_3). Below is the simple flow:
 * 
 * servo_Dummy_1    --> timer_0 or channel_0
 * servo_Dummy_2    --> timer_1 or channel_1
 * servo_LeftRight  --> timer_2 or channel_2
 * servo_UpDown     --> timer_3 or channel_3
 * 
 * Based on the flow above, the initialization of servo_Dummy_1 and servo_Dummy_2 is only used to shift the timer, 
 * so the 2 servos used in this project will use timer_2 (channel_2) and timer_3 (channel_3). 
 * servo_Dummy_1 and servo_Dummy_2 are not actually used, because timer_0 (channel_0) and timer_1 (channel_1) are already used by the camera.
 */
Servo servo_Dummy_1;
Servo servo_Dummy_2;
Servo servo_LeftRight;
Servo servo_UpDown;
/* ======================================== */

/* ________________________________________________________________________________ VOID SETUP() */
void setup() {
  // put your setup code here, to run once:

  /* ---------------------------------------- Init serial communication speed (baud rate). */
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();
  /* ---------------------------------------- */

  /* ---------------------------------------- Setting Servos */
  Serial.println();
  Serial.println("------------");
  Serial.println("Start setting Servos...");
  servo_LeftRight.setPeriodHertz(50); //--> // Standard 50hz servo
  servo_UpDown.setPeriodHertz(50);    //--> // Standard 50hz servo
  servo_Dummy_1.attach(12, 1000, 2000);
  servo_Dummy_2.attach(13, 1000, 2000);
  servo_LeftRight.attach(servo_LeftRight_Pin, 1000, 2000);
  servo_UpDown.attach(servo_UpDown_Pin, 1000, 2000);
  
  servo_LeftRight.write(servo_LeftRight_Pos);
  servo_UpDown.write(servo_UpDown_Pos);
  Serial.println("Setting up the servos was successful.");
  Serial.println("------------");
  Serial.println();
  /* ---------------------------------------- */
  
  delay(3000);
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ VOID LOOP() */
void loop() {
  // put your main code here, to run repeatedly:
  
  for (int i = 75; i <= 175; i+=5) {
    servo_LeftRight.write(i);
    Serial.print("SLR : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 180; i >= 5; i-=5) {
    servo_LeftRight.write(i);
    Serial.print("SLR : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 0; i <= 75; i+=5) {
    servo_LeftRight.write(i);
    Serial.print("SLR : ");
    Serial.println(i);
    delay(100);
  }

  delay(2000);

  for (int i = 75; i <= 175; i+=5) {
    servo_UpDown.write(i);
    Serial.print("SUD : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 180; i >= 5; i-=5) {
    servo_UpDown.write(i);
    Serial.print("SUD : ");
    Serial.println(i);
    delay(100);
  }

  for (int i = 0; i <= 75; i+=5) {
    servo_UpDown.write(i);
    Serial.print("SUD : ");
    Serial.println(i);
    delay(100);
  }

  delay(2000);
}
/* ________________________________________________________________________________ */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 02 ESP32 CAM Pan and Tilt Control SM (Station Mode)
/*
 * Reference :
 * - ESP32-CAM Pan and Tilt Video Streaming Web Server (2 Axis) : https://randomnerdtutorials.com/esp32-cam-pan-and-tilt-2-axis/
 * - ESP32Servo (Author: Kevin Harrington,John K. Bennett) : - https://www.arduino.cc/reference/en/libraries/esp32servo/
 *                                                           - https://github.com/madhephaestus/ESP32Servo
 * - Timer on ESP32 : https://openlabpro.com/guide/timer-on-esp32/
 * - HTML DOM TouchEvent : https://www.w3schools.com/jsref/obj_touchevent.asp
 * - HTML DOM MouseEvent : https://www.w3schools.com/jsref/obj_mouseevent.asp
 * - Add Space Between Buttons in HTML : https://www.delftstack.com/howto/html/html-space-between-buttons/
 * - CSS Buttons : https://www.w3schools.com/css/css3_buttons.asp
 * - How TO - Range Sliders : https://www.w3schools.com/howto/howto_js_rangeslider.asp
 * 
 * and from various other sources.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Important Information.                                                                                                                                                           //
// ----------------------                                                                                                                                                           //
// Since this project uses 2 servos, sufficient power supply is needed to drive 2 servos.                                                                                           //
// If the power supply is not enough, some errors will occur, such as ESP32 Cam restarts continuously, camera fails to capture or video stream cannot be loaded,                    //
// ESP32 Cam restarts when servo moves, video stream results streaky and others.                                                                                                    //
//                                                                                                                                                                                  //
// I have tested this project using a 5V 2A power supply and the result the project runs fine.                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I realized that the HTML code in this project could have been better or simpler. But because of my limited knowledge on WEB programming,                                         //
// then that code is the best code I can make. If you can make it better or simpler, I hope you can share it with me or with the video viewers of this project.                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* ======================================== Including the libraries. */
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include <ESP32Servo.h>
/* ======================================== */

/* ======================================== Select camera model */
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
#define CAMERA_MODEL_AI_THINKER
/* ======================================== */

/* ======================================== GPIO of camera models */
#if defined(CAMERA_MODEL_WROVER_KIT)
  #define PWDN_GPIO_NUM    -1
  #define RESET_GPIO_NUM   -1
  #define XCLK_GPIO_NUM    21
  #define SIOD_GPIO_NUM    26
  #define SIOC_GPIO_NUM    27
  
  #define Y9_GPIO_NUM      35
  #define Y8_GPIO_NUM      34
  #define Y7_GPIO_NUM      39
  #define Y6_GPIO_NUM      36
  #define Y5_GPIO_NUM      19
  #define Y4_GPIO_NUM      18
  #define Y3_GPIO_NUM       5
  #define Y2_GPIO_NUM       4
  #define VSYNC_GPIO_NUM   25
  #define HREF_GPIO_NUM    23
  #define PCLK_GPIO_NUM    22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     25
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    22
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_PSRAM_B)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     22
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_WITHOUT_PSRAM)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     25
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       17
  #define VSYNC_GPIO_NUM    22
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22
#else
  #error "Camera model not selected"
#endif
/* ======================================== */

// LEDs GPIO
#define LED_OnBoard 4

// SERVOs GPIO
#define servo_LeftRight_Pin   14
#define servo_UpDown_Pin      15

/* ======================================== Variables declaration */
// setting PWM properties
const int PWM_freq = 5000;
const int ledChannel = 5;
const int PWM_resolution = 8;

// Variable for servo position
int servo_LeftRight_Pos = 75;
int servo_UpDown_Pos = 75;
/* ======================================== */

/* ======================================== Initialize servos */
/*
 * Maybe in this section you are a little confused, why did I initialize 4 servos, even though in this project only 2 servos are used. Below is a brief explanation.
 * 
 * The ESP32 has 4 hardware timers (please correct me if I'm wrong). If only initialize 2 servos, then automatically based on servo library, 
 * then servo_1 will use timer_0 (channel_0) and servo_2 will use timer_1 (channel_1). 
 * That would be a problem because timer_0 (channel_0) and timer_1 (channel_1) are already in use by the camera.
 * 
 * Therefore, initialization of 4 servos is used, so that the 2 servos that are actually used will shift to timer_2 (channel_2) and timer_3 (channel_3). Below is the simple flow:
 * 
 * servo_Dummy_1    --> timer_0 or channel_0
 * servo_Dummy_2    --> timer_1 or channel_1
 * servo_LeftRight  --> timer_2 or channel_2
 * servo_UpDown     --> timer_3 or channel_3
 * 
 * Based on the flow above, the initialization of servo_Dummy_1 and servo_Dummy_2 is only used to shift the timer, 
 * so the 2 servos used in this project will use timer_2 (channel_2) and timer_3 (channel_3). 
 * servo_Dummy_1 and servo_Dummy_2 are not actually used, because timer_0 (channel_0) and timer_1 (channel_1) are already used by the camera.
 */
Servo servo_Dummy_1;
Servo servo_Dummy_2;
Servo servo_LeftRight;
Servo servo_UpDown;
/* ======================================== */

/* ======================================== Replace with your network credentials */
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
/* ======================================== */

/* ======================================== */
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
/* ======================================== */

/* ======================================== Empty handle to esp_http_server */
httpd_handle_t index_httpd = NULL;
httpd_handle_t stream_httpd = NULL;
/* ======================================== */

/* ======================================== HTML code for index / main page */
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>

  <head>
  
    <title>ESP32-CAM Pan and Tilt Control</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
    
    <style>
    
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 10px;}

      /* ----------------------------------- Slider */
      .slidecontainer {
        width: 100%;
      }

      .slider {
        -webkit-appearance: none;
        width: 50%;
        height: 10px;
        border-radius: 5px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }

      .slider:hover {
        opacity: 1;
      }

      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #04AA6D;
        cursor: pointer;
      }

      .slider::-moz-range-thumb {
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #04AA6D;
        cursor: pointer;
      }
      /* ----------------------------------- */
      
      /* ----------------------------------- Button */
      .button {
        display: inline-block;
        padding: 10px;
        font-size: 12px;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #4CAF50;
        border: none;
        border-radius: 15px;
        box-shadow: 0 6px #999;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        width: 15%
      }

      .button:hover {background-color: #3e8e41}

      .button:active {
        background-color: #3e8e41;
        box-shadow: 0 2px #666;
        transform: translateY(4px);
      }
      /* ----------------------------------- */
      
      /* ----------------------------------- */
      .space {
        width: 15%;
        height: auto;
        display: inline-block;
      }
      /* ----------------------------------- */

      /* ----------------------------------- Stream Viewer */
      img {
        width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
      /* ----------------------------------- */
      
    </style>
    
  </head>
  
  <body>
  
    <h3>ESP32-CAM Pan and Tilt Control</h3>
    
    <img src="" id="vdstream">
    
    <br><br>
    
    <div class="slidecontainer">
      <span style="font-size:15;">LED Flash &nbsp;</span>
      <input type="range" min="0" max="20" value="0" class="slider" id="mySlider">
    </div>

    <br>

    <div>
      <!-- "T" suffix in "start_timer_TC" (example : start_timer_TC('UT')) and -->
      <!-- "M" suffix in "start_timer_MP" (example : start_timer_MP('UM')) to differentiate between touch screen input and mouse pointer input on button. -->
      <button class="button" ontouchstart="start_timer_TC('UT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('UM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>UP</b></button>
      <br><br>
      <button class="button" ontouchstart="start_timer_TC('LT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('LM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>LEFT</b></button>
      <div class="space"></div>
      <button class="button" ontouchstart="start_timer_TC('RT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('RM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>RIGHT</b></button>
      <br><br>
      <button class="button" ontouchstart="start_timer_TC('DT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('DM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>DOWN</b></button>
    </div>
    
    <script>
      /* ----------------------------------- Calls the video stream link and displays it. */ 
      window.onload = document.getElementById("vdstream").src = window.location.href.slice(0, -1) + ":81/stream";
      /* ----------------------------------- */
      
      /* ----------------------------------- */
      var slider = document.getElementById("mySlider");
      /* ----------------------------------- */
      
      /* ----------------------------------- Variable declarations for timers and their intervals. */
      var myTmr;
      var myTmr_Interval = 250;
      /* ----------------------------------- */

      // Variable declarations to differentiate the sending of button commands when the button is held down or just a single press.
      var onmousedown_stat = 0;

      /* ----------------------------------- Variable declaration for sending commands to ESP32 Cam from index/web page. */
      let btn_commands = "";
      let btn_commands_rslt = "";
      /* ----------------------------------- */

      // Variable declarations to distinguish between button presses using the mouse or using the touch screen (on a mobile browser or in a browser on a touch screen device).
      var join_TM = 0;

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Update the current slider value (each time you drag the slider handle) */
      slider.oninput = function() {
        let slider_pwm_val = "S," + slider.value;
        send_cmd(slider_pwm_val);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Start and stop the timer */
      function start_timer_MP(cmds) {
        btn_commands = cmds;
        btn_commands_rslt = cmds.substring(0, 1);
        if (join_TM == 0) {
          myTmr = setInterval(myTimer, myTmr_Interval);
          if (onmousedown_stat == 0) send_btn_cmd(btn_commands_rslt);
          onmousedown_stat = 1;
        }
      }

      function start_timer_TC(cmds) {
        join_TM = 1;
        btn_commands_rslt = cmds.substring(0, 1);
        myTmr = setInterval(myTimer, myTmr_Interval);
        btn_commands = cmds;
        if (onmousedown_stat == 0) send_btn_cmd(btn_commands_rslt);
        onmousedown_stat = 1;
      }
      
      function stop_timer() {
        clearInterval(myTmr);
        onmousedown_stat = 0;
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Timer to send data or commands from index page/web server to ESP32 Cam. */
      function myTimer() {
        send_btn_cmd(btn_commands_rslt);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to send commands to the ESP32 Cam whenever the button is clicked. */
      function send_btn_cmd(cmds) {
        let btn_cmd = "B," + cmds;
        send_cmd(btn_cmd);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */
      
      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function for sending commands */
      function send_cmd(cmds) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/action?go=" + cmds, true);
        xhr.send();
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */
    </script>
  
  </body>
  
</html>
)rawliteral";
/* ======================================== */

/* ________________________________________________________________________________ Index handler function to be called during GET or uri request */
static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ stream handler function to be called during GET or uri request. */
static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  /* ---------------------------------------- Loop to show streaming video from ESP32 Cam camera. */
  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed (stream_handler)");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
  }
  /* ---------------------------------------- */
  return res;
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ cmd handler function to be called during GET or uri request. */
static esp_err_t cmd_handler(httpd_req_t *req){
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
   
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
 
  int res = 0;

  Serial.print("Incoming command : ");
  Serial.println(variable);
  Serial.println();
  String getData = String(variable);
  String resultData = getValue(getData, ',', 0);

  /* ---------------------------------------- Controlling the LEDs on the ESP32 Cam board with PWM. */
  // Example :
  // Incoming command = S,10
  // S = Slider
  // 10 = slider value
  // I set the slider value range from 0 to 20.
  // Then the slider value is changed from 0 - 20 or vice versa to 0 - 255 or vice versa.
  if (resultData == "S") {
    resultData = getValue(getData, ',', 1);
    int pwm = map(resultData.toInt(), 0, 20, 0, 255);
    ledcSetup(ledChannel, PWM_freq, PWM_resolution);
    ledcAttachPin(LED_OnBoard, ledChannel);
    ledcWrite(ledChannel, pwm);
    Serial.print("PWM LED On Board : ");
    Serial.println(pwm);
  }
  /* ---------------------------------------- */

  /* ---------------------------------------- Processes and executes commands received from the index page/web server. */
  // Example :
  // Incoming Command = B,U
  // B = Button
  // U = Command to drive the servo.
  
  if (resultData == "B") {
    resultData = getValue(getData, ',', 1);
    
    if (resultData == "U") {
      if (servo_UpDown_Pos >= 5) {
        servo_UpDown_Pos -= 5;
        servo_UpDown.write(servo_UpDown_Pos);
      }
    }

    if (resultData == "D") {
      if (servo_UpDown_Pos <= 175) {
        servo_UpDown_Pos += 5;
        servo_UpDown.write(servo_UpDown_Pos);
      }
    }

    if (resultData == "R") {
      if (servo_LeftRight_Pos <= 175) {
        servo_LeftRight_Pos += 5;
        servo_LeftRight.write(servo_LeftRight_Pos);
      }
    }

    if (resultData == "L") {
      if (servo_LeftRight_Pos >= 5) {
        servo_LeftRight_Pos -= 5;
        servo_LeftRight.write(servo_LeftRight_Pos);
      }
    }
    
    Serial.print("Button : ");
    Serial.println(resultData);
    Serial.print("servo_LeftRight_Pos : ");
    Serial.println(servo_LeftRight_Pos);
    Serial.print("servo_UpDown_Pos : ");
    Serial.println(servo_UpDown_Pos);
    Serial.println();
  }
  /* ---------------------------------------- */
  
  if(res){
    return httpd_resp_send_500(req);
  }
 
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ Subroutine for starting the web server / startCameraServer. */
void startCameraWebServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/action",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
    
  if (httpd_start(&index_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(index_httpd, &index_uri);
      httpd_register_uri_handler(index_httpd, &cmd_uri);
  }

  config.server_port += 1;
  config.ctrl_port += 1;  
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(stream_httpd, &stream_uri);
  }

  Serial.println();
  Serial.println("Camera Server started successfully");
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  Serial.println();
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ String function to process the data command */
// I got this from : https://www.electroniclinic.com/reyax-lora-based-multiple-sensors-monitoring-using-arduino/
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ VOID SETUP() */
void setup() {
  // put your setup code here, to run once:

  // Disable brownout detector.
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  /* ---------------------------------------- Init serial communication speed (baud rate). */
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();
  /* ---------------------------------------- */

  pinMode(LED_OnBoard, OUTPUT);

  /* ---------------------------------------- Setting Servos */
  Serial.println();
  Serial.println("------------");
  Serial.println("Start setting Servos...");
  servo_LeftRight.setPeriodHertz(50); //--> // Standard 50hz servo
  servo_UpDown.setPeriodHertz(50);    //--> // Standard 50hz servo
  servo_Dummy_1.attach(12, 1000, 2000);
  servo_Dummy_2.attach(13, 1000, 2000);
  servo_LeftRight.attach(servo_LeftRight_Pin, 1000, 2000);
  servo_UpDown.attach(servo_UpDown_Pin, 1000, 2000);
  
  servo_LeftRight.write(servo_LeftRight_Pos);
  servo_UpDown.write(servo_UpDown_Pos);
  Serial.println("Setting up the servos was successful.");
  Serial.println("------------");
  Serial.println();
  /* ---------------------------------------- */
  
  delay(2000);

  /* ---------------------------------------- Camera configuration. */
  Serial.println();
  Serial.println("------------");
  Serial.println("Start configuring and initializing the camera...");
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

  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
  
  Serial.println("Configure and initialize the camera successfully.");
  Serial.println("------------");
  Serial.println();
  /* ---------------------------------------- */

  /* ---------------------------------------- Connect to Wi-Fi. */
  WiFi.mode(WIFI_STA);
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  /* ::::::::::::::::: The process of connecting ESP32 CAM with WiFi Hotspot / WiFi Router. */
  /*
   * The process timeout of connecting ESP32 CAM with WiFi Hotspot / WiFi Router is 20 seconds.
   * If within 20 seconds the ESP32 CAM has not been successfully connected to WiFi, the ESP32 CAM will restart.
   * I made this condition because on my ESP32-CAM, there are times when it seems like it can't connect to WiFi, so it needs to be restarted to be able to connect to WiFi.
   */
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_OnBoard, HIGH);
    delay(250);
    digitalWrite(LED_OnBoard, LOW);
    delay(250);
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  digitalWrite(LED_OnBoard, LOW);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("------------");
  Serial.println("");
  /* ::::::::::::::::: */
  /* ---------------------------------------- */

  // Start camera web server
  startCameraWebServer(); 
}
/* ________________________________________________________________________________ */

void loop() {
  // put your main code here, to run repeatedly:
  delay(1);
}
/* ________________________________________________________________________________ */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 03 Completely erase and reformat the NVS memory used by Preferences
/*
 * In the Arduino implementation of Preferences, there is no method of completely removing a namespace.
 * As a result, over the course of several projects, the ESP32 non-volatile storage (nvs) Preferences partition may become full.
 * So this code To completely erase and reformat the NVS memory used by Preferences. This code is only used once.
 * 
 * Description and code are from : https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
 */
 
#include <nvs_flash.h>

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Started deleting the NVS partition and initializing the NVS partition...");
  nvs_flash_erase();  // erase the NVS partition and...
  nvs_flash_init();   // initialize the NVS partition.
  Serial.println("Deleting the NVS partition and initializing the NVS partition is complete.");
}

void loop() {
  delay(1);
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 04 ESP32 CAM Pan and Tilt Control SM SSP (Station Mode) (Saving Servo Position)
/*
 * Reference :
 * - ESP32-CAM Pan and Tilt Video Streaming Web Server (2 Axis) : https://randomnerdtutorials.com/esp32-cam-pan-and-tilt-2-axis/
 * - ESP32 Save Data Permanently using Preferences Library : https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
 * - ESP32Servo (Author: Kevin Harrington,John K. Bennett) : - https://www.arduino.cc/reference/en/libraries/esp32servo/
 *                                                           - https://github.com/madhephaestus/ESP32Servo
 * - Timer on ESP32 : https://openlabpro.com/guide/timer-on-esp32/
 * - HTML DOM TouchEvent : https://www.w3schools.com/jsref/obj_touchevent.asp
 * - HTML DOM MouseEvent : https://www.w3schools.com/jsref/obj_mouseevent.asp
 * - Add Space Between Buttons in HTML : https://www.delftstack.com/howto/html/html-space-between-buttons/
 * - CSS Buttons : https://www.w3schools.com/css/css3_buttons.asp
 * - How TO - Range Sliders : https://www.w3schools.com/howto/howto_js_rangeslider.asp
 * 
 * and from various other sources.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Important Information.                                                                                                                                                           //
// ----------------------                                                                                                                                                           //
// Since this project uses 2 servos, sufficient power supply is needed to drive 2 servos.                                                                                           //
// If the power supply is not enough, some errors will occur, such as ESP32 Cam restarts continuously, camera fails to capture or video stream cannot be loaded,                    //
// ESP32 Cam restarts when servo moves, video stream results streaky and others.                                                                                                    //
//                                                                                                                                                                                  //
// I have tested this project using a 5V 2A power supply and the result the project runs fine.                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I realized that the HTML code in this project could have been better or simpler. But because of my limited knowledge on WEB programming,                                         //
// then that code is the best code I can make. If you can make it better or simpler, I hope you can share it with me or with the video viewers of this project.                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* ======================================== Including the libraries. */
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include <ESP32Servo.h>
#include <Preferences.h>
/* ======================================== */

/* ======================================== Select camera model */
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
#define CAMERA_MODEL_AI_THINKER
/* ======================================== */

/* ======================================== GPIO of camera models */
#if defined(CAMERA_MODEL_WROVER_KIT)
  #define PWDN_GPIO_NUM    -1
  #define RESET_GPIO_NUM   -1
  #define XCLK_GPIO_NUM    21
  #define SIOD_GPIO_NUM    26
  #define SIOC_GPIO_NUM    27
  
  #define Y9_GPIO_NUM      35
  #define Y8_GPIO_NUM      34
  #define Y7_GPIO_NUM      39
  #define Y6_GPIO_NUM      36
  #define Y5_GPIO_NUM      19
  #define Y4_GPIO_NUM      18
  #define Y3_GPIO_NUM       5
  #define Y2_GPIO_NUM       4
  #define VSYNC_GPIO_NUM   25
  #define HREF_GPIO_NUM    23
  #define PCLK_GPIO_NUM    22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     25
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    22
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_PSRAM_B)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     22
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_WITHOUT_PSRAM)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     25
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       17
  #define VSYNC_GPIO_NUM    22
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22
#else
  #error "Camera model not selected"
#endif
/* ======================================== */

/* ======================================== SERVOs GPIO */
#define servo_LeftRight_Pin   14
#define servo_UpDown_Pin      15
/* ======================================== */

// LEDs GPIO
#define LED_OnBoard 4

/* ======================================== Variables declaration */
// setting PWM properties
const int PWM_freq = 5000;
const int ledChannel = 5;
const int PWM_resolution = 8;

// Variable for servo position
int servo_LeftRight_Pos = 75;
int servo_UpDown_Pos = 75;

// Variables for millis()
unsigned long previousMillis = 0;
const long interval = 1000;

// Variables for triggering and calculating the servo-servo position storage delay time
int count_to_save = 0;
bool save_servo_pos = false;
/* ======================================== */

/* ======================================== Initialize servos */
/*
 * Maybe in this section you are a little confused, why did I initialize 4 servos, even though in this project only 2 servos are used. Below is a brief explanation.
 * 
 * The ESP32 has 4 hardware timers (please correct me if I'm wrong). If only initialize 2 servos, then automatically based on servo library, 
 * then servo_1 will use timer_0 (channel_0) and servo_2 will use timer_1 (channel_1). 
 * That would be a problem because timer_0 (channel_0) and timer_1 (channel_1) are already in use by the camera.
 * 
 * Therefore, initialization of 4 servos is used, so that the 2 servos that are actually used will shift to timer_2 (channel_2) and timer_3 (channel_3). Below is the simple flow:
 * 
 * servo_Dummy_1    --> timer_0 or channel_0
 * servo_Dummy_2    --> timer_1 or channel_1
 * servo_LeftRight  --> timer_2 or channel_2
 * servo_UpDown     --> timer_3 or channel_3
 * 
 * Based on the flow above, the initialization of servo_Dummy_1 and servo_Dummy_2 is only used to shift the timer, 
 * so the 2 servos used in this project will use timer_2 (channel_2) and timer_3 (channel_3). 
 * servo_Dummy_1 and servo_Dummy_2 are not actually used, because timer_0 (channel_0) and timer_1 (channel_1) are already used by the camera.
 */
Servo servo_Dummy_1;
Servo servo_Dummy_2;
Servo servo_LeftRight;
Servo servo_UpDown;
/* ======================================== */

// Initialize Preferences
Preferences preferences;

/* ======================================== Replace with your network credentials */
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
/* ======================================== */

/* ======================================== */
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
/* ======================================== */

/* ======================================== Empty handle to esp_http_server */
httpd_handle_t index_httpd = NULL;
httpd_handle_t stream_httpd = NULL;
/* ======================================== */

/* ======================================== HTML code for index / main page */
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>

  <head>
  
    <title>ESP32-CAM Pan and Tilt Control</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
    
    <style>
    
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 10px;}

      /* ----------------------------------- Slider */
      .slidecontainer {
        width: 100%;
      }

      .slider {
        -webkit-appearance: none;
        width: 50%;
        height: 10px;
        border-radius: 5px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }

      .slider:hover {
        opacity: 1;
      }

      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #04AA6D;
        cursor: pointer;
      }

      .slider::-moz-range-thumb {
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #04AA6D;
        cursor: pointer;
      }
      /* ----------------------------------- */
      
      /* ----------------------------------- Button */
      .button {
        display: inline-block;
        padding: 10px;
        font-size: 12px;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #4CAF50;
        border: none;
        border-radius: 15px;
        box-shadow: 0 6px #999;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        width: 15%
      }

      .button:hover {background-color: #3e8e41}

      .button:active {
        background-color: #3e8e41;
        box-shadow: 0 2px #666;
        transform: translateY(4px);
      }
      /* ----------------------------------- */
      
      /* ----------------------------------- */
      .space {
        width: 15%;
        height: auto;
        display: inline-block;
      }
      /* ----------------------------------- */

      /* ----------------------------------- Stream Viewer */
      img {
        width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
      /* ----------------------------------- */
      
    </style>
    
  </head>
  
  <body>
  
    <h3>ESP32-CAM Pan and Tilt Control</h3>
    
    <img src="" id="vdstream">
    
    <br><br>
    
    <div class="slidecontainer">
      <span style="font-size:15;">LED Flash &nbsp;</span>
      <input type="range" min="0" max="20" value="0" class="slider" id="mySlider">
    </div>

    <br>

    <div>
      <!-- "T" suffix in "start_timer_TC" (example : start_timer_TC('UT')) and -->
      <!-- "M" suffix in "start_timer_MP" (example : start_timer_MP('UM')) to differentiate between touch screen input and mouse pointer input on button. -->
      <button class="button" ontouchstart="start_timer_TC('UT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('UM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>UP</b></button>
      <br><br>
      <button class="button" ontouchstart="start_timer_TC('LT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('LM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>LEFT</b></button>
      <div class="space"></div>
      <button class="button" ontouchstart="start_timer_TC('RT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('RM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>RIGHT</b></button>
      <br><br>
      <button class="button" ontouchstart="start_timer_TC('DT')" ontouchend="stop_timer()" onmousedown="start_timer_MP('DM')" onmouseup="stop_timer()" onmouseleave="stop_timer()"><b>DOWN</b></button>
    </div>
    
    <script>
      /* ----------------------------------- Calls the video stream link and displays it. */ 
      window.onload = document.getElementById("vdstream").src = window.location.href.slice(0, -1) + ":81/stream";
      /* ----------------------------------- */
      
      /* ----------------------------------- */
      var slider = document.getElementById("mySlider");
      /* ----------------------------------- */
      
      /* ----------------------------------- Variable declarations for timers and their intervals. */
      var myTmr;
      var myTmr_Interval = 250;
      /* ----------------------------------- */

      // Variable declarations to differentiate the sending of button commands when the button is held down or just a single press.
      var onmousedown_stat = 0;

      /* ----------------------------------- Variable declaration for sending commands to ESP32 Cam from index/web page. */
      let btn_commands = "";
      let btn_commands_rslt = "";
      /* ----------------------------------- */

      // Variable declarations to distinguish between button presses using the mouse or using the touch screen (on a mobile browser or in a browser on a touch screen device).
      var join_TM = 0;

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Update the current slider value (each time you drag the slider handle) */
      slider.oninput = function() {
        let slider_pwm_val = "S," + slider.value;
        send_cmd(slider_pwm_val);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Start and stop the timer */
      function start_timer_MP(cmds) {
        btn_commands = cmds;
        btn_commands_rslt = cmds.substring(0, 1);
        if (join_TM == 0) {
          myTmr = setInterval(myTimer, myTmr_Interval);
          if (onmousedown_stat == 0) send_btn_cmd(btn_commands_rslt);
          onmousedown_stat = 1;
        }
      }

      function start_timer_TC(cmds) {
        join_TM = 1;
        btn_commands_rslt = cmds.substring(0, 1);
        myTmr = setInterval(myTimer, myTmr_Interval);
        btn_commands = cmds;
        if (onmousedown_stat == 0) send_btn_cmd(btn_commands_rslt);
        onmousedown_stat = 1;
      }
      
      function stop_timer() {
        clearInterval(myTmr);
        onmousedown_stat = 0;
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Timer to send data or commands from index page/web server to ESP32 Cam. */
      function myTimer() {
        send_btn_cmd(btn_commands_rslt);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to send commands to the ESP32 Cam whenever the button is clicked. */
      function send_btn_cmd(cmds) {
        let btn_cmd = "B," + cmds;
        send_cmd(btn_cmd);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */
      
      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function for sending commands */
      function send_cmd(cmds) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/action?go=" + cmds, true);
        xhr.send();
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */
    </script>
  
  </body>
  
</html>
)rawliteral";
/* ======================================== */

/* ________________________________________________________________________________ Index handler function to be called during GET or uri request */
static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ stream handler function to be called during GET or uri request. */
static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  /* ---------------------------------------- Loop to show streaming video from ESP32 Cam camera. */
  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed (stream_handler)");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
  }
  /* ---------------------------------------- */
  return res;
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ cmd handler function to be called during GET or uri request. */
static esp_err_t cmd_handler(httpd_req_t *req){
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
   
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
 
  int res = 0;

  Serial.print("Incoming command : ");
  Serial.println(variable);
  Serial.println();
  String getData = String(variable);
  String resultData = getValue(getData, ',', 0);

  /* ---------------------------------------- Controlling the LEDs on the ESP32 Cam board with PWM. */
  // Example :
  // Incoming command = S,10
  // S = Slider
  // 10 = slider value
  // I set the slider value range from 0 to 20.
  // Then the slider value is changed from 0 - 20 or vice versa to 0 - 255 or vice versa.
  if (resultData == "S") {
    resultData = getValue(getData, ',', 1);
    int pwm = map(resultData.toInt(), 0, 20, 0, 255);
    ledcSetup(ledChannel, PWM_freq, PWM_resolution);
    ledcAttachPin(LED_OnBoard, ledChannel);
    ledcWrite(ledChannel, pwm);
    Serial.print("PWM LED ON Board : ");
    Serial.println(pwm);
  }
  /* ---------------------------------------- */

  /* ---------------------------------------- Processes and executes commands received from the index page/web server. */
  // Example :
  // Incoming Command = B,U
  // B = Button
  // U = Command to drive the servo.
  
  if (resultData == "B") {
    /* :::::::::::::::::::: Reset trigger and save delay time every time the command from the button is entered. */
    save_servo_pos = true;
    count_to_save = 0;
    /* :::::::::::::::::::: */
    
    resultData = getValue(getData, ',', 1);
    
    if (resultData == "U") {
      if (servo_UpDown_Pos >= 5) {
        servo_UpDown_Pos -= 5;
        servo_UpDown.write(servo_UpDown_Pos);
      }
    }

    if (resultData == "D") {
      if (servo_UpDown_Pos <= 175) {
        servo_UpDown_Pos += 5;
        servo_UpDown.write(servo_UpDown_Pos);
      }
    }

    if (resultData == "R") {
      if (servo_LeftRight_Pos <= 175) {
        servo_LeftRight_Pos += 5;
        servo_LeftRight.write(servo_LeftRight_Pos);
      }
    }

    if (resultData == "L") {
      if (servo_LeftRight_Pos >= 5) {
        servo_LeftRight_Pos -= 5;
        servo_LeftRight.write(servo_LeftRight_Pos);
      }
    }
    
    Serial.print("Button : ");
    Serial.println(resultData);
    Serial.print("servo_LeftRight_Pos : ");
    Serial.println(servo_LeftRight_Pos);
    Serial.print("servo_UpDown_Pos : ");
    Serial.println(servo_UpDown_Pos);
    Serial.println();
  }
  /* ---------------------------------------- */
  
  if(res){
    return httpd_resp_send_500(req);
  }
 
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ Subroutine for starting the web server / startCameraServer. */
void startCameraWebServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/action",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
    
  if (httpd_start(&index_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(index_httpd, &index_uri);
      httpd_register_uri_handler(index_httpd, &cmd_uri);
  }

  config.server_port += 1;
  config.ctrl_port += 1;  
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(stream_httpd, &stream_uri);
  }

  Serial.println();
  Serial.println("Camera Server started successfully");
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  Serial.println();
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ String function to process the data command */
// I got this from : https://www.electroniclinic.com/reyax-lora-based-multiple-sensors-monitoring-using-arduino/
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ VOID SETUP() */
void setup() {
  // put your setup code here, to run once:

  // Disable brownout detector.
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  /* ---------------------------------------- Init serial communication speed (baud rate). */
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();
  /* ---------------------------------------- */

  /* ---------------------------------------- Loading stored servos position data */
  Serial.println();
  Serial.println("------------");
  Serial.println("Start loading stored servo position data...");
  preferences.begin("servo_pos", false);
  servo_LeftRight_Pos = preferences.getInt("SLR_pos", 75);
  servo_UpDown_Pos = preferences.getInt("SUD_pos", 75);
  Serial.print("SLR_pos : ");
  Serial.println(servo_LeftRight_Pos);
  Serial.print("SUD_pos : ");
  Serial.println(servo_UpDown_Pos);
  Serial.println("Loading of stored servo position data is complete.");
  Serial.println("------------");
  Serial.println();
  /* ---------------------------------------- */

  pinMode(LED_OnBoard, OUTPUT);

  /* ---------------------------------------- Setting Servos */
  Serial.println();
  Serial.println("------------");
  Serial.println("Start setting Servos...");
  servo_LeftRight.setPeriodHertz(50); //--> // Standard 50hz servo
  servo_UpDown.setPeriodHertz(50);    //--> // Standard 50hz servo
  servo_Dummy_1.attach(12, 1000, 2000);
  servo_Dummy_2.attach(13, 1000, 2000);
  servo_LeftRight.attach(servo_LeftRight_Pin, 1000, 2000);
  servo_UpDown.attach(servo_UpDown_Pin, 1000, 2000);
  
  servo_LeftRight.write(servo_LeftRight_Pos);
  servo_UpDown.write(servo_UpDown_Pos);
  Serial.println("Setting up the servos was successful.");
  Serial.println("------------");
  Serial.println();
  /* ---------------------------------------- */
  
  delay(2000);

  /* ---------------------------------------- Camera configuration. */
  Serial.println();
  Serial.println("------------");
  Serial.println("Start configuring and initializing the camera...");
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

  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
  
  Serial.println("Configure and initialize the camera successfully.");
  Serial.println("------------");
  Serial.println();
  /* ---------------------------------------- */

  /* ---------------------------------------- Connect to Wi-Fi. */
  WiFi.mode(WIFI_STA);
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  /* ::::::::::::::::: The process of connecting ESP32 CAM with WiFi Hotspot / WiFi Router. */
  /*
   * The process timeout of connecting ESP32 CAM with WiFi Hotspot / WiFi Router is 20 seconds.
   * If within 20 seconds the ESP32 CAM has not been successfully connected to WiFi, the ESP32 CAM will restart.
   * I made this condition because on my ESP32-CAM, there are times when it seems like it can't connect to WiFi, so it needs to be restarted to be able to connect to WiFi.
   */
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_OnBoard, HIGH);
    delay(250);
    digitalWrite(LED_OnBoard, LOW);
    delay(250);
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  digitalWrite(LED_OnBoard, LOW);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("------------");
  Serial.println("");
  /* ::::::::::::::::: */
  /* ---------------------------------------- */

  // Start camera web server
  startCameraWebServer(); 
}
/* ________________________________________________________________________________ */

void loop() {
  // put your main code here, to run repeatedly:
  
  /* ---------------------------------------- Millis to store servos position. */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    count_to_save++;
    if (count_to_save > 3) count_to_save = 3; //--> The servo position will be saved 3 seconds after the command from the index page button is entered.

    if (save_servo_pos == true && count_to_save == 3) {
      preferences.putInt("SLR_pos", servo_LeftRight_Pos);
      preferences.putInt("SUD_pos", servo_UpDown_Pos);
      Serial.println("");
      Serial.println("The servo position is stored.");
      Serial.println("");
      save_servo_pos = false;
    }
  }
  /* ---------------------------------------- */
}
/* ________________________________________________________________________________ */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 05 ESP32 CAM Pan and Tilt Control AM SSP (Access Point/SoftAP Mode)
/*  Additional code for Access Point Mode. */

/* ======================================== Access Point Declaration and Configuration */
const char* ssid = "ESP32-CAM";  //--> access point name
const char* password = "helloesp32cam"; //--> access point password

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
/* ======================================== */

Serial.println(local_ip);

/* ---------------------------------------- Wi-Fi connection */
WiFi.softAP(ssid, password); //--> Creating Access Points
WiFi.softAPConfig(local_ip, gateway, subnet);
/* ---------------------------------------- */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<