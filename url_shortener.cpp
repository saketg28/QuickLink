#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include "crow_all.h"
using namespace std;

class URLShortener {
private:
    unordered_map<string, string> urlMap;
    const string filename = "url_mapping.dat";

    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \r\n\t");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \r\n\t");
        return str.substr(first, (last - first + 1));
    }

    string generateShortId(const string& longUrl) {
        size_t h = hash<string>{}(longUrl + to_string(time(0)));
        stringstream ss;
        ss << hex << setw(6) << setfill('0') << (h & 0xFFFFFF);
        return ss.str();
    }

public:
    URLShortener() {
        ifstream infile(filename);
        if (infile.is_open()) {
            string shortUrl, longUrl;
            while (getline(infile, shortUrl) && getline(infile, longUrl)) {
                urlMap[trim(shortUrl)] = trim(longUrl);
            }
            infile.close();
        }
    }

    string shortenUrl(const string& longUrl) {
        for (auto& pair : urlMap) {
            if (pair.second == longUrl) return pair.first;
        }

        string shortId = generateShortId(longUrl);

        int salt = 0;
        while (urlMap.find(shortId) != urlMap.end()) {
            shortId = generateShortId(longUrl + to_string(salt++));
        }

        urlMap[shortId] = longUrl;

        ofstream outfile(filename, ios::app);
        if (outfile.is_open()) {
            outfile << shortId << "\n" << longUrl << "\n";
            outfile.close();
        }

        return shortId;
    }

    string retrieveUrl(const string& shortId) {
        auto it = urlMap.find(shortId);
        if (it != urlMap.end()) return it->second;
        return "";
    }
};

