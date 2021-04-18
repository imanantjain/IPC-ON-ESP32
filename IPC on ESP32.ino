#include<esp_ipc.h>
int a = 123;
int b = 456;
int *param0 = &a;
int *param1 = &b;
void run_onCore0(void* arg0);
void run_onCore1(void* arg1);
void (*p0)(void*) = &run_onCore0;
void (*p1)(void*) = &run_onCore1;
TaskHandle_t task0 ;

void core0_task(void* a){
  Serial.printf("The Core ID is : %d\n",xPortGetCoreID());
  for(int i = 0; i < 3; i++)
      {
      esp_ipc_call_blocking(1,*p1,param1);
      delay(6000);
      }
      vTaskDelete(task0);
}

void run_onCore0(void* arg0){
  Serial.printf("This is run_onCore0 and Core ID is : %d\n",xPortGetCoreID());
  Serial.printf("The passed parameter is %d\n",*(int*)arg0);
}

void run_onCore1(void* arg1){
  Serial.printf("This is run_onCore1 and Core ID is : %d\n",xPortGetCoreID());
  Serial.printf("The passed parameter is %d\n",*(int*)arg1);
}



void setup() {
    pinMode(2,OUTPUT);
    Serial.begin(115200);
    esp_ipc_call_blocking(0,*p0,param0);
    delay(1000);
    xTaskCreatePinnedToCore(core0_task,"Task0",10000,(void*)param0,2,&task0,0);
  }

void loop() {
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}
