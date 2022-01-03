#include <Servo.h>
#define EN 8
#define X_DIR 5
#define Y_DIR 6
#define Z_DIR 7
#define W_DIR 13
#define P_DIR 40
#define X_STP 2
#define Y_STP 3
#define Z_STP 4
#define W_STP 12
#define P_STP 38
#define X_photo 9
#define Y_photo 10
#define Z_photo 11
#define W_photo A3
#define P_photo 30
#define servoPin A0
#define TRIG 22 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 24 //ECHO 핀 설정 (초음파 받는 핀)

Servo myservo;

// 아날로그 0번핀 사용
int X_photo_init = A2;         
int angle = 0;
//serial_num받아오는 변수
int serial_num=0;
int serial_box_num=1;
int serial_stop =0;
int mode_select_flag = 0;
//스피드 설정
int x_speed =1000;
int y_speed =800;
int z_speed =500;
int w_speed =800;
int p_speed =550;

//위치 설정
unsigned int x_position =0;
unsigned int y_position =0;
unsigned int z_position =0;
unsigned int w_position =0;

void setup ()
{

  //connect(영준수정)
  Serial.begin(9600);
  Serial.setTimeout(50);
  /////////limit switch///////////
   pinMode(X_photo, INPUT); 
   pinMode(Y_photo, INPUT); 
   pinMode(Z_photo, INPUT);
   pinMode(W_photo, INPUT);
   pinMode(P_photo, INPUT);
   ////////step motor//////////
   pinMode (X_DIR, OUTPUT); pinMode (X_STP, OUTPUT);
   pinMode (Y_DIR, OUTPUT); pinMode (Y_STP, OUTPUT);
   pinMode (Z_DIR, OUTPUT); pinMode (Z_STP, OUTPUT);
   pinMode (W_DIR, OUTPUT); pinMode (W_STP, OUTPUT);
   pinMode (P_DIR, OUTPUT); pinMode (P_STP, OUTPUT);
   pinMode(TRIG, OUTPUT);
   pinMode(ECHO, INPUT);
   pinMode (EN, OUTPUT);
   digitalWrite (EN, LOW);
   ////////servo motor
   myservo.attach(servoPin, 500, 2500);
   To_init();
}

void loop ()
{  
     //serial 받아오기
     serial_num = Serial.read();
        //스위치 값 받아오기
     if(serial_num == 10)
     {
        mode_select_flag = 1;
     }
     else if(serial_num ==20)
     {
        mode_select_flag = 2;
     }
     switch (mode_select_flag){
      case 1:
        AutoMode();
        break;
      case 2:
        if(serial_num == 11)
        {
          x_cw_run();
        }
        else if(serial_num == 12)
        {
          x_ccw_run();
        }
        else if(serial_num == 21)
        {
          y_cw_run();
        }
        else if(serial_num == 22)
        {
          y_ccw_run();
        }
        else if(serial_num == 31)
        {
          z_up_run();
        }
        else if(serial_num == 32)
        {
          z_down_run();
        }
        else if(serial_num == 41)
        {
          w_cw_run();
        }
        else if(serial_num == 42)
        {
          w_ccw_run();
        }
        else if(serial_num == 51)
        {
          gripper_open();
        }
        else if(serial_num == 52)
        {
          gripper_close();
        }
     }
}
//오토모드
//집고오는 형식
void AutoMode() 
{
  if(Distance()<12)
  {
    forward();
    delay(200);
    To_DarkBox_From_init();
    back();
    //serial_box_num = Serial.read();
    switch (serial_box_num)
    {
      case 1:
        To_Box1_From_DarkBox();
        break;
      case 2:
        To_Box2_From_DarkBox();
        break;
      case 3:
        To_Box3_From_DarkBox();
        break;
    }

  }
}

