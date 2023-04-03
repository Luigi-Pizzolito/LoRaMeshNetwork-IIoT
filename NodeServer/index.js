// Import local modules
const sws = require('./webserver.js');
let StaticWebServer = sws.StaticWebServer;
const wss = require('./websocket.js');
let WebSocketServer = wss.WebSocketServer;
const fms = require('./formserver.js');
let FormServer = fms.FormServer;
//todo: add a logging module for better log output and logging levels.


// Start website server
let siteServer = new StaticWebServer("localhost", 8000, "public");
siteServer.start();

// Start websocket server
function replyToClient(wsServer, client, msg) {
    // Example barebones request
    //'{"time_range":[1679201350,1679403456],"id":0}'
    // query DB
    wsServer.db.requestQuery(msg.toString(), function(err, response) {
        if (err == null) {
            // send results back to client
            wsServer.sendSingleMsg(client, JSON.stringify(response));
        } else {
            // send error back to client
            wsServer.sendSingleMsg(client, '{"error": "'+err.toString()+'"}');
        }
    });
}

// Start Websocket server
let websockServer = new WebSocketServer("localhost", 9876, replyToClient);
websockServer.start();

//Start Form server
let formserver = new FormServer("localhost", 8003, websockServer.db);
formserver.start();