#ifndef _WiFiConsts_
#define _WiFiConsts_

/* change it with your ssid-password */
const char* ssid = "Beeline_2G";
const char* password = "Transcend23";

//http://www.hivemq.com/demos/websocket-client
//http://www.hivemq.com

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

/* Not used yet*/
const char *mqtt_user = "Login"; // Логи от сервер
const char *mqtt_pass = "Pass";  // Пароль от сервера

 
/* topics */
#define LAMP_TOPIC     "cvtirer"

#endif