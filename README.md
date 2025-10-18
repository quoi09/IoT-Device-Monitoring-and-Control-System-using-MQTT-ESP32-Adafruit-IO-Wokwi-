# IoT-Device-Monitoring-and-Control-System-using-MQTT-ESP32-Adafruit-IO-Wokwi-
Hệ thống IoT giám sát và điều khiển thiết bị qua MQTT
(ESP32 + Adafruit IO + Wokwi)

📌 Giới thiệu

Dự án xây dựng hệ thống IoT giám sát và điều khiển thiết bị thông qua giao thức MQTT sử dụng ESP32.
Dữ liệu cảm biến nhiệt độ và độ ẩm được gửi lên Adafruit IO, đồng thời người dùng có thể bật tắt thiết bị từ xa qua giao diện web.
Dự án được mô phỏng trên nền tảng Wokwi.

⚙️ Chức năng chính

Đọc nhiệt độ và độ ẩm từ cảm biến DHT22
Gửi dữ liệu cảm biến lên Adafruit IO qua MQTT
Nhận lệnh điều khiển từ người dùng để bật tắt LED xanh và LED đỏ
Tự động bật LED đỏ khi nhiệt độ vượt ngưỡng 60°C
Hiển thị thông tin hoạt động trên Serial Monitor

🧠 Công nghệ sử dụng

Vi điều khiển: ESP32
Cảm biến: DHT22
Giao thức: MQTT
Nền tảng Cloud: Adafruit IO
Mô phỏng: Wokwi

🚀 Cách chạy dự án

Mở dự án trong Wokwi
Cài đặt các thư viện cần thiết:
WiFi.h, PubSubClient.h, DHT.h
Nhập thông tin AIO_USERNAME và AIO_KEY từ tài khoản Adafruit IO
Nạp code lên ESP32 và mở Serial Monitor để theo dõi kết quả