//오토모드 도와주는 함수
void To_DarkBox_From_init()
{
  To_DarkBox_Z();
  To_DarkBox_Y();
  To_DarkBox_X();
  To_DarkBox_W();
  gripper_open();
  delay(1000);
  down();
  delay(1000);
  gripper_close();
  delay(1000);
  To_DarkBox_Z();
}
void To_Box1_From_DarkBox()
{
  x_cw100();
  y_cw80();
  gripper_open();
}
void To_Box2_From_DarkBox()
{
  x_cw90();
  y_cw80();
  gripper_open();
}
void To_Box3_From_DarkBox()
{
  x_cw80();
  y_clockwise();   //영준이가 바꿨따
  gripper_open();
}
void To_init()
{
  To_Xinit();
  To_DarkBox_Z();
  To_DarkBox_Y();
  To_DarkBox_W();
  gripper_open();
  x_ccw90();
}
//PHOTO
//포토인터럽트 감지시 멈추는 함수
void To_Xinit()
{ 
  run_set(true, X_DIR);
  while(1)
  {
    if(analogRead(X_photo_init) <500)
    {
      break;
    }
     x_position++;
     run(X_STP, x_speed);
  }
}
void To_DarkBox_XinitYZW()
{
  run_set(false, X_DIR);
  run_set(false, Y_DIR);
  run_set(false, Z_DIR);
  run_set(false, W_DIR);
  while(1)
  {
    if(digitalRead(X_photo_init) == LOW)
    {
      break;
    }
    else
    {
     x_position++;
     run(X_STP, x_speed); 
    }
    if(digitalRead(Y_photo) == LOW)
    {
      break;
    }
    else
    {
     y_position++;
     run(Y_STP, y_speed); 
    }
    if(digitalRead(Z_photo) == HIGH)
    {
      break;
    }
    else
    {
     x_position++;
     run(Z_STP, z_speed); 
    }
    if(digitalRead(W_photo) == HIGH)
    {
      break;
    }
     w_position++;
         run(W_STP, w_speed);
  }
}
void To_DarkBox_XYZW()
{
  run_set(false, X_DIR);
  run_set(false, Y_DIR);
  run_set(false, Z_DIR);
  run_set(false, W_DIR);
  int x_end=0;
  int y_end=0;
  int z_end=0;
  int w_end=0;
  while(1)
  {
    if(digitalRead(X_photo) == LOW)
    {
      break;
    }
    else
    {
     x_position++;
     run(X_STP, x_speed); 
    }
    if(digitalRead(Y_photo) == LOW)
    {
      break;
    }
    else
    {
     y_position++;
     run(Y_STP, y_speed); 
    }
    if(digitalRead(Z_photo) == HIGH)
    {
      break;
    }
    else
    {
     x_position++;
     run(Z_STP, z_speed); 
    }
    if(digitalRead(W_photo) == HIGH)
    {
      break;
    }
     w_position++;
         run(W_STP, w_speed);
  }
}
void To_DarkBox_X()
{ 
  run_set(false, X_DIR);
  while(1)
  {
    if(digitalRead(X_photo) == LOW)
    {
      break;
    }
     x_position++;
     run(X_STP, x_speed);
  }
}
void To_DarkBox_Y()
{
  run_set(false, Y_DIR);

  while(1)
  {
    if(digitalRead(Y_photo) == LOW)
    {
      break;
    }
     y_position++;
     run(Y_STP, y_speed);
  }
}
void To_DarkBox_Z()
{
  run_set(true, Z_DIR);

  while(1)
  {
    if(digitalRead(Z_photo) == HIGH) //걸릴때가 high!!
    {
      break;
    }
     z_position++;
         run(Z_STP, z_speed);
  }
}
void To_DarkBox_W()
{
  run_set(false, W_DIR);

  while(1)
  {
    if(digitalRead(W_photo) == HIGH)
    {
      break;
    }
     w_position++;
         run(W_STP, w_speed);
  }
}

//To 우인
//수동조작 : 버튼누를 때 동작 안누르면 동작X
void x_cw_run()
{
   run_set(true, X_DIR);
   while (1) //회전 시작
   {
      serial_num=Serial.read();
      if (digitalRead(X_photo_init)  == LOW)
      {
         break;
      }//센서 접촉시

      else if (serial_num == 4) //버튼 뗄때
      {
         digitalWrite(EN, HIGH);
         break;
      }// 멈춤

      else
      {
         x_position--;
         run(X_STP, x_speed);
      }// 작동

   }//while

}// x_cw_run
void x_ccw_run()
{
   run_set(false, X_DIR);
   while (1) //회전 시작
   {
      serial_num=Serial.read();
      if (digitalRead(X_photo) == LOW)
      {
         break;
      }
      else if (serial_num == 4) //버튼 뗄때
      {
         digitalWrite(EN, HIGH);
         break;
      }
      else
      {
        x_position++;
         run(X_STP, x_speed);
      }
   }
}

