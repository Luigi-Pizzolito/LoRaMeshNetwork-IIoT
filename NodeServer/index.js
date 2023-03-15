// Import local modules
const sws = require('./webserver.js');
let StaticWebServer = sws.StaticWebServer;
const wss = require('./websocket.js');
let WebSocketServer = wss.WebSocketServer;
//todo: add a logging module for better log output and logging levels.

// Start website server
let siteServer = new StaticWebServer("localhost", 8000, "public");
siteServer.start();

// Start websocket server
// sample onMessage() function
function testEchoOnMessage(wsServer, msg) {
    // when we recieve a message to the server
    // send the same message back to all clients
    wsServer.sendUnicastMsg(msg.toString());
}
// Start Websocket server
let websockServer = new WebSocketServer("localhost", 9876, testEchoOnMessage);
websockServer.start();