# qt-sensors-bmp280-i2c
Qt Sensors Plugin &amp; Library For BMP280

# Installation

Run the following commands in a terminal session to download and setup the plugin
```console
git clone https://github.com/DigitalArtifex/qt-sensors-bmp280-i2c.git
mkdir qt-sensors-bmp280-i2c/build
cd qt-sensors-bmp280-i2c/build
```

Next, we will need to configure the project against your Qt Installation by executing `qt-cmake` located in `<Qt Path>/<Version>/<Arch>/bin`. Typically, the Qt Path will be `~/Qt`, but can be `/opt/Qt` if installed with sudo, or elsewhere if configured differently. The example assumes Qt Path to be `/opt/Qt`, Qt Version to be `6.9.0` and the arch to be `gcc_arm64`

```
/opt/Qt/6.9.0/gcc_arm64/bin/qt-cmake -S ../ -B ./
```

Once configured, we will switch to the system provided cmake. If Qt is installed to `~/` there is no need to execute `cmake install` with sudo

```
cmake --build ./
sudo cmake --install ./
```

# Usage

## Adding the reference (CMake)

```cmake
target_link_libraries(
  MyApp
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sensors
  i2c
  qt${QT_VERSION_MAJOR}-sensors-bmp280-i2c
)
```

## Using in code

```cpp
QBMP280 m_bmp280 = new QBMP280(this);
m_bmp280->connectToBackend();
m_bmp280->setDataRate(1);
m_bmp280->setBus("/dev/i2c-1");
m_bmp280->setAddress(0x76);
m_bmp280->setPowerMode(QBMP280::PowerMode::UltraLowPower);
m_bmp280->setFilterMode(QBMP280::Filter::X2);

connect(m_bmp280, SIGNAL(readingChanged()), this, SLOT(onSensorReadingAvailable()));
m_bmp280->start();
```
