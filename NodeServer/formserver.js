const express = require('express');
const app = express();
const multer = require('multer');

class FormServer {
    // Constructor runs when a WebServer class is initialised to setup variables and start
    constructor(host, port, db) {
        this.host = host;
        this.port = port;
        this.db = db;
        app.get('/', (req, res, next) => {
            res.send(`<form method="POST" action="/" enctype="multipart/form-data">
            <input type="text" name="json" placeholder="json">
            <input type="submit">
          </form>`);
        });

        app.post('/', multer().none(), this.acceptForm.bind(this));

    }

    // Method to start server
    start() {
        app.listen(this.port, this.host, this.serverStarted(this));
    }

    // Method to log when server starts
    serverStarted() {
        console.log(`Form Server listening on http://${this.host}:${this.port}`);
    }

    // Method to accept form
    acceptForm(req, res, next) {
        try {
            let formdata = JSON.parse(req.body.json);
            if (formdata.hasOwnProperty("id") && formdata.hasOwnProperty("ts") && formdata.hasOwnProperty("data")) {
                console.log("[formServer] New datapoint from sensor: "+formdata.id);
                // write to db
                this.db.addEntry(formdata);
                res.send("OK");
            } else {
                throw Error("Invalid JSON");
            }
        } catch (error) {
            // console.log(error)
            console.log("Invalid JSON Upload: " + req.body.json);
            res.send("BAD REQUEST");
        }
    }


}
module.exports = {
    FormServer: FormServer
}; 