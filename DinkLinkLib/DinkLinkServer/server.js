// Minimal Express + WebSocket server example.
// Requires: npm install express ws
const express = require('express');
const http = require('http');
const WebSocket = require('ws');

const app = express();
const port = 8080;

// Serve public folder
app.use(express.static('public'));

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

/* Basic message routing:
   - Dashboard clients send { type: "input", id, buttons, axes }
   - ESP32 clients should register by sending: { type: "register", id }
   - Server routes input messages to the ESP32 client with matching id
*/

const devices = new Map(); // id -> ws

wss.on('connection', (ws) => {
  ws.on('message', (msg) => {
    try {
      const obj = JSON.parse(msg);
      if (obj.type === 'register' && obj.id){
        devices.set(obj.id, ws);
        ws._deviceId = obj.id;
        ws.send(JSON.stringify({ type: 'registered', id: obj.id }));
        return;
      }
      if (obj.type === 'input' && obj.id){
        const target = devices.get(obj.id);
        if (target && target.readyState === WebSocket.OPEN){
          target.send(JSON.stringify(obj));
        }
      }
    } catch (err) {
      // ignore bad JSON
    }
  });

  ws.on('close', () => {
    if (ws._deviceId) devices.delete(ws._deviceId);
  });
});

server.listen(port, '0.0.0.0', () => {
  console.log(`Server listening on http://localhost:${port}`);
});