// Build the HTML page as a function to avoid raw string issues
string buildHtmlPage() {
    string html = "";
    html += "<!DOCTYPE html>\n";
    html += "<html lang=\"en\">\n";
    html += "<head>\n";
    html += "<meta charset=\"UTF-8\"/>\n";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>\n";
    html += "<title>URL Shortener</title>\n";
    html += "<style>\n";
    html += "* { box-sizing: border-box; margin: 0; padding: 0; }\n";
    html += "body {\n";
    html += "  font-family: 'Segoe UI', sans-serif;\n";
    html += "  min-height: 100vh;\n";
    html += "  display: flex;\n";
    html += "  align-items: center;\n";
    html += "  justify-content: center;\n";
    html += "  background: #0f0f0f;\n";
    html += "  color: #f0f0f0;\n";
    html += "}\n";
    html += ".card {\n";
    html += "  background: #1a1a1a;\n";
    html += "  border: 1px solid #2a2a2a;\n";
    html += "  border-radius: 16px;\n";
    html += "  padding: 40px;\n";
    html += "  width: 100%;\n";
    html += "  max-width: 520px;\n";
    html += "  box-shadow: 0 8px 32px rgba(0,0,0,0.4);\n";
    html += "}\n";
    html += "h2 { font-size: 1.6rem; margin-bottom: 8px; color: #ffffff; }\n";
    html += "p.subtitle { color: #888; font-size: 0.9rem; margin-bottom: 28px; }\n";
    html += ".input-row { display: flex; gap: 10px; margin-bottom: 20px; }\n";
    html += "input {\n";
    html += "  flex: 1; padding: 12px 16px; border-radius: 10px;\n";
    html += "  border: 1px solid #333; background: #111; color: #f0f0f0;\n";
    html += "  font-size: 0.95rem; outline: none; transition: border 0.2s;\n";
    html += "}\n";
    html += "input:focus { border-color: #555; }\n";
    html += "button {\n";
    html += "  padding: 12px 20px; border-radius: 10px; border: none;\n";
    html += "  background: #6c63ff; color: white; font-size: 0.95rem;\n";
    html += "  cursor: pointer; transition: background 0.2s; white-space: nowrap;\n";
    html += "}\n";
    html += "button:hover { background: #574fd6; }\n";
    html += "button:disabled { background: #444; cursor: not-allowed; }\n";
    html += ".result-box {\n";
    html += "  display: none; background: #111; border: 1px solid #2a2a2a;\n";
    html += "  border-radius: 10px; padding: 16px; margin-top: 10px;\n";
    html += "}\n";
    html += ".result-box.visible { display: block; }\n";
    html += ".result-box p { font-size: 0.8rem; color: #888; margin-bottom: 8px; }\n";
    html += ".result-row { display: flex; align-items: center; gap: 10px; }\n";
    html += ".result-row a {\n";
    html += "  color: #6c63ff; text-decoration: none;\n";
    html += "  font-size: 1rem; word-break: break-all; flex: 1;\n";
    html += "}\n";
    html += ".result-row a:hover { text-decoration: underline; }\n";
    html += ".copy-btn {\n";
    html += "  padding: 7px 14px; font-size: 0.8rem; background: #2a2a2a;\n";
    html += "  border-radius: 8px; border: 1px solid #333; color: #ccc;\n";
    html += "  cursor: pointer; transition: background 0.2s; white-space: nowrap;\n";
    html += "}\n";
    html += ".copy-btn:hover { background: #333; }\n";
    html += ".error { color: #ff6b6b; font-size: 0.88rem; margin-top: 10px; display: none; }\n";
    html += ".error.visible { display: block; }\n";
    html += ".spinner {\n";
    html += "  display: inline-block; width: 14px; height: 14px;\n";
    html += "  border: 2px solid rgba(255,255,255,0.2);\n";
    html += "  border-top-color: #fff; border-radius: 50%;\n";
    html += "  animation: spin 0.6s linear infinite;\n";
    html += "  vertical-align: middle; margin-right: 6px;\n";
    html += "}\n";
    html += "@keyframes spin { to { transform: rotate(360deg); } }\n";
    html += "</style>\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "<div class=\"card\">\n";
    html += "  <h2>URL Shortener</h2>\n";
    html += "  <p class=\"subtitle\">Paste a long URL and get a short one instantly.</p>\n";
    html += "  <div class=\"input-row\">\n";
    html += "    <input id=\"urlInput\" type=\"text\" placeholder=\"https://example.com/very/long/url\" />\n";
    html += "    <button id=\"shortenBtn\" onclick=\"doShorten()\">Shorten</button>\n";
    html += "  </div>\n";
    html += "  <div class=\"error\" id=\"errorMsg\">Please enter a valid URL.</div>\n";
    html += "  <div class=\"result-box\" id=\"resultBox\">\n";
    html += "    <p>Your shortened URL:</p>\n";
    html += "    <div class=\"result-row\">\n";
    html += "      <a id=\"shortLink\" href=\"#\" target=\"_blank\"></a>\n";
    html += "      <button class=\"copy-btn\" onclick=\"copyLink()\">Copy</button>\n";
    html += "    </div>\n";
    html += "  </div>\n";
    html += "</div>\n";
    html += "<script>\n";
    // JS uses doShorten() instead of shorten() to avoid any name conflicts
    html += "async function doShorten() {\n";
    html += "  var input = document.getElementById('urlInput');\n";
    html += "  var btn = document.getElementById('shortenBtn');\n";
    html += "  var resultBox = document.getElementById('resultBox');\n";
    html += "  var errorMsg = document.getElementById('errorMsg');\n";
    html += "  var shortLink = document.getElementById('shortLink');\n";
    html += "  var url = input.value.trim();\n";
    html += "  resultBox.classList.remove('visible');\n";
    html += "  errorMsg.classList.remove('visible');\n";
    html += "  if (!url) {\n";
    html += "    errorMsg.textContent = 'Please enter a URL.';\n";
    html += "    errorMsg.classList.add('visible');\n";
    html += "    return;\n";
    html += "  }\n";
    html += "  if (url.indexOf('http://') !== 0 && url.indexOf('https://') !== 0) {\n";
    html += "    url = 'https://' + url;\n";
    html += "  }\n";
    html += "  btn.disabled = true;\n";
    html += "  btn.textContent = 'Shortening...';\n";
    html += "  try {\n";
    html += "    var res = await fetch('/shorten/' + url);\n";
    html += "    var text = await res.text();\n";
    // Extract URL by splitting on space — avoids regex entirely
    html += "    var parts = text.split(' ');\n";
    html += "    var shortUrl = parts[parts.length - 1].trim();\n";
    html += "    if (shortUrl.indexOf('http') === 0) {\n";
    html += "      shortLink.href = shortUrl;\n";
    html += "      shortLink.textContent = shortUrl;\n";
    html += "      resultBox.classList.add('visible');\n";
    html += "    } else {\n";
    html += "      errorMsg.textContent = 'Something went wrong. Try again.';\n";
    html += "      errorMsg.classList.add('visible');\n";
    html += "    }\n";
    html += "  } catch(e) {\n";
    html += "    errorMsg.textContent = 'Could not reach server. Is it running?';\n";
    html += "    errorMsg.classList.add('visible');\n";
    html += "  }\n";
    html += "  btn.disabled = false;\n";
    html += "  btn.textContent = 'Shorten';\n";
    html += "}\n";
    html += "function copyLink() {\n";
    html += "  var link = document.getElementById('shortLink').textContent;\n";
    html += "  navigator.clipboard.writeText(link).then(function() {\n";
    html += "    var btn = document.querySelector('.copy-btn');\n";
    html += "    btn.textContent = 'Copied!';\n";
    html += "    setTimeout(function() { btn.textContent = 'Copy'; }, 2000);\n";
    html += "  });\n";
    html += "}\n";
    html += "document.addEventListener('DOMContentLoaded', function() {\n";
    html += "  document.getElementById('urlInput').addEventListener('keydown', function(e) {\n";
    html += "    if (e.key === 'Enter') doShorten();\n";
    html += "  });\n";
    html += "});\n";
    html += "</script>\n";
    html += "</body>\n";
    html += "</html>\n";
    return html;
}

