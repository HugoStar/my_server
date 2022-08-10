
#include <Wire.h>   //   Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <iarduino_I2C_SHT.h>   //   Подключаем библиотеку для работы с датчиком температуры и влажности I2C-flash.
#include <iarduino_RTC.h>   //   Подключаем библиотеку по работе с часами
#include <iarduino_I2C_TDS.h>   //   Подключаем датчик солеметра
#include <GParser.h>

//УСТРОЙСТВА

iarduino_I2C_SHT sht(0x1A);   //   Датчик температуры и влажностиiarduino_I2C_SHT sht(0x1A);                      
iarduino_I2C_TDS tds(0x1B);   //   Датчик солеметра
iarduino_RTC watch(RTC_DS1307);   //    Датчик времени
int rele_one_light = 2;   //    РЕЛЕ СВЕТА
int rele_two_humidity_up = 3;    //РЕЛЕ УВЛАЖНИТЕЛЯ
int rele_three_humidity_down = 4;   //РЕЛЕ ИССУШИТЕЛЯ

//СОСТОЯНИЯ

int light_is_on = 0;
int humidity_is_up = 0;
int humidity_is_down = 0;

double humidity = 0.0;
double temperature = 0.0;
double current_time = 0.0;
double tds_data = 0.0;

//ОГРАНИЧЕНИЯ

double light_on_time = 10.30;
double light_off_time = 15.20;

double prepare_max_humidity = 80.0;
double prepare_min_humidity = 60.0;


void setup() {
    delay(500);                                   // * Ждём завершение переходных процессов связанных с подачей питания.
    Serial.begin(115200);                           //
    while (!Serial) continue;                             // * Ждём завершения инициализации шины UART.
    watch.begin(); 
    //watch.settime(0,49,9,9,8,22,2);
    sht.begin();
    tds.begin();
}                                                 
                                                  
void loop() {
  prepareData();
  sendSerialData();
  Serial.println();
  delay(10000);
}

//ПОДГОТАВЛИВАЕМ ДАННЫЕ

void prepareData() {
  tds.set_t(29.5f);
  humidity = sht.getHum();
  temperature = sht.getTem();
  watch.gettime(); 
  current_time = (double)watch.Hours + (double)watch.minutes / 100;
  tds_data = tds.getTDS();

  //КОННТРОЛЬ СВЕТА
  if (current_time >= light_on_time && current_time <= light_off_time) {
    light_is_on = 1;
  } else {
    light_is_on = 0;
  }

//  //КОНТРОЛЬ ВЛАЖНОСТИ
  if (humidity > prepare_min_humidity && humidity < prepare_max_humidity) {
    humidity_is_up = 0;
    humidity_is_down = 0;
  } else if (humidity < prepare_min_humidity) {
    humidity_is_up = 1;
    humidity_is_down = 0;
  } else if (humidity > prepare_max_humidity) {
    humidity_is_up = 0;
    humidity_is_down = 1;
  } else {
    humidity_is_up = 0;
    humidity_is_down = 0;
  }
}

//ПОДГОТАВЛИВАЕМ ФЕЙКОВЫЕ ДАННЫЕ
void prepareFakeData() {
  tds.set_t(29.5f);
  humidity = 70.20;
  temperature = 28.2; 
  current_time = 15.30;
  tds_data = 135.8;
}


//ГЕНЕРИРУЕМ ДАННЫЕ ДЛЯ ПЕРЕДАЧИ

void sendSerialData() {
  Serial.print(light_is_on);
  Serial.print(",");
  Serial.print(humidity_is_up);
  Serial.print(",");
  Serial.print(humidity_is_down);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(current_time);
  Serial.print(",");
  Serial.print(tds_data);
  Serial.print(",");
  Serial.print(light_on_time);
  Serial.print(",");
  Serial.print(light_off_time);
  Serial.print(",");
  Serial.print(prepare_min_humidity);
  Serial.print(",");
  Serial.print(prepare_max_humidity);
  Serial.print(";");
}
