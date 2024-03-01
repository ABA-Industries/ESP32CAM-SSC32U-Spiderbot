void toggle_LED_headlight(){
  //toggle LED Headlight
  if (incomingPacket[0] == 'F')
  {
    digitalWrite(4, !digitalRead(4));
    incomingPacket[0] = old_Packet[0];
  }
}

void toogle_calibration_mode(){
  //toggle calibration mode
  if (incomingPacket[0] == 'C')
  {
    if (calibration_tracker == false)
    {
      leg_group_1_command (0, 0, 0);
      leg_group_2_command (0, 0, 0);
      calibration_tracker = true;
    }
    else
    {
      leg_group_1_command (-45, 45, 45);
      leg_group_2_command (-45, 45, 45);
      calibration_tracker = false;
    }
  }
}

void update_servo_speed(){
  //update servo speed
  if (incomingPacket[0] == 'P')
  {
    if (incomingPacket[1] == '1')
    {
      update_servo_speed_int(1);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '2')
    {
      update_servo_speed_int(2);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '3')
    {
      update_servo_speed_int(3);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '4')
    {
      update_servo_speed_int(4);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '5')
    {
      update_servo_speed_int(5);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '6')
    {
      update_servo_speed_int(6);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '7')
    {
      update_servo_speed_int(7);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '8')
    {
      update_servo_speed_int(8);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '9')
    {
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
  }
}

void update_body_height(){
  //update Spider height
  if (incomingPacket[0] == 'H')
  {
    if (incomingPacket[1] == '1')
    {
      update_defeault_leg_angles(1);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '2')
    {
      update_defeault_leg_angles(2);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '3')
    {
      update_defeault_leg_angles(3);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '4')
    {
      update_defeault_leg_angles(4);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '5')
    {
      update_defeault_leg_angles(5);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '6')
    {
      update_defeault_leg_angles(6);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '7')
    {
      update_defeault_leg_angles(7);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '8')
    {
      update_defeault_leg_angles(8);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
    if (incomingPacket[1] == '9')
    {
      update_defeault_leg_angles(9);
      incomingPacket[0] = old_Packet[0];
      incomingPacket[1] = old_Packet[1];
    }
  }
}

void send_serial_command(){
  //send the final serial command string appended with servo speed
  if (servo_serial_command_buffer.length() > 0 && servo_serial_command_buffer != servo_serial_command_buffer_old || servo_speed_old != servo_speed || spider_height_old != spider_height)
  {
    Serial.println(servo_serial_command_buffer + "T" + String(servo_speed));
  }
  //then reset the string for new command
  servo_serial_command_buffer_old = servo_serial_command_buffer;
  servo_serial_command_buffer = "";
}

void update_servo_speed_int(int speed_int) {
  int max_servo_speed = 150;
  int min_servo_speed = 1000;  //max is 850 but tends to cause robot errors
  speed_int = constrain(speed_int, 1, 9);
  servo_speed = map(speed_int, 1, 9, min_servo_speed, max_servo_speed);
  servo_speed_old = servo_speed;
}

void movement_state_transition(char old_command, char new_command) {
  int transition_delay = servo_speed + 150;  //was 50

  if (old_command != new_command) {
    delay(transition_delay);
    brake();
    servo_serial_command_buffer = "";
  }
}


void update_defeault_leg_angles(int normalized_spider_height) {
  default_standing_femur_angle = map(normalized_spider_height, 1, 9, 45, -45);
  default_standing_tibia_angle = 90 + default_standing_femur_angle;

  spider_height = normalized_spider_height;
  spider_height_old = normalized_spider_height;
}


void servo_position(int servo_num, int servo_angle, int servo_speed) {
  //speed 100-9999
  servo_angle = map(servo_angle, -max_servo_angle, max_servo_angle, -90, 90);
  servo_angle = map(servo_angle, -90, 90, 500, 2500);
  servo_angle = constrain(servo_angle, 500, 2500);
  Serial.print("#" + String(servo_num) + "P" + String(servo_angle) + "T" + String(servo_speed));
  return;
}

void multi_servo_position(int servo_num1, int servo_angle1, int servo_num2, int servo_angle2, int servo_num3, int servo_angle3) {
  //speed 100-9999
  servo_angle1 = map(servo_angle1, -max_servo_angle, max_servo_angle, -90, 90);
  servo_angle1 = map(servo_angle1, -90, 90, 500, 2500);
  servo_angle1 = constrain(servo_angle1, 500, 2500);

  servo_angle2 = map(servo_angle2, -max_servo_angle, max_servo_angle, -90, 90);
  servo_angle2 = map(servo_angle2, -90, 90, 500, 2500);
  servo_angle2 = constrain(servo_angle2, 500, 2500);

  servo_angle3 = map(servo_angle3, -max_servo_angle, max_servo_angle, -90, 90);
  servo_angle3 = map(-servo_angle3, -90, 90, 500, 2500);
  servo_angle3 = constrain(servo_angle3, 500, 2500);

  //Serial.print("#" + String(servo_num1) + "P" + String(servo_angle1) + "#" + String(servo_num2) + "P" + String(servo_angle2) + "#" + String(servo_num3) + "P" + String(servo_angle3) + "T" + String(servo_speed) + "\r\n");
  servo_serial_command_buffer = servo_serial_command_buffer + "#" + String(servo_num1) + "P" + String(servo_angle1) + "#" + String(servo_num2) + "P" + String(servo_angle2) + "#" + String(servo_num3) + "P" + String(servo_angle3);

  return;
}

void servo_test() {
  for (int i = 0; i <= 23; i++) {
    servo_position(i, -90, 100);
    delay(1000);
    servo_position(i, 90, 100);
    delay(1000);
    servo_position(i, 0, 100);
    delay(1000);
  }
}

void specific_leg_relative_command(int leg_ID, int coxa_servo_angle, int femur_servo_angle, int tibia_servo_angle) {
  //spider leg assignment diagram
  //    Front
  //
  //leg 2     leg 1
  //     \   /
  // leg4- 0 -leg 3
  //     /   \
  // leg 6   leg 5

  //group1: 1, 4, 5
  //group2: 2, 3, 6

  switch (leg_ID) {

    //command group1
    case 1:
      multi_servo_position(servo_coxa_1, -coxa_servo_angle, servo_femur_1, -femur_servo_angle, servo_tibia_1, tibia_servo_angle);
      break;
    case 2:
      multi_servo_position(servo_coxa_2, coxa_servo_angle, servo_femur_2, femur_servo_angle, servo_tibia_2, -tibia_servo_angle);
      break;
    case 3:
      multi_servo_position(servo_coxa_3, -coxa_servo_angle, servo_femur_3, -femur_servo_angle, servo_tibia_3, tibia_servo_angle);
      break;
    //command group 2
    case 4:
      multi_servo_position(servo_coxa_4, coxa_servo_angle, servo_femur_4, femur_servo_angle, servo_tibia_4, -tibia_servo_angle);
      break;
    case 5:
      multi_servo_position(servo_coxa_5, -coxa_servo_angle, servo_femur_5, -femur_servo_angle, servo_tibia_5, tibia_servo_angle);
      break;
    case 6:
      multi_servo_position(servo_coxa_6, coxa_servo_angle, servo_femur_6, femur_servo_angle, servo_tibia_6, -tibia_servo_angle);
      break;
    default:
      break;
  }
}

void leg_group_1_command(int coxa_servo_angle, int femur_servo_angle, int tibia_servo_angle) {
  //legs 1, 4, 5
  //  //coxa

  multi_servo_position(servo_coxa_1, -coxa_servo_angle, servo_coxa_4, coxa_servo_angle, servo_coxa_5, coxa_servo_angle);

  //  //femur

  multi_servo_position(servo_femur_1, -femur_servo_angle, servo_femur_4, femur_servo_angle, servo_femur_5, femur_servo_angle);
  //
  //  //tibia

  multi_servo_position(servo_tibia_1, -tibia_servo_angle, servo_tibia_4, tibia_servo_angle, servo_tibia_5, tibia_servo_angle);
}

void leg_group_2_command(int coxa_servo_angle, int femur_servo_angle, int tibia_servo_angle) {
  //legs 2, 3, 6
  //  //coxa

  multi_servo_position(servo_coxa_2, coxa_servo_angle, servo_coxa_3, -coxa_servo_angle, servo_coxa_6, -coxa_servo_angle);

  //  //femur

  multi_servo_position(servo_femur_2, femur_servo_angle, servo_femur_3, -femur_servo_angle, servo_femur_6, -femur_servo_angle);

  //  //tibia

  multi_servo_position(servo_tibia_2, tibia_servo_angle, servo_tibia_3, -tibia_servo_angle, servo_tibia_6, -tibia_servo_angle);
}

void brake() {
  //stand still
  leg_group_1_command(default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);
  leg_group_2_command(default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);
}

void walk_forward(int reference_time_millis, int servo_speed) {

  //total seperate actions preformed
  int action_group_count = 4;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;

  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1
    leg_group_2_command(42, 85, 85);                                                     //leg group 2 forward and up
    leg_group_1_command(0, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 1 back to default pos
  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2
    leg_group_2_command(42, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 2 down
  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 2 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 3) {
    //ACTION GROUP 3
    leg_group_2_command(0, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 2 back to default pos
    leg_group_1_command(42, 85, 85);                                                     // leg group 1 fowrd and up
  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 3 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 4) {
    //ACTION GROUP 4
    leg_group_1_command(42, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 1 down
  }
}

void walk_backward(int reference_time_millis, int servo_speed) {

  //total seperate actions preformed
  int action_group_count = 4;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;

  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1
    leg_group_2_command(-42, 85, 85);                                                    //leg group 2 forward and up
    leg_group_1_command(0, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 1 back to default pos
  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2
    leg_group_2_command(-42, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 2 down
  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 2 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 3) {
    //ACTION GROUP 3
    leg_group_2_command(0, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 2 back to default pos
    leg_group_1_command(-42, 85, 85);                                                    // leg group 1 fowrd and up
  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 3 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 4) {
    //ACTION GROUP 4
    leg_group_1_command(-42, default_standing_femur_angle, default_standing_tibia_angle);  //leg group 1 down
  }
}

void rotate_left(int reference_time_millis, int servo_speed) {

  //  int default_standing_coxa_angle = 0;
  //int default_standing_femur_angle = 5;
  //int default_standing_tibia_angle = 85;

  //total seperate actions preformed
  int action_group_count = 4;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;

  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1
    //leg_group_2_command (-45, -45, 85); //leg group 2 forward and up
    //leg_group_1_command (0, -5, 85);  //leg group 1 back to default pos

    specific_leg_relative_command(1, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(2, -40, 45, 5);

    specific_leg_relative_command(3, 40, 45, 5);

    specific_leg_relative_command(4, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(5, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(6, -40, 45, 5);


  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2

    specific_leg_relative_command(2, -40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(3, 40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(6, -40, default_standing_femur_angle, default_standing_tibia_angle);

  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 2 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 3) {
    //ACTION GROUP 3
    //leg_group_1_command (-45, -45, 85); // leg group 1 fowrd and up

    specific_leg_relative_command(1, 40, 45, 5);

    specific_leg_relative_command(2, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(3, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(4, -40, 45, 5);

    specific_leg_relative_command(5, 40, 45, 5);

    specific_leg_relative_command(6, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 3 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 4) {
    //ACTION GROUP 4
    //leg_group_1_command (-45, -5, 85); //leg group 1 down

    specific_leg_relative_command(1, 40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(4, -40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(5, 40, default_standing_femur_angle, default_standing_tibia_angle);
  }
}

void rotate_right(int reference_time_millis, int servo_speed) {
  //  int default_standing_coxa_angle = 0;
  //int default_standing_femur_angle = 5;
  //int default_standing_tibia_angle = 85;

  //total seperate actions preformed
  int action_group_count = 4;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;

  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1
    //leg_group_2_command (-45, -45, 85); //leg group 2 forward and up
    //leg_group_1_command (0, -5, 85);  //leg group 1 back to default pos

    specific_leg_relative_command(1, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(2, 40, 45, 5);

    specific_leg_relative_command(3, -40, 45, 5);

    specific_leg_relative_command(4, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(5, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(6, 40, 45, 5);


  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2
    //leg_group_2_command (-45, -5, 85); //leg group 2 down

    specific_leg_relative_command(2, 40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(3, -40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(6, 40, default_standing_femur_angle, default_standing_tibia_angle);

  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 2 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 3) {
    //ACTION GROUP 3
    //leg_group_1_command (-45, -45, 85); // leg group 1 fowrd and up

    specific_leg_relative_command(1, -40, 45, 5);

    specific_leg_relative_command(2, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(3, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(4, 40, 45, 5);

    specific_leg_relative_command(5, -40, 45, 5);

    specific_leg_relative_command(6, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 3 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 4) {
    //ACTION GROUP 4
    //leg_group_1_command (-45, -5, 85); //leg group 1 down

    specific_leg_relative_command(1, -40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(4, 40, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(5, -40, default_standing_femur_angle, default_standing_tibia_angle);
  }
}

void strafe_right(int reference_time_millis, int servo_speed) {

  //  int default_standing_coxa_angle = 0;
  //int default_standing_femur_angle = 5;
  //int default_standing_tibia_angle = 85;

  //total seperate actions preformed
  int action_group_count = 6;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;


  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1

    specific_leg_relative_command(1, -45, 45, 70);

    specific_leg_relative_command(2, -45, -10, 110);

    specific_leg_relative_command(3, 0, -55, 10);

    specific_leg_relative_command(4, 0, 45, 90);

    specific_leg_relative_command(5, 45, 45, 70);

    specific_leg_relative_command(6, 45, -10, 110);

  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2

    specific_leg_relative_command(1, -45, 45, 90);

    specific_leg_relative_command(2, -45, -10, 20);

    specific_leg_relative_command(3, 0, -50, 110);

    specific_leg_relative_command(4, 0, 45, 90);

    specific_leg_relative_command(5, 45, 45, 90);

    specific_leg_relative_command(6, 45, -10, 20);



  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 2 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 3) {
    //ACTION GROUP 3

    specific_leg_relative_command(1, -45, -55, 10);

    specific_leg_relative_command(2, -45, -10, 55);

    specific_leg_relative_command(3, 0, -10, 130);

    specific_leg_relative_command(4, 0, -10, 110);

    specific_leg_relative_command(5, 45, -55, 10);

    specific_leg_relative_command(6, 45, -10, 55);



  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 3 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 4) {
    //ACTION GROUP 4


    specific_leg_relative_command(1, -45, -55, 10);

    specific_leg_relative_command(2, -45, 70, 110);

    specific_leg_relative_command(3, 0, 45, 70);

    specific_leg_relative_command(4, 0, -10, 90);

    specific_leg_relative_command(5, 45, -55, 10);

    specific_leg_relative_command(6, 45, 70, 110);

  }

  else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 4 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 5) {
    //ACTION GROUP 5

    specific_leg_relative_command(1, -45, -10, 110);

    specific_leg_relative_command(2, -45, 70, 110);

    specific_leg_relative_command(3, 0, 45, 70);

    specific_leg_relative_command(4, 0, -10, 30);

    specific_leg_relative_command(5, 45, -10, 110);

    specific_leg_relative_command(6, 45, 70, 110);

  }

  else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 5 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 6) {
    //ACTION GROUP 6

    specific_leg_relative_command(1, -45, -10, 110);

    specific_leg_relative_command(2, -45, -10, 110);

    specific_leg_relative_command(3, 0, -55, 10);

    specific_leg_relative_command(4, 0, -10, 30);

    specific_leg_relative_command(5, 45, -10, 110);

    specific_leg_relative_command(6, 45, -10, 110);
  }
}


void strafe_left(int reference_time_millis, int servo_speed) {

  //  int default_standing_coxa_angle = 0;
  //int default_standing_femur_angle = 5;
  //int default_standing_tibia_angle = 85;

  //total seperate actions preformed
  int action_group_count = 6;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;


  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1

    specific_leg_relative_command(2, -45, 45, 70);

    specific_leg_relative_command(1, -45, -10, 110);

    specific_leg_relative_command(4, 0, -55, 10);

    specific_leg_relative_command(3, 0, 45, 90);

    specific_leg_relative_command(6, 45, 45, 70);

    specific_leg_relative_command(5, 45, -10, 110);

  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2

    specific_leg_relative_command(2, -45, 45, 90);

    specific_leg_relative_command(1, -45, -10, 20);

    specific_leg_relative_command(4, 0, -50, 110);

    specific_leg_relative_command(3, 0, 45, 90);

    specific_leg_relative_command(6, 45, 45, 90);

    specific_leg_relative_command(5, 45, -10, 20);



  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 2 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 3) {
    //ACTION GROUP 3

    specific_leg_relative_command(2, -45, -55, 10);

    specific_leg_relative_command(1, -45, -10, 55);

    specific_leg_relative_command(4, 0, -10, 130);

    specific_leg_relative_command(3, 0, -10, 110);

    specific_leg_relative_command(6, 45, -55, 10);

    specific_leg_relative_command(5, 45, -10, 55);



  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 3 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 4) {
    //ACTION GROUP 4


    specific_leg_relative_command(2, -45, -55, 10);

    specific_leg_relative_command(1, -45, 70, 110);

    specific_leg_relative_command(4, 0, 45, 70);

    specific_leg_relative_command(3, 0, -10, 90);

    specific_leg_relative_command(6, 45, -55, 10);

    specific_leg_relative_command(5, 45, 70, 110);

  }

  else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 4 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 5) {
    //ACTION GROUP 5

    specific_leg_relative_command(2, -45, -10, 110);

    specific_leg_relative_command(1, -45, 70, 110);

    specific_leg_relative_command(4, 0, 45, 70);

    specific_leg_relative_command(3, 0, -10, 30);

    specific_leg_relative_command(6, 45, -10, 110);

    specific_leg_relative_command(5, 45, 70, 110);

  }

  else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 5 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 6) {
    //ACTION GROUP 6

    specific_leg_relative_command(2, -45, -10, 110);

    specific_leg_relative_command(1, -45, -10, 110);

    specific_leg_relative_command(4, 0, -55, 10);

    specific_leg_relative_command(3, 0, -10, 30);

    specific_leg_relative_command(6, 45, -10, 110);

    specific_leg_relative_command(5, 45, -10, 110);
  }
}


void greeting(int reference_time_millis, int servo_speed) {
  //  int default_standing_coxa_angle = 0;
  //int default_standing_femur_angle = 5;
  //int default_standing_tibia_angle = 85;

  //total seperate actions preformed
  int action_group_count = 2;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;

  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1

    specific_leg_relative_command(1, -45, default_standing_femur_angle, -90);

    specific_leg_relative_command(2, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(3, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(4, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(5, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);

    specific_leg_relative_command(6, default_standing_coxa_angle, default_standing_femur_angle, default_standing_tibia_angle);


  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2

    specific_leg_relative_command(1, -45, default_standing_femur_angle, -20);
  }
}

void dance(int reference_time_millis, int servo_speed) {

  //total seperate actions preformed
  int action_group_count = 2;

  //add time to movement cycle to allow for movement speed slower than expected (respects a min value)
  int added_time_to_movement_cycle = servo_speed * 2;
  if (added_time_to_movement_cycle < added_time_to_movement_cycle_min) {
    added_time_to_movement_cycle = added_time_to_movement_cycle_min;
  }
  //how long it take to repeat the action (ms)
  int movement_cycle_period = servo_speed * action_group_count + added_time_to_movement_cycle;

  //how many ms have elapsed during the current movemnt cycle
  int current_movement_cycle_elapsed = (millis() - reference_time_millis) % movement_cycle_period;

  //determine which action group to do based on how much time has elapsed in the period of the movement cycle
  if (current_movement_cycle_elapsed < movement_cycle_period / action_group_count) {
    //ACTION GROUP 1

    specific_leg_relative_command(1, -45, 0, 130);

    specific_leg_relative_command(2, -45, 0, 40);

    specific_leg_relative_command(3, 0, 45, -45);

    specific_leg_relative_command(4, 0, 45, -45);

    specific_leg_relative_command(5, 45, 0, 130);

    specific_leg_relative_command(6, 45, 0, 40);


  } else if (current_movement_cycle_elapsed > (movement_cycle_period / action_group_count) * 1 && current_movement_cycle_elapsed < (movement_cycle_period / action_group_count) * 2) {
    //ACTION GROUP 2
    //

    specific_leg_relative_command(1, -45, 0, 40);

    specific_leg_relative_command(2, -45, 0, 130);

    specific_leg_relative_command(3, 0, 0, -90);

    specific_leg_relative_command(4, 0, 0, -90);

    specific_leg_relative_command(5, 45, 0, 40);

    specific_leg_relative_command(6, 45, 0, 130);
  }
}
