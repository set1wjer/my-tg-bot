# myTgBot
My first project on GitHub, just example Telegram Bot with some functions, the main functions is finances


## Requirements
- C++ compiler (g++, clang, MSVC)
- CMake >= 3.10
- Libraries:
  - Boost (for X functionality)
  - OpenSSL (for TLS/HTTPS)
  - cURL (for HTTP requests)


## Build
### Visual Studio 
1. Clone the repository:
   ```bash
   git clone https://github.com/set1wjer/my-tg-bot.git 
   ```
2. Open Visual Studio.
3. Open project or solution.
4. Find my-tg-bot/CMakeLists.txt and open it.
5. Replace bot token in `CMakeLists.json`
   ```cmake
   "TG_BOT_TOKEN": "YOUR TG BOT TOKEN"
   ```

### Another
1. Clone the repository:
   ```bash
   git clone https://github.com/set1wjer/my-tg-bot.git
   cd my-tg-bot
   ```
2. Open `CMakeLists.json`
   Find the 15 line:
   ```cmake
   "TG_BOT_TOKEN": "YOUR TG BOT TOKEN"
   ```
   **Replace _YOUR TG BOT TOKEN_ with your actual bot token**
   
3. Create a build directory and go into:
   ```bash
   mkdir build
   cd build
   ```
4. Run and build CMake:
   ```bash
   cmake ..
   cmake --build .
   ```

6. Place sqlite3.dll and TgBot.dll in Folder with echobot.exe
   
7. Run the bot:
   ```bash
    ./echobot    # Linux/macOS
    echobot.exe  # Windows
   ```

   
   
