#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// Cấu hình Wi-Fi
#define WIFI_SSID "Wokwi-GUEST"  // Tên Wi-Fi
#define WIFI_PASSWORD ""         // Mật khẩu Wi-Fi

// Cấu hình Adafruit IO
#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883
#define AIO_USERNAME ""  // Tên người dùng Adafruit IO
#define AIO_KEY ""  // Khóa API Adafruit IO
#define FEED_BLUE "digital"  // Tên feed cho LED xanh
#define FEED_RED "led_red"    // Tên feed cho LED đỏ
#define FEED_TEMP "temperature"  // Feed cho nhiệt độ
#define FEED_HUMIDITY "humidity"  // Feed cho độ ẩm

// Cấu hình kết nối Wi-Fi và MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Cấu hình chân LED
const int ledBluePin = 4;  // GPIO 4 cho LED xanh
const int ledRedPin = 2;   // GPIO 2 cho LED đỏ

// Chân cảm biến DHT (Giả sử dùng DHT22)
#define DHTPIN 14  // Cảm biến DHT kết nối với GPIO 14
#define DHT_TYPE DHT22
DHT dht(DHTPIN, DHT_TYPE);

unsigned long previousMillis = 0;  // Thời gian trôi qua
const long interval = 2000;  // Thời gian cập nhật nhiệt độ và độ ẩm (2 giây)

void setup() {
  Serial.begin(115200);  // Khởi tạo Serial Monitor

  // Kết nối Wi-Fi
  connectWiFi();

  // Kết nối MQTT
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);

  pinMode(ledBluePin, OUTPUT);  // Cài đặt chân LED xanh làm đầu ra
  pinMode(ledRedPin, OUTPUT);   // Cài đặt chân LED đỏ làm đầu ra

  // Khởi tạo cảm biến DHT
  dht.begin();
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqttReconnect() {
  while (!client.connected()) {
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), AIO_USERNAME, AIO_KEY)) {
      Serial.println("Connected to MQTT");
      // Đăng ký vào các feed để điều khiển LED và theo dõi nhiệt độ và độ ẩm
      client.subscribe(AIO_USERNAME "/feeds/" FEED_BLUE);  
      client.subscribe(AIO_USERNAME "/feeds/" FEED_RED);
      client.subscribe(AIO_USERNAME "/feeds/" FEED_TEMP);  // Đăng ký feed nhiệt độ
      client.subscribe(AIO_USERNAME "/feeds/" FEED_HUMIDITY);  // Đăng ký feed độ ẩm
    } else {
      Serial.print("Failed to connect to MQTT, state: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String stMessage;
  for (int i = 0; i < length; i++) {
    stMessage += (char)message[i];
  }

  // Chuyển tin nhắn sang chữ in hoa để đồng nhất
  stMessage.toUpperCase();  // Chuyển thành chữ in hoa

  // Điều khiển LED xanh dựa trên tin nhắn nhận được từ feed digital
  if (String(topic) == AIO_USERNAME "/feeds/" FEED_BLUE) {
    if (stMessage == "ON") {
      digitalWrite(ledBluePin, HIGH);  // Bật LED xanh
      Serial.println("LED Blue: ON");
    } else if (stMessage == "OFF") {
      digitalWrite(ledBluePin, LOW);  // Tắt LED xanh
      Serial.println("LED Blue: OFF");
    }
  }

  // Điều khiển LED đỏ dựa trên nhiệt độ
  if (String(topic) == AIO_USERNAME "/feeds/" FEED_TEMP) {
    float temperature = atof(stMessage.c_str());  // Chuyển chuỗi thành số thực
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");  // In ra đơn vị nhiệt độ

    if (temperature > 60) {
      digitalWrite(ledRedPin, HIGH);  // Bật LED đỏ nếu nhiệt độ > 60°C
      Serial.println("LED Red: ON (Temperature > 60°C)");
    } else {
      digitalWrite(ledRedPin, LOW);  // Tắt LED đỏ nếu nhiệt độ <= 60°C
      Serial.println("LED Red: OFF (Temperature <= 60°C)");
    }
  }
}

void sendTemperatureToMQTT(float temp) {
  if (client.connected()) {
    String tempPayload = "{\"value\": " + String(temp) + "}";
    client.publish(AIO_USERNAME "/feeds/" FEED_TEMP, tempPayload.c_str());
    Serial.print("Sent temperature to MQTT: ");
    Serial.print(temp);
    Serial.println(" °C");  // In ra đơn vị nhiệt độ
  }
}

void sendHumidityToMQTT(float humidity) {
  if (client.connected()) {
    String humidityPayload = "{\"value\": " + String(humidity) + "}";
    client.publish(AIO_USERNAME "/feeds/" FEED_HUMIDITY, humidityPayload.c_str());
    Serial.print("Sent humidity to MQTT: ");
    Serial.print(humidity);
    Serial.println(" %");  // In ra đơn vị độ ẩm
  }
}

void loop() {
  unsigned long currentMillis = millis();  // Lấy thời gian hiện tại

  if (!client.connected()) {
    mqttReconnect();  // Reconnect to MQTT if disconnected
  }
  client.loop();

  // Chỉ gửi nhiệt độ và độ ẩm nếu đủ thời gian (2 giây)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Cập nhật thời gian
    // Đọc nhiệt độ và độ ẩm từ cảm biến DHT
    float temperature = dht.readTemperature();  // Lấy nhiệt độ (đơn vị Celsius)
    float humidity = dht.readHumidity();  // Lấy độ ẩm

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read temperature or humidity from DHT sensor");
    } else {
      // Gửi nhiệt độ và độ ẩm lên MQTT
      sendTemperatureToMQTT(temperature);
      sendHumidityToMQTT(humidity);
    }
  }
}