int main() {
    crow::SimpleApp app;
    URLShortener shortener;

    // ─── HTML Frontend ───────────────────────────────────────────────
    CROW_ROUTE(app, "/")
    ([](const crow::request& req, crow::response& res){
        res.set_header("Content-Type", "text/html");
        res.code = 200;
        res.body = buildHtmlPage();
        res.end();
    });

    // ─── Shorten Route ───────────────────────────────────────────────
    CROW_ROUTE(app, "/shorten/<path>")
    ([&shortener](const crow::request& req, crow::response& res, string urlPath){
        string longUrl = urlPath;

        // Restore https:// collapsed by browser to https:/
        if (longUrl.size() >= 8 && longUrl.substr(0, 8) == "https://") {
            // already fine
        } else if (longUrl.size() >= 7 && longUrl.substr(0, 7) == "https:/") {
            longUrl = "https://" + longUrl.substr(7);
        } else if (longUrl.size() >= 6 && longUrl.substr(0, 6) == "http:/") {
            longUrl = "http://" + longUrl.substr(6);
        }

        string shortId = shortener.shortenUrl(longUrl);
        string shortUrl = "http://localhost:8080/r/" + shortId;

        res.set_header("Content-Type", "text/plain");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.code = 200;
        res.body = "Short URL: " + shortUrl;
        res.end();
    });

    // ─── Redirect Route ──────────────────────────────────────────────
    CROW_ROUTE(app, "/r/<string>")
    ([&shortener](const crow::request& req, crow::response& res, string shortId){
        cout << "DEBUG: Looking up ID: " << shortId << endl;

        string originalUrl = shortener.retrieveUrl(shortId);

        if (originalUrl.empty()) {
            res.code = 404;
            res.set_header("Content-Type", "text/html");
            res.body = "<h2>404 - Short URL not found</h2>";
            res.end();
            return;
        }

        if (originalUrl.find("http://") != 0 && originalUrl.find("https://") != 0) {
            originalUrl = "http://" + originalUrl;
        }

        cout << "DEBUG: Redirecting to: " << originalUrl << endl;

        res.code = 301;
        res.set_header("Location", originalUrl);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.end();
    });

    cout << "Server running at http://localhost:8080" << endl;
    app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
}