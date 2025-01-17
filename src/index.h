const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1>ESP32-CAM Web Server</h1>
  <div class="left-section" >
        <form method='get' action='settings'>
            <h2>Camera Trap Setup</h2>
            <label for="date">Date: </label>
            <input id='date' name='date' type='date'>
            <label for="time">Time: </label>
            <input id='time' name='time' type='time'>
            <label for="numPics">Num Photos: </label>
            <input id='numPics' name='numPics' type='number' min="1">
            <label for="delayTime">Delay Time(minutes): </label>
            <input id='delayTime' name='delayTime' type='number' min="0.5" step="0.5">
            <input type='submit'>
        </form>
    </div>
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
  <button id="download-button" class="button" onclick="downloadImage()">
    Download
    <a id="download" style="display:none;"></a>
    </button>

  <script>
    function loadFileList() {
      fetch('/list')
        .then(response => response.json())
        .then(data => {
          const fileSelect = document.getElementById('file');
          fileSelect.innerHTML = '<option disabled selected value style="display: none;">Select File</option>';
          
          if (data.folders) {
            data.folders.forEach(folder => {
            
          if (folder.name === "System Volume Information") {
            return;
          }
              // Add folder name as a group label
              const optGroup = document.createElement('optgroup');
              optGroup.label = folder.name;

              // Add files under the folder
              folder.files.forEach(file => {
                const option = document.createElement('option');
                option.value = `${folder.name}/${file}`; // Full path
                option.textContent = `${folder.name}/${file}`; // Display folder/file
                optGroup.appendChild(option);
              });

              fileSelect.appendChild(optGroup);
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

  function downloadImage() {
    const link = document.getElementById("download");
    const select = document.getElementById("file");
    const uri = "/image?file=" + encodeURIComponent(select.value);

    if (select.value !== "") {
      link.download = decodeURIComponent(select.value).replace(/\//g, " ");
      link.href = uri;
      link.click();
    }
  }

    
    // Load file list when the page is loaded
    window.onload = loadFileList;
  </script>
</body>
</html>
)rawliteral";