void y_cw_run()
{
  run_set(true, Y_DIR);
  while(1) //회전 시작
  {
     serial_num=Serial.read(); 
     

      if (serial_num == 4) //버튼 뗄때
      {
         digitalWrite(EN, HIGH);
         break;
      }// 멈춤

      else
      {
        y_position++;
         run(Y_STP, y_speed);
      }// 작동

  }
}

void y_ccw_run()
{
   run_set(false, Y_DIR);
   while (1) //회전 시작
   {
      serial_num=Serial.read();
      if (digitalRead(Y_photo) == LOW)
      {
         break;
      }

      else if (serial_num == 4)//버튼 뗄때
      {
         digitalWrite(EN, HIGH); //모든 스텝모터 비활성화
         break;
      }

      else
      {
        y_position--;
         run(Y_STP, y_speed);
      }
   }
      
}
void z_up_run()
{
  run_set(true, Z_DIR);
  while(1) //회전 시작
  {
     serial_num=Serial.read();     
     if (digitalRead(Z_photo) == HIGH)
      {
         break;
      }//센서 접촉시
      else if (serial_num == 4) //버튼 뗄때
      {
         digitalWrite(EN, HIGH);
         break;
      }// 멈춤

      else
      {
         z_position--;
         run(Z_STP, z_speed);
      }// 작동
  }
}
void z_down_run()
{
  run_set(false, Z_DIR);
  while(1) //회전 시작
  {
     serial_num=Serial.read();
     
      if (serial_num == 4) //버튼 뗄때
      {
         break;
      }// 멈춤

      else
      {
        z_position++;
         run(Z_STP, z_speed);
      }// 작동

  }
}
void w_cw_run()
{
  run_set(false, W_DIR);
  while(1) //회전 시작
  {
     serial_num=Serial.read();
  
      if (serial_num == 4) //버튼 뗄때
      {
         break;
      }// 멈춤
      else
      {
         w_position--;
         run(W_STP, w_speed);
      }// 작동
  }
}
void w_ccw_run()
{
  run_set(true, W_DIR);
  while(1) //회전 시작
  {
     serial_num=Serial.read();
     
      if (serial_num == 4) //버튼 뗄때
      {
         break;
      }// 멈춤
      else
      {
          w_position++;
         run(W_STP, w_speed);
      }// 작동
  }
}
void forward_app() //포토인터럽트로 가기
{
  run_set(false, P_DIR);
  while(1) //회전 시작
  {
     serial_num=Serial.read();
     if (digitalRead(P_photo) == LOW)
      {
         break;
      }//센서 접촉시
      else if (serial_num == 4) //버튼 뗄때
      {
         digitalWrite(EN, HIGH);
         break;
      }// 멈춤
      else
      {
         run(P_STP, p_speed);
      }// 작동
  }
}
void back_app()
{
  run_set(true, P_DIR);
  while(1) //회전 시작
  {
     serial_num=Serial.read();
     
      if (serial_num == 4) //버튼 뗄때
      {
         digitalWrite(EN, HIGH);
         break;
      }// 멈춤
      else
      {
         run(P_STP, p_speed);
      }// 작동
  }
}
/////////////////그리퍼조작//////////////////////
void gripper_open()
{
     myservo.write(50);
}

void gripper_close()
{
    myservo.write(170);
}
////////////////동작 // 정지//////////////////////////////
void run_set(boolean dir, byte dirPin)
{
  digitalWrite (EN, LOW); //스텝모터 활성화
  digitalWrite (dirPin, dir);  // 방향: 정회전
  delay (50);
}
void run(byte stepperPin, int speed)
{
  digitalWrite(stepperPin, HIGH);
  delayMicroseconds(speed);
  digitalWrite(stepperPin, LOW);
  delayMicroseconds(speed);
}
void stop()
{
  digitalWrite (EN, HIGH);
}

////////////////스텝모터 동작함수//////////////////////////////
void step (boolean dir, byte dirPin, byte stepperPin, int steps, int speed)
{
  digitalWrite (EN, LOW);
  digitalWrite (dirPin, dir);
  delay (50);
  
  for (int i = 0; i<steps; i++)
  {
      digitalWrite (stepperPin, HIGH);
      delayMicroseconds (speed);
      digitalWrite (stepperPin, LOW);
      delayMicroseconds (speed);
  }
}
/////////////////////x, y, z동작함수////////////////////////
void y_clockwise()
{
  step(true, Y_DIR, Y_STP, 450, y_speed);
}

