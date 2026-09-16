# TB3582 System

Qt 6 기반 Raspberry Pi 센서 모니터링 프로그램입니다. ADS1115 전압은 Linux IIO/sysfs에서 읽으며, MAX6675 온도와 HX711 하중을 함께 표시합니다.

## 개발 및 빌드

프로젝트 소스는 PC의 `D:\TB-3582\System`에서만 수정합니다. Raspberry Pi에는 Git으로 변경 내용을 전달하며, Pi에서 프로젝트 소스를 직접 수정하지 않습니다.

CMake 3.16 이상, C++17 컴파일러, Qt 6.9.1 이상(Core, Gui, Widgets, Charts)이 필요합니다. Linux 빌드는 pigpio, hx711, lgpio 라이브러리도 필요합니다. 해당 의존성이 설치된 Qt Creator에서 `CMakeLists.txt`를 열어 빌드하거나 다음을 실행합니다.

```sh
cmake -S . -B build-local
cmake --build build-local
```

Qt를 자동으로 찾지 못하면 구성 단계에 `-DCMAKE_PREFIX_PATH=<Qt 설치 경로>`를 추가합니다. Windows 실행은 센서 시뮬레이션이며 실제 GPIO 검증은 Pi에서 수행합니다.

## Raspberry Pi ADS1115 설정

센서 전원 MOSFET의 게이트는 BCM GPIO15(물리 핀 10)로 제어합니다. 프로그램의 GPIO 초기화 시 출력 HIGH로 켜서 실행 중 유지하고, 정상 종료 시 `gpioTerminate()` 전에 LOW로 끕니다. 강제 종료(`SIGKILL`)나 충돌 시에는 종료 코드 실행을 보장할 수 없습니다.

프로그램은 I2C 버스 1의 두 ADC를 사용합니다.

| 주소 | 채널 A0 / A1 / A2 / A3 |
| --- | --- |
| `0x48` | 배터리 전압 / 연료 펌프 / AB 펌프 / 오일 펌프 |
| `0x49` | 연료 압력 / AB 압력 / 입구 압력 / 오일 압력 |

연료(A1), AB(A2), 오일(A3) 펌프는 공통으로 `스로틀(%) = clamp(V / 2.73 × 100, 0, 100)`을 사용합니다. 오일 최대 입력 약 2.74 V를 기준으로 끝점 여유를 둔 2.73 V 보정을 연료와 AB에도 우선 적용했습니다. AB는 미연결 상태로 확인되었으며, 미연결 ADC 입력은 부유 전압 때문에 0이 아닌 값을 읽을 수 있습니다.

압력은 LEFOO T2000 2선식 4~20 mA 신호를 변환한 입력 0.6~3.0 V에 맞춰 `압력(bar) = clamp((V - 0.6) / 2.4 × 10, 0, 10)`으로 계산합니다. 이 전압 범위는 150 Ω 저항으로 전류를 전압으로 변환할 때에 해당합니다. 압력 범위는 기존 0~10 bar를 유지하며, 실제 센서 명판의 정격 범위가 다르면 별도 변경이 필요합니다.

프로그램 시작 시 펌프(`0x48` A1~A3)와 압력(`0x49` A0~A3)을 ±4.096 V 범위(`scale=0.125` mV/LSB)로 설정하고 실제 scale을 다시 읽습니다. 기존 ±2.048 V 범위에서는 입력이 포화됩니다. 범위 변경에는 해당 IIO scale 파일의 쓰기 권한이 필요하며, 설정 실패 시 해당 채널을 유효한 측정값으로 표시하지 않습니다. 배터리 채널의 범위는 변경하지 않습니다.

`config/ads1115-config.txt`의 항목을 `/boot/firmware/config.txt`의 적용되는 구역에 반영합니다. 구형 OS는 `/boot/config.txt`를 사용할 수 있습니다. 전체 부팅 설정 파일을 예시 파일로 덮어쓰지 말고, 기존 ADS1115 항목을 교체한 뒤 재부팅합니다.

실제 장치가 `0x48`인데 overlay가 `0x4b`이면 드라이버 초기화가 실패하고, 프로그램이 필요한 IIO 파일을 찾지 못해 배터리 전압과 펌프 출력에 `nan`이 표시됩니다. 이 경우 `dtoverlay=ads1115,addr=0x4b`를 `dtoverlay=ads1115,addr=0x48`로 교체합니다. 코드의 `ADDR_SCL` 이름과 관계없이 현재 주소 값은 `0x48`입니다.

재부팅 후 확인:

```sh
sudo i2cdetect -y 1
ls -d /sys/bus/i2c/devices/1-0048/iio:device*
ls -d /sys/bus/i2c/devices/1-0049/iio:device*
cat /sys/bus/i2c/devices/1-0048/iio:device*/in_voltage[0-3]_raw
cat /sys/bus/i2c/devices/1-0049/iio:device*/in_voltage[0-3]_raw
```

각 주소에 IIO 장치와 채널 0~3의 `raw`, `scale` 파일이 있어야 합니다. 드라이버가 주소를 사용 중이면 스캔에는 `UU`로 표시됩니다. 전압은 `raw × scale / 1000` V입니다. 읽기에 성공해도 값이 거의 0 V라면 센서 전원, 입력 신호와 공통 GND를 별도로 확인합니다.
