# MXChip IoT DevKit sample code using IoT Plug and Play

This is the sample codes for MXChip IoT DevKit using IoT Plug and Play. Two samples currently we provide:

- [iotc_devkit](./iotc_devkit): Sample code to connect the DevKit to your Azure IoT Central application.

  > This sample is **archieved** and have already moved to a new place. You can find it on [https://github.com/Azure-Samples/mxchip-iot-devkit-pnp/](https://github.com/Azure-Samples/mxchip-iot-devkit-pnp/).

- [aics_devkit](./aics_devkit): Sample code to certify the DevKit on Azure IoT Certification Service.

## Sample: Connect the DevKit to your Azure IoT Central application

First, go to the [quickstart](https://docs.microsoft.com/azure/iot-central/howto-connect-devkit-pnp) tutorial to learn how to connect the DevKit to your Azure IoT Central application.

### Review the code

If you want to explore and modify the device code, follow these steps.

1. To download sample code for DevKit, run the following command on your desktop machine.

   ```cmd/sh
   git clone https://github.com/MXCHIP/IoTDevKit.git
   ```

    > If **git** is not installed in your development environment, you can download it from [https://git-scm.com/download](https://git-scm.com/download).

1. The source code is located in `pnp/iotc_devkit` folder.

To see how the telemetry is sent to the Azure IoT Central application, open the `sensor_interface.c` file.

- The function **SensorsInterface_Telemetry_SendAll** sends multiple telemetries including humidity, temperature, pressure, magnetometer, gyroscope and accelerometer .
- The functions **Sensors_Serialize{XXXX}** in `digitaltwin_serializer.c` create the telemetry payload, using **Sensors_Telemetry_Read{XXXX}** functions in `mxchip_iot_devkit_impl.c` to get data from the sensors on the DevKit device.

To see how property values are reported to the Azure IoT Central application, open the `deviceinfo_interface.c` file.

- The function **DeviceinfoInterface_Property_ReportAll** sends properties including manufacturer, device model, software version, operating system name, processor architecture, processor manufacturer, total storage and total memory.
- The functions in `digitaltwin_serializer.c` create the property payload.

To see how the DevKit device responds to commands for LED actions called from the IoT Central application, open the `leds_interface.c` file.

- The function **LedsInterface_Command_TurnOnLedCallback** processes the turnOnLed command. It uses **Leds_Command_TurnOnLed** function in `mxchip_iot_devkit_impl.c` to turn on the User LED.
- The function **LedsInterface_Command_BlinkCallback** processes the blink command. It uses **Leds_Command_Blink** function in `mxchip_iot_devkit_impl.c` to blink the RGB LED.

### Compile and upload code

If you plan to modify the code, you should follow these instructions to [prepare the development environment](https://docs.microsoft.com/azure/iot-hub/iot-hub-arduino-iot-devkit-az3166-get-started#prepare-the-development-environment) for your desktop operating system.

1. Configure Arduino extension to install latest DevKit. In VS Code, use **Ctrl+Shift+P** to open the command palette, enter and select **Preferences: Open User Settings**. Type **arduino** in the search bar and choose **Edit in settings.json**.

    ![Arduino setting](../docs/howto-connect-devkit-pnp/arduino-setting.png)

1. Add a line in `arduino.additionalUrls` section that points to the DevKit SDK that supports IoT Plug and Play.

    ```json
    https://raw.githubusercontent.com/VSChina/azureiotdevkit_tools/pnp/package_mxchip_board_index.json
    ```

    ![Arduino additional URL](../docs/howto-connect-devkit-pnp/add-url.png)

1. Use **Ctrl+Shift+P** to open the command palette, enter and select **Arduino: Board Manager**.

1. In the board manager page, choose **Refresh Package Indexes**.

1. After the refresh completed, enter **AZ3166** in the search bar. Select and install latest version (>= 1.9.9).

    ![Select DevKit SDK version](../docs/howto-connect-devkit-pnp/select-version.png)

    It takes sometimes to download and install the latest SDK.

1. Check the MXCHIP AZ3166 is shown as selected board in the status bar and serial port with STMicroelectronics is used.

   ![Select serial port](../docs/howto-connect-devkit-pnp/select-serialport.png)

   Your COM port may differ from the image. Click the serial port, choose the port with **STMicroelectronics** in command palette.

   ![Select COM](../docs/howto-connect-devkit-pnp/select-COM.png)

   If the board type is incorrect, or there is no board type selected, click the board type and select **MXCHIP AZ3166** in the window that pops up.

   ![Select board](../docs/howto-connect-devkit-pnp/select-board.png)

1. In `pnp/iotc_devkit` folder, open **iotc_devkit.code-workspace** in VS Code.

1. Use **Ctrl+Shift+P** to open the command palette, enter and select **Arduino: Upload** to compile the project and upload to the DevKit.

   In OUTPUT pane, you will see the compilation and upload progress.

   ![Arduino upload done](../docs/howto-connect-devkit-pnp/arduino-upload-done.png)

1. Click **Open Serial Monitor** button status bar, then you will see the messages sent from your DevKit in OUTPUT pane.

   ![Open serial monitor](../docs/howto-connect-devkit-pnp/open-serial-monitor.png)

   ![Serial monitor message](../docs/howto-connect-devkit-pnp/serial-message.png)

### Troubleshooting

1. Download a serial client such as [Tera Term](https://tera-term.en.lo4d.com/windows).

1. Connect the DevKit to your computer by USB.

1. Open Tera Term, select **serial**, and then expand the port. The device should appear as an STMicroelectronics device. Choose **STMicroelectronics STLink Virtual COM Port**. Choose OK.

    ![Select port](../docs/howto-connect-devkit-pnp/select-port.png)

1. Choose **Setup** on the menu bar, select **serial port**, and configure the connection speed to **115200** baud. Then choose OK to open the serial monitor.

    ![Configure speed](../docs/howto-connect-devkit-pnp/configure-speed.png)

1. You can see the output log in the Window.

    ![Serial message](../docs/howto-connect-devkit-pnp/serial-message.png)

### MXChip Device template details

A device created from the MXChip IoT DevKit device template has the following characteristics:

#### Telemetry

Field name|Units|Minimum|Maximum|Decimal places
-|-|-|-|-
humidity|%|0|100|0
temp|°C|-40|120|0
pressure|hPa|260|1260|0
magnetometerX|mgauss|-1000|1000|0
magnetometerY|mgauss|-1000|1000|0
magnetometerZ|mgauss|-1000|1000|0
accelerometerX|mg|-2000|2000|0
accelerometerY|mg|-2000|2000|0
accelerometerZ|mg|-2000|2000|0
gyroscopeX|mdps|-2000|2000|0
gyroscopeY|mdps|-2000|2000|0
gyroscopeZ|mdps|-2000|2000|0

#### Properties

Display name|Field name|Data type|Writable/Read only
-|-|-|-
Manufacturer|manufacturer|string|Read only
Device model|model|string|Read only
Software version|swVersion|string|Read only
Operating system name|osName|string|Read only
Processor architecture|processorArchitecture|string|Read only
Processor manufacturer|processorManufacturer|string|Read only
Total storage|totalStorage|long|Read only
Total memory|totalMemory|long|Read only
Fan Speed|fanSpeed|double|Writable
Voltage|voltage|double|Writable
Current|current|double|Writable
IR|irSwitch|boolean|Writable

#### Commands

Field name|Input field name|Input field type
-|-|-
blink|interval|long
turnOnLed|/|/
turnOffLed|/|/
echo|text|string
countdown|number|integer

## Sample: Certify the DevKit on Azure IoT Certification Service

[TBD]