void x_cw40() // x축 90도 이동 [시계방향]
{
  step(true, X_DIR, X_STP, 800, x_speed);
}
void x_ccw40() // x축 90도 이동 [반시계방향]
{
  step(false, X_DIR, X_STP, 800, x_speed);
}
void x_cw80() // x축 90도 이동 [시계방향]
{
  step(true, X_DIR, X_STP, 880, x_speed);
}
void x_ccw80() // x축 90도 이동 [반시계방향]
{
  step(false, X_DIR, X_STP, 1200, x_speed);
}
void x_cw90() // x축 90도 이동 [시계방향]
{
  step(true, X_DIR, X_STP, 1398, x_speed);
}
void x_ccw90() // x축 90도 이동 [반시계방향]
{
  step(false, X_DIR, X_STP, 1400, x_speed);
}
void x_cw100() // x축 90도 이동 [시계방향]
{
  step(true, X_DIR, X_STP, 1700, x_speed);
}
void x_ccw100() // x축 90도 이동 [반시계방향]
{
  step(false, X_DIR, X_STP, 1700, x_speed);
}
void x_ccw180() // x축 180도 이동 [반시계방향]
{
  step(false, X_DIR, X_STP, 2500, x_speed);
}
void y_cw80() // y축 90도 이동 [시계방향]
{
  step(true, Y_DIR, Y_STP, 600, y_speed);
}
void y_ccw80() // y축 90도 이동 [반시계방향]
{
  step(false, Y_DIR, Y_STP, 600, y_speed);
}
void y_cw90() // y축 90도 이동 [시계방향]
{
  step(true, Y_DIR, Y_STP, 700, y_speed);
}
void y_ccw90() // y축 90도 이동 [반시계방향]
{
  step(false, Y_DIR, Y_STP, 700, y_speed);
}
void up() // z축 90도 이동 [시계방향]
{
  step(true, Z_DIR, Z_STP, 4500, z_speed);
}
void down() // z축 90도 이동 [반시계방향]
{
  step(false, Z_DIR, Z_STP, 1700, z_speed);
}
void w_cw90() // y축 90도 이동 [시계방향]
{
  step(false, W_DIR, W_STP, 700, w_speed);
}
void w_ccw90() // y축 90도 이동 [반시계방향]
{
  step(true, W_DIR, W_STP, 700, w_speed);
}
void w_cw20() // y축 90도 이동 [시계방향]
{
  step(false, W_DIR, W_STP, 350, w_speed);
}
void w_ccw20() // y축 90도 이동 [반시계방향]
{
  step(true, W_DIR, W_STP, 163, w_speed);
}
void forward() //포토인터럽트로 가기
{
  run_set(false, P_DIR);
  while(1) //회전 시작
  {
     if (digitalRead(P_photo) == LOW)
      {
        Serial.println(5);
         break;
      }//센서 접촉시
      else
      {
         
         run(P_STP, p_speed);
      }// 작동
  }
}
void back()
{
  run_set(true, P_DIR);
  for (int i = 0; i<1800; i++)
  {
     
      run(P_STP, p_speed);
  }
}
int Distance() {
  int i;
  int index = 0;
  int max = 0;
  int array1[10];
  int array2[10];

  for (i = 0; i<10; i++) {
    array1[i] = getDistanceSensor();
  }

  for (i = 0; i<10; i++) {
    array2[i] = getCount(array1, array1[i]);
  }

  for (i = 0; i<9; i++) {
    if (array2[i] > max) {
      max = array2[i];
      index = i;
    }
  }
  return array1[index];
}
int getDistanceSensor()
{
  long start_time, end_time;
  int distance;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(100);
  digitalWrite(TRIG, LOW);
  while (digitalRead(ECHO) == 0);
  start_time = micros();
  while (digitalRead(ECHO) == 1);
  end_time = micros();
  distance = (end_time - start_time) / 58;
  delay(100);
  return distance;
}
int getCount(int *array, int value) {
  int i;
  int count = 0;
  for (i = 0; i<10; i++) {
    if (array[i] == value) {
      count++;
    }
  }
  return count;
}
