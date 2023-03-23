const express = require('express');
const app = express();
const multer = require('multer');

app.get('/', (req, res, next) => {
  res.send(`<form method="POST" action="/" enctype="multipart/form-data">
  <input type="text" name="json" placeholder="json">
  <input type="submit">
</form>`);
});

app.post('/', multer().none(), function (req, res, next) {
 try {
  console.log(JSON.parse(req.body.json));
  res.send("OK");
 } catch (error) {
  console.log("Invalid JSON Upload: "+req.body.json);
  res.send("BAD REQUEST");
 }
});


app.listen(3000);