#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

int M1 = 0;
int M2 = 90;
int M3 = 180;
int M4 = 170;
int M5 = 0;
int M6 = 73;
int step_delay = 20;
int step_size = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Braccio.begin(20); 
  reset_robot_arm(); 
  Braccio.ServoMovement(step_delay,           M1,  M2, M3, M4, M5,  M6);  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Reading Serial and Send Serial
  if(Serial.available() > 0){
    String data = Serial.readStringUntil('\n');
    
    //Start to move the robot arm
    if(data.equals("101")){  
      M1 = range_trim(M1+step_size, 0, 180);
    }
    else if(data.equals("102")){     
      M2 = range_trim(M2+step_size, 20, 175);
    }
    else if(data.equals("103")){  
      M3 = range_trim(M3+step_size, 0, 180);  
    }
    else if(data.equals("104")){
      M4 = range_trim(M4+step_size, 0, 180);   
    }
    else if(data.equals("105")){  
      M5 = range_trim(M5+step_size, 0, 180);   
    }
    else if(data.equals("106")){     
      M6 = range_trim(M6+step_size, 10, 73);
    }
    else if(data.equals("201")){
      M1 = range_trim(M1-step_size, 0, 180);      
    }
    else if(data.equals("202")){
      M2 = range_trim(M2-step_size, 20, 175);       
    }
    else if(data.equals("203")){
      M3 = range_trim(M3-step_size, 0, 180);       
    }
    else if(data.equals("204")){
      M4 = range_trim(M4-step_size, 0, 180);         
    }
    else if(data.equals("205")){
      M5 = range_trim(M5-step_size, 0, 180);       
    }
    else if(data.equals("206")){
      M6 = range_trim(M6-step_size, 10, 73);         
    }
    else if(data.equals("300")){ //Reset Robot
      reset_robot_arm();       
    }
    else if(data.equals("301")){ //Refresh data for current motor value
      //        
    }
    else if(data.equals("302")){ //step_size++
      step_size = range_trim(step_size+1, 1, 20);        
    }
    else if(data.equals("303")){ //step_size--
      step_size = range_trim(step_size-1, 1, 20);        
    }
    Braccio.ServoMovement(step_delay,           M1,  M2, M3, M4, M5,  M6);  

    char data_str[50];
    sprintf(data_str, "Arduino:%i,%i,%i,%i,%i,%i,%i,%i", step_delay, M1,  M2, M3, M4, M5, M6, step_size);
    Serial.println(data_str);
    delay(1000);       
  }
  else{
    //Serial.println("Hellow from Arduino");
    //delay(1000);   
  }
}

int range_trim(int current_motor_value, int min_num, int max_num){
  if(current_motor_value > max_num){
    return max_num;   
  }
  else if(current_motor_value < min_num){
    return min_num;
  }
  return current_motor_value;  
}

void reset_robot_arm(){
  M1 = 0;
  M2 = 90;
  M3 = 180;
  M4 = 170;
  M5 = 0;
  M6 = 73;
  step_delay = 20;
  step_size = 5;
}

/*
   Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
