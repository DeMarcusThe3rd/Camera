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
  <div class="header">
    <label for="file-name">Choose File:</label>
    <select id="file" name="file-name" onchange="selectFile()">
      <option disabled selected value style="display: none;">Select File</option>
    </select>
  </div>
  <div id="image-container">
    <h3>Selected Image:</h3>
    <img id="selected-image" src="" alt="Image will be displayed here" style="max-width:100%;"/>
  </div>

  <script>
    function loadFileList() {
      fetch('/list')
        .then(response => response.json())
        .then(data => {
          const fileSelect = document.getElementById('file');
          fileSelect.innerHTML = '<option disabled selected value style="display: none;">Select File</option>';
          if (data.files) {
            data.files.forEach(file => {
              const option = document.createElement('option');
              option.value = file;
              option.textContent = file;
              fileSelect.appendChild(option);
            });
          }
        });
    }

    function selectFile() {
      const fileSelect = document.getElementById('file');
      const selectedFile = fileSelect.value;
      const img = document.getElementById('selected-image');
      img.src = `/image?file=${encodeURIComponent(selectedFile)}`;
    }

    // Load file list when the page is loaded
    window.onload = loadFileList;
  </script>
</body>
</html>
)rawliteral";
