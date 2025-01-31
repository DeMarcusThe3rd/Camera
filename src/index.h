const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Camera Trap Setup</title>
    <link href='https://unpkg.com/boxicons@2.1.4/css/boxicons.min.css' rel='stylesheet'>
    <style>
    * {
    font-family: "Arial", sans-serif;
    box-sizing: border-box;
    margin: 0;
    padding: 0;
}

body {
    display: flex;
    min-height: 100vh;
    margin: 0;
}

/* Main container layout */
.main-container {
    display: flex;
    flex: 1;
}

/* Sidebar Styles */
.sidebar {
    background-color: #3f51b5;
    color: white;
    padding: 2rem;
    width: 250px;
    min-width: 250px;
    box-shadow: 7px 0px 27px -10px rgba(0, 0, 0, 0.75);
    flex-shrink: 0;
}

/* Content Styles */
.content {
    flex: 1;
    padding: 2rem;
    background-color: #f4f4f9;
    display: flex;
    flex-direction: column;
    align-items: flex-start;
    justify-content: flex-start;
}

/* Title Style */
.content h1 {
    font-size: 2rem;
    margin-bottom: 20px;
}

/* Navigation Button Container */
.nav-buttons {
    margin-bottom: 20px;
}

/* Individual Navigation Buttons */
.nav-btn {
    background-color: #3f51b5;
    color: white;
    border: none;
    padding: 10px 20px;
    margin-right: 10px;
    border-radius: 5px;
    font-size: 1rem;
    cursor: pointer;
    transition: background-color 0.3s;
}

.nav-btn:hover {
    background-color: #1a237e;
}

/* Sidebar Form Styles */
.sidebar h2 {
    margin-bottom: 1.5rem;
}

.sidebar label {
    display: block;
    margin: 0.5rem 0;
}

.sidebar input {
    width: calc(100% - 1rem);
    padding: 0.5rem;
    margin: 0.5rem 0;
    border: none;
    border-radius: 0.25rem;
}

.sidebar input[type="submit"] {
    background-color: #E8EAF6;
    color: #3f51b5;
    font-weight: bold;
    cursor: pointer;
    transition: background-color 0.3s, color 0.3s;
}

.sidebar input[type="submit"]:hover {
    background-color: #1A237E;
    color: white;
}

.download-button {
    background-color: #3f51b5; /* Primary color */
    color: white;
    border: none;
    padding: 10px 20px;
    margin-right: 10px;
    border-radius: 5px;
    font-size: 1rem;
    cursor: pointer;
    transition: background-color 0.3s, color 0.3s;
}

.download-button:hover {
    background-color: #1a237e; /* Darker shade for hover */
    color: white;
}

/* Views */
.view {
    display: none; /* Hide both views by default */
}

#camera-window {
    border: 2px solid #3f51b5; /* Adjust the color and thickness as needed */
    border-radius: 8px;       /* Optional: Rounded corners */
    padding: 10px;            /* Optional: Space between the content and border */
    box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2); /* Optional: Add a subtle shadow */
    margin: auto;             /* Center the container horizontally */
}

#image-container {
    border: 2px solid #3f51b5; /* Adjust the color and thickness as needed */
    border-radius: 8px;       /* Optional: Rounded corners */
    padding: 10px;            /* Optional: Space between the content and border */
    box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2); /* Optional: Add a subtle shadow */
    margin: auto;             /* Center the container horizontally */
}

/* Media Queries for Responsiveness */
@media (max-width: 768px) {
    body {
        display: flex;
        min-height: 100vh;
        margin: 0;
    }

    .main-container {
        display: flex;
        flex: 1;
    }

    .sidebar {
        background-color: #3f51b5;
        color: white;
        padding: 2rem;
        width: 250px;
        min-width: 250px;
        box-shadow: 7px 0px 27px -10px rgba(0, 0, 0, 0.75);
        flex-shrink: 0;
    }

    .content {
        flex: 1;
        padding: 2rem;
        background-color: #f4f4f9;
        display: flex;
        flex-direction: column;
        align-items: flex-start;
        justify-content: flex-start;
    }
}

    </style>
</head>

<body>
    <div class="main-container">
        <div class="sidebar">
            <!-- Sidebar content (Setup form) -->
            <form class="setup-form" method="get" action="settings">
                <h2>Camera Trap Setup</h2>
                <label for="date">Date:</label>
                <input id="date" name="date" type="date" required>
                <label for="time">Time:</label>
                <input id="time" name="time" type="time" required>
                <label for="numPics">Number of Photos:</label>
                <input id="numPics" name="numPics" type="number" min="1" required>
                <label for="delayTime">Delay Time (minutes):</label>
                <input id="delayTime" name="delayTime" type="number" min="0.5" step="0.5" required>
                <input type="submit" value="Submit">
            </form>
        </div>
        <div class="content">
            <h1>
                <button onclick="fetch('/capture').then(response => response.text()).then(alert)" style="border: none; background: none; padding: 0; cursor: pointer;">
                  <i class='bx bxs-camera' style="font-size: 30px; color: black;"></i>
                </button> 
                EGS x EE Camera Trap
            </h1>
            <!-- Navigation buttons below the header -->
            <div class="nav-buttons">
                <button class="nav-btn" id="view-camera-btn">View Camera</button>
                <button class="nav-btn" id="download-btn">Download</button>
            </div>
            <!-- Views to switch between -->
            <div class="view" id="view-camera">
                <div class="camera-window">
                </div>
            </div>
            <div class="view" id="download-view">
                <div class="selectfile">
                    <label for="file-name">Choose File:</label>
                    <select id="file" name="file-name" onchange="selectFile()">
                      <option disabled selected value style="display: none;">Select File</option>
                    </select>
                </div>
                <br>
                  <div id="image-container">
                    <h3>Selected Image:</h3>
                    <br>
                    <img id="selected-image" src="" alt="Image will be displayed here"/>
                    </div>
                    <br>
                  <button id="download-button" class="download-button" onclick="downloadImage()">
                    Download
                    <a id="download" style="display:none;"></a>
                    </button>
            </div>
        </div>
    </div>

    <script>
    //Switch between views
    document.getElementById('view-camera-btn').addEventListener('click', () => {
        document.getElementById('view-camera').style.display = 'block';
        document.getElementById('download-view').style.display = 'none';
    });

    document.getElementById('download-btn').addEventListener('click', () => {
        document.getElementById('view-camera').style.display = 'none';
        document.getElementById('download-view').style.display = 'block';
    });

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
