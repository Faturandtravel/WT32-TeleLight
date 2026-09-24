# WT32-ETH01 Telegram Light Control

An Arduino/PlatformIO project for controlling a light or electrical device through a relay connected to the **WT32-ETH01**, controlled by a Telegram bot over an Ethernet connection.

## Features

- Network connectivity through Ethernet LAN8720.
- Relay control through Telegram.
- Active-low relay logic.
- Only the configured `CHAT_ID` can control the relay.
- Relay status monitoring through Telegram.

## Telegram Commands

| Command | Function |
| --- | --- |
| `/on` | Turn the relay/light on |
| `/off` | Turn the relay/light off |
| `/status` | Display the current relay status |

## Hardware Connections

| Component | WT32-ETH01 GPIO |
| --- | ---: |
| Relay | GPIO 14 |
| Ethernet MDC | GPIO 23 |
| Ethernet MDIO | GPIO 18 |
| Ethernet PHY power | GPIO 16 |
| Ethernet clock | GPIO 0, `ETH_CLOCK_GPIO0_IN` |

PHY configuration:

- PHY address: `1`
- PHY type: `LAN8720`

Make sure the relay module is compatible with the voltage and current of the load. Disconnect the power source before wiring an AC load.

## Prerequisites

1. Install [Visual Studio Code](https://code.visualstudio.com/).
2. Install the [PlatformIO IDE](https://platformio.org/install/ide?gclid=) extension.
3. Create a Telegram bot using [@BotFather](https://t.me/BotFather) and save the bot token.
4. Get the authorized Telegram `CHAT_ID`.
5. Connect the WT32-ETH01 to an Ethernet network.

## Configuration

Edit the following values in `src/main.cpp` before uploading:

```cpp
#define BOT_TOKEN "TOKEN_BOT_TELEGRAM"
#define CHAT_ID "CHAT_ID_TELEGRAM"
```

Do not share the bot token or commit it to a public repository. If a token stored in the source has been exposed, revoke it through BotFather and generate a new token.

## Build and Upload

Run the following commands from the project root:

```bash
pio run
pio run --target upload
pio device monitor
```

The serial monitor uses a baud rate of `115200`.

After the board starts, open the serial monitor. The program displays the Ethernet status and IP address when the connection succeeds. The Telegram bot starts processing messages after Ethernet receives an IP address.

## Project Structure

```text
.
├── platformio.ini    # PlatformIO configuration and dependencies
├── src/
│   └── main.cpp      # Ethernet, Telegram, and relay logic
├── include/          # Project headers
├── lib/              # Local libraries
└── test/             # PlatformIO tests
```
## Dependencies
## Dependency
Dependencies are managed by PlatformIO through `platformio.ini`:
Dependency dikelola oleh PlatformIO melalui `platformio.ini`:

- [Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- [ArduinoJson](https://arduinojson.org/) `^6.21.3`
- Arduino framework for ESP32

## Implementation Notes

- The relay is initialized in the off state when the board starts (`HIGH`).
- Since the relay is active-low, `LOW` means on and `HIGH` means off.
- Telegram TLS uses `client.setInsecure()`. This simplifies the connection, but does not validate the server certificate and should be hardened for production use.
- Messages from other Telegram chats are ignored based on the `CHAT_ID` value.
