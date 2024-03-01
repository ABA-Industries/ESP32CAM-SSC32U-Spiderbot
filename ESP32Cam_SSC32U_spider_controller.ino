//created by ATLIN ANDERSON
//BOARD: AI THINKER ESP32-CAM
//ESP32 CAM board used

#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
#include <WiFiUdp.h>

/////////////////////////////////////////////////////////////
//SPIDERBOT Global Variables/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

//    //spider leg assignment diagram
//    //
//    //    Front
//    //
//    //leg 2     leg 1
//    //     \   /
//    // leg4- 0 -leg 3
//    //     /  \
//    // leg 6   leg 5
//
//    //group1: 1, 4, 5
//    //group2: 2, 3, 6

int max_servo_angle = 85;

//leg1 servo binding
int servo_coxa_1 = 8;
int servo_femur_1 = 9;
int servo_tibia_1 = 10;
//leg2 servo binding
int servo_coxa_2 = 24;
int servo_femur_2 = 25;
int servo_tibia_2 = 26;
//leg3 servo binding
int servo_coxa_3 = 4;
int servo_femur_3 = 5;
int servo_tibia_3 = 6;
//leg4 servo binding
int servo_coxa_4 = 20;
int servo_femur_4 = 21;
int servo_tibia_4 = 22;
//leg5 servo binding
int servo_coxa_5 = 0;
int servo_femur_5 = 1;
int servo_tibia_5 = 2;
//leg6 servo binding
int servo_coxa_6 = 16;
int servo_femur_6 = 17;
int servo_tibia_6 = 18;

//spider control variables
//default speed of 400ms
int servo_speed = 400;
int servo_speed_old = 400;

//spider control variables
//height from 0-9
int spider_height = 5;
int spider_height_old = 5;

//default standing angles
int default_standing_coxa_angle = 0;
int default_standing_femur_angle = 0;
int default_standing_tibia_angle = 90;

//used for timing of movement command
unsigned long milli_time_reference = 0;

//stores all commands to be sent to servo controller
String servo_serial_command_buffer = "";
String servo_serial_command_buffer_old = "";

//adds buffer time between spiderbot action groups
int added_time_to_movement_cycle_min = 200;

//used for tracking if calibration is enabled
bool calibration_tracker = false;

/////////////////////////////////////////////////////////////
//Network setup global variables/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

//Wifi Access Point credentials
//Set desired SSID name and password
const char* WIFI_SSID = "ESP32 Spiderbot";
const char* WIFI_PASS = "0123456789";

// //setting up recieval of UDP Packets
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[2];            // buffer for incoming packets
char old_Packet[2];                // buffer for old packets
//the first char is the coded action & the section char is either speed or height, if applicable

//initate webserver
WebServer server(80);

//initate a task (to run the camera stream wbeserver on the 2nd core of the ESP32Cam)
TaskHandle_t Task1;

///////////////////////////////////////////////////////////////
////ESP32cam Variables/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto hiRes = esp32cam::Resolution::find(800, 600);


void setup() {
  //enable the GPIO port for LED headlight
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  Serial.begin(9600);

  //configure ESP32CAM
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(loRes);
    cfg.setBufferCount(2);
    //sewt resultion from 0-100 , 100 being best
    cfg.setJpeg(50);

    bool ok = Camera.begin(cfg);
    //Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  //set up wifi access point
  WiFi.softAP(WIFI_SSID, WIFI_PASS);  // Start the access point

  //enable the ESP32Cam camera server
  server.on("/cam.bmp", handleBmp);
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam.jpg", handleJpg);
  server.on("/cam.mjpeg", handleMjpeg);
  server.begin();

  //set up UDP packets
  Udp.begin(localUdpPort);

  //create a task that to operate ESP32cam on core 1
  xTaskCreatePinnedToCore(
    Task1code, /* Task function. */
    "Task1",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task1,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */

  ///SPIDERBOT SETUP////////////////////////////////////////////////////////////////////////

  //set time reference
  milli_time_reference = millis();

  //lay flat on ground
  leg_group_1_command(0, 0, 0);
  leg_group_2_command(0, 0, 0);
  send_serial_command();

  delay(2000);

  //default standing position
  leg_group_1_command(default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);
  leg_group_2_command(default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);
  send_serial_command();

  delay(1000);
}

void loop() {
  //check for and read packet
  if (read_UDP_packet() == true) {
    //functions only preformed when packet is recieved
    toggle_LED_headlight();
    toogle_calibration_mode();
    update_body_height();
    update_servo_speed();
  }

  //read & decode the packet & preform action

  //stop moving and move to default position
  if (incomingPacket[0] == 'S') {
    brake();
  }

  //walk forward
  if (incomingPacket[0] == 'W') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    walk_forward(milli_time_reference, servo_speed);
  }

  //walk backward
  if (incomingPacket[0] == 'X') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    walk_backward(milli_time_reference, servo_speed);
  }

  //strafe left
  if (incomingPacket[0] == 'A') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    strafe_left(milli_time_reference, servo_speed);
  }

  //strafe right
  if (incomingPacket[0] == 'D') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    strafe_right(milli_time_reference, servo_speed);
  }

  //rotate left
  if (incomingPacket[0] == 'Q') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    rotate_left(milli_time_reference, servo_speed);
  }

  //rotate right
  if (incomingPacket[0] == 'E') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    rotate_right(milli_time_reference, servo_speed);
  }

  //wave front arm
  if (incomingPacket[0] == 'H') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    greeting(milli_time_reference, servo_speed);
  }

  //dance
  if (incomingPacket[0] == 'V') {
    movement_state_transition(old_Packet[0], incomingPacket[0]);
    dance(milli_time_reference, servo_speed);
  }

  send_serial_command();

  old_Packet[0] = incomingPacket[0];

  delay(10);
}
