# C++ URL Shortener

A high-performance, lightweight URL shortener service built with C++17, utilizing the [Crow](https://crowcpp.org/) web framework and [Asio](https://think-async.com/Asio/) for networking. This project features a responsive, dark-themed HTML frontend embedded directly into the binary, providing a seamless user experience.

## Features

*   **Embedded Frontend:** A custom HTML/CSS/JS interface is served directly by the C++ backend.
*   **Persistent Storage:** Mappings are saved to `url_mapping.dat` using a file-based storage system, ensuring data persists across server restarts.
*   **HTTP Redirection:** Efficiently handles requests using HTTP 301 (Moved Permanently) status codes.
*   **Collision Handling:** Implements a salt-based generation algorithm to prevent ID clashes.
*   **Performance:** Built on modern C++17 standards, utilizing features like `std::string_view` and efficient memory handling.

## System Requirements

*   **Compiler:** A C++17 compliant compiler (GCC 7+, Clang 5+, or MSVC 2017+).
*   **OS:** Windows (tested with MinGW-w64) or Linux/macOS.
*   **Network Libraries:** Windows requires `ws2_32` and `mswsock`.

## Prerequisites

1.  **Crow Framework:** Download `crow_all.h` from the [Crow GitHub releases](https://github.com/CrowCpp/Crow/releases).
2.  **Asio:** Download the standalone [Asio library](https://sourceforge.net/projects/asio/files/asio/).

## Setup & Compilation

### 1. Project Structure
Ensure your project folder is organized as follows:
```text
/your-project-folder
├── url_shortner.cpp
├── crow_all.h
└── include/        <-- Asio header files (asio.hpp, asio/, etc.)
```

### 2. Compilation
Open your terminal in the project directory and run the following command. Note the `-std=c++17` flag, which is required for the modern features used in this project:

**Windows (MinGW):**
```bash
g++ -std=c++17 url_shortner.cpp -o shortener.exe -I include -lws2_32 -lmswsock
```

**Linux/macOS:**
```bash
g++ -std=c++17 url_shortner.cpp -o shortener -I include -lpthread
```

### 3. Execution
Run the compiled binary:
```bash
./shortener.exe
```
The server will start and listen on `http://localhost:8080`.

## How It Works

*   **Endpoint `/`:** Serves the interactive frontend.
*   **Endpoint `/shorten/<path>`:** Accepts a URL, generates a unique 6-character hash, stores the mapping in `url_mapping.dat`, and returns the shortened link.
*   **Endpoint `/r/<id>`:** The redirect route. It performs a lookup in the `unordered_map`, verifies the existence of the ID, and issues a 301 redirect to the original destination.
*   **Storage Logic:** The `URLShortener` class manages the `unordered_map` in-memory for lightning-fast lookups, while synchronizing to `url_mapping.dat` to ensure durability.

## Troubleshooting

*   **"404 Not Found" / Invalid Redirect:** Ensure you have deleted any old `url_mapping.dat` files before running the new code to prevent loading "poisoned" data with hidden carriage return characters (`\r`).
*   **"No such file or directory":** Verify that `crow_all.h` and the Asio `include` folder are in the same directory as `url_shortner.cpp`.
*   **Network Errors:** Ensure port `8080` is not being used by another application (e.g., another server or a hung instance of `shortener.exe`).

## 👤 Author
**Shubham Anand**

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
