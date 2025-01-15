#include "camera.h"

void Camera::init() {
        // Turn-off the 'brownout detector'
        WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

        // OV2640 camera module configuration
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
        config.pin_sccb_sda = SIOD_GPIO_NUM;
        config.pin_sccb_scl = SIOC_GPIO_NUM;
        config.pin_pwdn = PWDN_GPIO_NUM;
        config.pin_reset = RESET_GPIO_NUM;
        config.xclk_freq_hz = 20000000; // 20 MHz
        config.pixel_format = PIXFORMAT_JPEG;

    // Select lower framesize if the camera doesn't support PSRAM
    if (psramFound()) {
        config.frame_size = FRAMESIZE_VGA; // Smaller frame size
        config.jpeg_quality = 10; //10-63 lower number means higher quality
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_VGA; // Smaller frame size
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // Initialize the Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
    else{
        Serial.println("Camera initialized successfully!");
    }

    // Camera quality adjustments
    sensor_t * s = esp_camera_sensor_get();

    // BRIGHTNESS (-2 to 2)
    s->set_brightness(s, 0);
    // CONTRAST (-2 to 2)
    s->set_contrast(s, 0);
    // SATURATION (-2 to 2)
    s->set_saturation(s, 0);
    // SPECIAL EFFECTS (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    s->set_special_effect(s, 0);
    // WHITE BALANCE (0 = Disable , 1 = Enable)
    s->set_whitebal(s, 1);
    // AWB GAIN (0 = Disable , 1 = Enable)
    s->set_awb_gain(s, 1);
    // WB MODES (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_wb_mode(s, 0);
    // EXPOSURE CONTROLS (0 = Disable , 1 = Enable)
    s->set_exposure_ctrl(s, 1);
    // AEC2 (0 = Disable , 1 = Enable)
    s->set_aec2(s, 0);
    // AE LEVELS (-2 to 2)
    s->set_ae_level(s, 0);
    // AEC VALUES (0 to 1200)
    s->set_aec_value(s, 300);
    // GAIN CONTROLS (0 = Disable , 1 = Enable)
    s->set_gain_ctrl(s, 1);
    // AGC GAIN (0 to 30)
    s->set_agc_gain(s, 0);
    // GAIN CEILING (0 to 6)
    s->set_gainceiling(s, (gainceiling_t)0);
    // BPC (0 = Disable , 1 = Enable)
    s->set_bpc(s, 0);
    // WPC (0 = Disable , 1 = Enable)
    s->set_wpc(s, 1);
    // RAW GMA (0 = Disable , 1 = Enable)
    s->set_raw_gma(s, 1);
    // LENC (0 = Disable , 1 = Enable)
    s->set_lenc(s, 1);
    // HORIZ MIRROR (0 = Disable , 1 = Enable)
    s->set_hmirror(s, 0);
    // VERT FLIP (0 = Disable , 1 = Enable)
    s->set_vflip(s, 0);
    // DCW (0 = Disable , 1 = Enable)
    s->set_dcw(s, 1);
    // COLOR BAR PATTERN (0 = Disable , 1 = Enable)
    s->set_colorbar(s, 0);  
}

void Camera::sdInit(){   //initialize sd card
  if(!SD_MMC.begin("/sd_card", true)){
    Serial.println("MicroSD card mount failed");
    return;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No MicroSD Card found");
    return;
  }
  Serial.println("MicroSD card mounted successfully!");
}

void Camera::capture(){
    camera_fb_t *fb = esp_camera_fb_get();
    if(!fb){
      Serial.println("Camera capture failed");
      return;
    }
    String path = "/picture" + String(millis()) + ".jpg";  //create jpg file path 
    File file = SD_MMC.open(path, FILE_WRITE);

    if (!file) {   //error opening file 
        Serial.println("Failed to open file on SD card");
        esp_camera_fb_return(fb);
        return;
    }

    file.write(fb->buf, fb->len); // Write photo to SD card
    file.close();
    esp_camera_fb_return(fb);

    Serial.println("Photo captured and saved: " + path);
}