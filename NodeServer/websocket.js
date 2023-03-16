const WebSocket = require("ws");
const express = require('express');
const path = require("path");

// Class for imnplementing WebSocket Server
class WebSocketServer {
    // Constructor to start server
    constructor(host, port, onmessage) {
        // Initialise variables
        this.host = host;
        this.port = port;
        this.onMessageP = onmessage;
        // Initialise server variables
        this.app = express();
        this.app.use("/",express.static(path.resolve(__dirname, "../client")));
    }

    // Method to start server
    start() {
        // Start WebServer->WebSocket upgrade server
        let wsServer = new WebSocket.Server({
            noServer: true
        });
        this.server = this.app.listen(this.port, this.host, this.serverStarted(this));
        this.server.on('upgrade', async function upgrade(request, socket, head) {
            // Handle HTTP->WebSocket upgrade
            wsServer.handleUpgrade(request, socket, head, function done(ws) {
                wsServer.emit('connection', ws, request);
            });
        });
        // Start WebSocket Server
        wsServer.on("connection", (ws) => {
            this.onConnect(ws);
        });
        this.wsServer = wsServer;
    }

    // Method to log when server starts
    serverStarted() {
        console.log(`Web Socket Server listening on http://${this.host}:${this.port}`);
    }

    // Methods for handling websocket events
    onConnect(ws) {
        // When a new client connects
        console.log("["+ws._socket.remoteAddress.toString()+":"+ws._socket.remotePort.toString()+"]", "NEW Client Connection");
        // Listen for new incoming messages
        ws.on("message", (msg) => {
            console.log("["+ws._socket.remoteAddress.toString()+":"+ws._socket.remotePort.toString()+"]", "RECV Message:", msg.toString());
            // console.log(ws);
            this.onMessage(ws, msg);
        });
    }
    onMessage(ws, msg) {
        this.onMessageP(this, ws, msg);
    }

    // Methods for sending messages
    sendUnicastMsg(msg) {
        // for every client
        this.wsServer.clients.forEach((client) => {
            // if client is ready to listen
            if (client.readyState == WebSocket.OPEN) {
                // send message
                client.send(msg);
            }
        });
        console.log("[unicast:"+this.wsServer.clients.size.toString()+"] SEND Message:", msg);
    }
    sendSingleMsg(ws, msg) {
        ws.send(msg);
    }
}

module.exports = {
    WebSocketServer: WebSocketServer
};