🛰️ Hệ thống IoT giám sát và điều khiển thiết bị qua MQTT (ESP32 + Adafruit IO + Wokwi)

Dự án xây dựng hệ thống IoT giúp giám sát và điều khiển thiết bị từ xa thông qua giao thức MQTT sử dụng vi điều khiển ESP32. Cảm biến DHT22 được dùng để thu thập dữ liệu nhiệt độ và độ ẩm, sau đó gửi lên nền tảng Adafruit IO để hiển thị và lưu trữ. Người dùng có thể điều khiển bật tắt các thiết bị như đèn LED xanh và LED đỏ thông qua giao diện web hoặc ứng dụng, đồng thời hệ thống tự động bật LED đỏ khi nhiệt độ vượt ngưỡng 60°C. Toàn bộ quá trình hoạt động được hiển thị trên Serial Monitor và được mô phỏng hoàn toàn trên nền tảng Wokwi.

Dự án sử dụng ngôn ngữ C/C++ trong môi trường Arduino IDE, kết hợp các thư viện WiFi.h, PubSubClient.h và DHT.h để thực hiện kết nối mạng, giao tiếp MQTT và đọc dữ liệu cảm biến. Việc kết nối với Adafruit IO được thực hiện bằng tài khoản và khóa API cá nhân, đảm bảo tính bảo mật và ổn định trong truyền nhận dữ liệu.

Cấu trúc dự án bao gồm các tệp chính như sketch.ino (chứa mã nguồn chính), PubSubClient.cpp và PubSubClient.h (thư viện MQTT), cùng diagram.json mô tả sơ đồ mô phỏng phần cứng trong Wokwi. Sau khi cấu hình Wi-Fi và thông tin tài khoản Adafruit IO, chỉ cần nạp chương trình lên ESP32 và mở Serial Monitor để theo dõi các thông số hoạt động.
