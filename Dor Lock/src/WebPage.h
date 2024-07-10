const char WebPage[] PROGMEM =
    R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Fingerprint Settings</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: #f4f4f9;
        margin: 0;
        padding: 0;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
      }

      .card {
        background: white;
        border-radius: 8px;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        padding: 20px;
        max-width: 400px;
        width: 100%;
      }

      .card-body {
        display: flex;
        flex-direction: column;
      }

      .card-body label {
        margin-bottom: 5px;
        font-weight: bold;
      }

      .card-body input {
        margin-bottom: 15px;
        padding: 10px;
        border: 1px solid #ccc;
        border-radius: 4px;
        font-size: 16px;
      }

      .card-body input:focus {
        border-color: #007bff;
        outline: none;
        box-shadow: 0 0 5px rgba(0, 123, 255, 0.5);
      }

      .card-body input[type="text"] {
        width: calc(100% - 22px); /* Adjust for padding and border */
      }

      .card-body button {
        padding: 10px;
        background-color: #007bff;
        color: white;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        font-size: 16px;
        font-weight: bold;
      }

      .card-body button:hover {
        background-color: #0056b3;
      }
    </style>
  </head>
  <body>
    <form id="formInput">
    <div class="card">
      <div class="card-body">
        <label for="ssid">SSID</label>
        <input type="text" name="ssid" id="ssid" required />
        <label for="password">Password</label>
        <input type="text" name="password" id="password" required />
        <label for="url">Url</label>
        <input type="url" name="url" id="url" required />
        <label for="token">Device Token</label>
        <input type="text" name="token" id="token" required />
        <button type="submit">Submit</button>
      </div>
    </div>
    </form>
    <script>
      document.getElementById('formInput').addEventListener('submit', function(e) {
        e.preventDefault();
        var formData = new FormData(this);
        fetch('/save', {
          method: 'POST',
          body: formData
        })
        .then(response => response.text())
        .then(data => {
          alert('Settings saved. ESP32 will restart.');
          // Anda bisa menambahkan logika tambahan di sini jika diperlukan
        })
        .catch(error => {
          console.error('Error:', error);
          alert('An error occurred. Please try again.');
        });
      });
    </script>
  </body>
</html>
)rawliteral";