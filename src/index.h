const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
    <h1>ESP32-CAM Web Server</h1>
    <button onclick="fetch('/capture').then(response => response.text()).then(alert)">Take Photo</button>
    <p><a href='/'>Refresh File List</a></p>
    <ul>
    <!-- Dynamic content will be injected here -->
    </ul>
</body>
</html>)rawliteral";