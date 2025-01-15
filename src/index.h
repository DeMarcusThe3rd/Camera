const char HTML_PAGE[] PROGMEM = R"=====(

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

.camera-window {
    width: 100%;
    max-width: 600px;
    height: 400px;
    border: 2px solid #ccc;
    border-radius: 8px;
    background-color: #000;
    position: relative;
    margin-top: 20px;
    display: flex;
    justify-content: center;
    align-items: center;
    overflow: hidden;
}

.camera-window video {
    width: 100%;
    height: 100%;
    object-fit: cover;
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

/* Views */
.view {
    display: none; /* Hide both views by default */
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
            <h1><i class='bx bxs-camera'></i> EGS x EE Camera Trap</h1>
            <!-- Navigation buttons below the header -->
            <div class="nav-buttons">
                <button class="nav-btn" id="view-camera-btn">View Camera</button>
                <button class="nav-btn" id="download-btn">Download</button>
            </div>
            <!-- Views to switch between -->
            <div class="view" id="view-camera">
                <div class="camera-window">
                    <!-- Camera feed (Embedded video stream from ESP32) -->
                    <video id="camera-feed" autoplay></video>
                </div>
            </div>
            <div class="view" id="download-view">
                <h2>Download your data</h2>
                <!-- Add download content here (e.g., links, buttons to download files, etc.) -->
                <button>Download Photos</button>
                <button>Download Settings</button>
            </div>
        </div>
    </div>

    <script>
        // Get buttons and views
        const viewCameraBtn = document.getElementById('view-camera-btn');
        const downloadBtn = document.getElementById('download-btn');
        const viewCamera = document.getElementById('view-camera');
        const downloadView = document.getElementById('download-view');
        const cameraFeed = document.getElementById('camera-feed');

        // Set the camera feed source (MJPEG stream from ESP32)
        cameraFeed.src = "http://172.20.10.2/stream"; // Update with your ESP32 stream URL

        // Initially, show the camera view
        viewCamera.style.display = 'block';
        downloadView.style.display = 'none';

        // Event listeners for buttons
        viewCameraBtn.addEventListener('click', function () {
            viewCamera.style.display = 'block';
            downloadView.style.display = 'none';
        });

        downloadBtn.addEventListener('click', function () {
            downloadView.style.display = 'block';
            viewCamera.style.display = 'none';
        });
    </script>
</body>

</html>

)=====";
