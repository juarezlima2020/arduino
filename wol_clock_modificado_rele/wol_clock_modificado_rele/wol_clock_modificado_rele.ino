//************* Wol Clock by Jon Fuge ******************************

//************* Declarar bibliotecas incluídas ******************************
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <BlynkSimpleEsp8266.h> //do relé
#define BLYNK_TEMPLATE_ID "TMPL2kmfuXcPx"
#define BLYNK_TEMPLATE_NAME "wolclock"
#define BLYNK_AUTH_TOKEN "fYUkvrKANuUjvgq54aGXF5n1OcMh25tq" //jupel tec - juarez.lima2020@gmail.com

//************* Declarar estruturas ******************************
//Crie estrutura para informações LED RGB
struct RGB {
  byte r, g, b;
};

//Criar estrutura para informações de tempo
struct TIME {
  byte Hour, Minute;
};

//************* Opções Editáveis ******************************
//A cor do "12" para dar referência visual ao topo
const RGB Twelve = { 100, 100, 100};     // meio dia = Azul =={ 128, 0, 128 }; //roxa clara
//A cor dos "quartos" 3, 6 e 9 para dar referência visual
const RGB Quarters = { 64, 0, 40 };  //roxa escura
//Ta cor das "divisões" 1,2,4,5,7,8,10 e 11 para dar referência visual
const RGB Divisions = { 32, 0, 20 }; //roxa mais escura ainda
//Todos os outros pixels sem informação
const RGB Background = { 0, 0, 2 };  //azul claro

//O ponteiro das horas
const RGB Hour = { 255, 200, 0 };    //AMARELO
//O ponteiro dos minutos
const RGB Minute = { 0, 51, 1 };     //VERDE
//O ponteiro dos segundos
const RGB Second = { 200, 16, 0 };   //LARANJA == 

// Fazer o relógio avançar ou retroceder (dependendo do hardware)
const char ClockGoBackwards = 1;

//Defina o brilho por hora para o modo noturno e diurno
const TIME WeekNight = {22, 30};     // A noite vai escurecer
const TIME WeekMorning = {5, 45};    // Manhã hora de ir brilhante
const TIME WeekendNight = {23, 30};  // A noite vai escurecer
const TIME WeekendMorning = {7, 30}; // Manhã hora de ir brilhante

const int day_brightness = 255;
const int night_brightness = 100;

//Defina seu fuso horário em horas de diferença rom GMT (Horário de verão)
const int hours_Offset_From_GMT = -3;

//Defina seus detalhes de wi-fi para que a placa possa se conectar e obter o tempo da internet
//const char *ssid      = "JPL";    //  your network SSID (name)
//const char *password  = "123@juarez"; // your network password

char ssid[] = "JPL"; //VARIÁVEL QUE ARMAZENA O NOME DA REDE SEM FIO EM QUE VAI CONECTAR
char pass[] = "123@juarez"; //VARIÁVEL QUE ARMAZENA A SENHA DA REDE SEM FIO EM QUE VAI CONECTAR
char auth[] = BLYNK_AUTH_TOKEN; //do relé
byte SetClock;

// Por padrão, 'time.nist.gov' é usado.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Qual pino do ESP8266 está conectado aos NeoPixels?
#define PIN           2 // 14 This is the D5 pin /-\ esp01 = 2 = nodemcu 2 = D4
const int GPIO_0 = 0; //PINO DIGITAL UTILIZADO PELO RELÉ DO MÓDULO
//const int GPIO_2 = 2; //PINO DIGITAL UTILIZADO PELO RELÉ DO MÓDULO
//************* Declare user functions ******************************
void Draw_Clock(time_t t, byte Phase);
int ClockCorrect(int Pixel);
void SetBrightness(time_t t);
void SetClockFromNTP ();
bool IsDst();

//************* Declare NeoPixel ******************************
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

BlynkTimer timer;//do relé

//************* Setup function for Wol_Clock ******************************
void setup() {

  pinMode(GPIO_0, OUTPUT); //DEFINE O PINO COMO SAÍDA
  //pinMode(GPIO_2, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(GPIO_0, HIGH); //RELÉ INICIA DESLIGADO
  //digitalWrite(GPIO_2, HIGH); //RELÉ INICIA DESLIGADO
  Blynk.begin(auth, ssid, pass); //INICIALIZA A COMUNICAÇÃO BLYNK INFORMANDO OS PARÂMETROS
  Blynk.config(auth);
  
  pixels.begin();   // Isso inicializa a biblioteca NeoPixel.
  Draw_Clock(0, 1); // Basta desenhar um relógio em branco

 //WiFi.begin(ssid, password); // Tente se conectar ao Wi-Fi
  Draw_Clock(0, 2);           // Desenhe o fundo do relógio

  while ( WiFi.status() != WL_CONNECTED )
    delay ( 500 );   // continue esperando até nos conectarmos com sucesso ao WiFi

  Draw_Clock(0, 3);  // Adicione os indicadores de quarto de hora

  SetClockFromNTP(); // obtenha a hora do servidor NTP com correção de fuso horário
}

