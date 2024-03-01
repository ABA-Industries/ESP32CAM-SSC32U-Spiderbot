bool
read_UDP_packet(){
  //read UDP Packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    //Serial.printf("UDP packet contents: %s\n", incomingPacket);
    //Serial.print(incomingPacket);
    return true;
  }
  return false;
}

void
handleBmp()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    //Serial.println("SET-LO-RES FAIL");
  }

  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    //Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  //Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),static_cast<int>(frame->size()));

  if (!frame->toBmp()) {
    //Serial.println("CONVERT FAIL");
    server.send(503, "", "");
    return;
  }
  //Serial.printf("CONVERT OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),static_cast<int>(frame->size()));

  server.setContentLength(frame->size());
  server.send(200, "image/bmp");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void
serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    //Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  //Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),static_cast<int>(frame->size()));

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void
handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    //Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}

void
handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    //Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

void
handleJpg()
{
  server.sendHeader("Location", "/cam-hi.jpg");
  server.send(302, "", "");
}

void
handleMjpeg()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    //Serial.println("SET-HI-RES FAIL");
  }

  //Serial.println("STREAM BEGIN");
  WiFiClient client = server.client();
  auto startTime = millis();
  int res = esp32cam::Camera.streamMjpeg(client);
  if (res <= 0) {
    //Serial.printf("STREAM ERROR %d\n", res);
    return;
  }
  auto duration = millis() - startTime;
  //Serial.printf("STREAM END %dfrm %0.2ffps\n", res, 1000.0 * res / duration);
}

//Task1code:operates the ESP32 Camera server
void
Task1code( void * pvParameters )
{
  for (;;) {
    server.handleClient();
  }
}