void SetClockFromNTP ()
{
  timeClient.update();                // obtenha a hora do servidor NTP
  setTime(timeClient.getEpochTime()); // Definir a hora do sistema a partir do relógio
  if (IsDst())
    adjustTime((hours_Offset_From_GMT - 3) * 3600); // compensar a hora do sistema com o fuso horário definido pelo usuário (3600 segundos em uma hora)
  else
    adjustTime(hours_Offset_From_GMT * 3600); // compensar a hora do sistema com o fuso horário definido pelo usuário (3600 segundos em uma hora)
}

bool IsDst()
{
  if (month() < 3 || month() > 10)  return false; 
  if (month() > 3 && month() < 10)  return true; 

  int previousSunday = day() - weekday();

  if (month() == 3) return previousSunday >= 24;
  if (month() == 10) return previousSunday < 24;

  return false; // esta linha nunca vai acontecer
}
        
//************* Main program loop for Wol_Clock ******************************
void loop() {

 Blynk.run(); //INICIALIZA O BLYNK
  
  time_t t = now();   // Obter a hora atual

  Draw_Clock(t, 4);   // Desenhe todo o mostrador do relógio com horas, minutos e segundos
  if (minute(t) == 0) // no início de cada hora, atualize a hora do servidor de horário
    if (SetClock == 1)
    {
      SetClockFromNTP(); // obtenha a hora do servidor NTP com correção de fuso horário
      SetClock = 0;
    }
  else
  {
    delay(200); // Basta esperar por 0,1 segundos
    SetClock = 1;
  }
}

//************* Functions to draw the clock ******************************
void Draw_Clock(time_t t, byte Phase)
{
  if (Phase <= 0)
    for (int i = 0; i < 60; i++)
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // para Fase = 0 ou menos, todos os pixels são pretos

  if (Phase >= 1)
    for (int i = 0; i < 60; i++)
      pixels.setPixelColor(i, pixels.Color(Background.r, Background.g, Background.b)); // para Fase = 1 ou mais, desenhe os minutos com a cor de fundo

  if (Phase >= 2)
    for (int i = 0; i < 60; i = i + 5)
      pixels.setPixelColor(i, pixels.Color(Divisions.r, Divisions.g, Divisions.b)); // para Fase = 2 ou mais, desenhe divisões de 5 minutos

  if (Phase >= 3) {
    for (int i = 0; i < 60; i = i + 15)
      pixels.setPixelColor(ClockCorrect(i), pixels.Color(Quarters.r, Quarters.g, Quarters.b)); // for Phase = 3 or more, draw 15 minute divisions
    pixels.setPixelColor(ClockCorrect(0), pixels.Color(Twelve.r, Twelve.g, Twelve.b)); // for Phase = 3 and above, draw 12 o'clock indicator
  }

  if (Phase >= 4) {
    pixels.setPixelColor(ClockCorrect(second(t)), pixels.Color(Second.r, Second.g, Second.b)); // desenhe a segunda mão primeiro
    if (second() % 2)
      pixels.setPixelColor(ClockCorrect(minute(t)), pixels.Color(Minute.r, Minute.g, Minute.b)); // para ajudar na identificação, o ponteiro dos minutos pisca entre normal e meia intensidade
    else
      pixels.setPixelColor(ClockCorrect(minute(t)), pixels.Color(Minute.r / 2, Minute.g / 2, Minute.b / 2)); // ponteiro dos minutos de menor intensidade

    pixels.setPixelColor(ClockCorrect(((hour(t) % 12) * 5) + minute(t) / 12), pixels.Color(Hour.r, Hour.g, Hour.b)); // desenhe o ponteiro das horas por último
  }

  SetBrightness(t); // Defina o brilho do relógio dependendo da hora
  pixels.show();    // mostrar todos os pixels
}

//************* Função para definir o brilho do relógio ******************************
void SetBrightness(time_t t)
{
  int NowHour = hour(t);
  int NowMinute = minute(t);

  if ((weekday() >= 2) && (weekday() <= 6))
    if ((NowHour > WeekNight.Hour) || ((NowHour == WeekNight.Hour) && (NowMinute >= WeekNight.Minute)) || ((NowHour == WeekMorning.Hour) && (NowMinute <= WeekMorning.Minute)) || (NowHour < WeekMorning.Hour))
      pixels.setBrightness(night_brightness);
    else
      pixels.setBrightness(day_brightness);
  else if ((NowHour > WeekendNight.Hour) || ((NowHour == WeekendNight.Hour) && (NowMinute >= WeekendNight.Minute)) || ((NowHour == WeekendMorning.Hour) && (NowMinute <= WeekendMorning.Minute)) || (NowHour < WeekendMorning.Hour))
    pixels.setBrightness(night_brightness);
  else
    pixels.setBrightness(day_brightness);
}

//************* Esta função inverte a ordem dos pixels ******************************
int ClockCorrect(int Pixel)
{
  if (ClockGoBackwards == 1)
    return ((60 - Pixel +30) % 60); // minha primeira tentativa de dirigir o relógio foi para trás :)
  else
    return (Pixel);
}
