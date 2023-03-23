const express = require('express');
const app = express();

/** Require multer */
const multer = require('multer');

/** Show page with a form with a specific enctype */
app.get('/', (req, res, next) => {
  res.send(`<form method="POST" action="/" enctype="multipart/form-data">
  <input type="text" name="username" placeholder="username">
  <input type="text" name="password" placeholder="password">
  <input type="submit">
</form>`);
});

/** Process POST request with a mutter's middleware */
app.post('/', multer().none(), function (req, res, next) {
  console.log(req.body);
});

/** Run the app */
app.listen(3000);

/*const express = require('express');
const app = express();

app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res, next) => {
  response.send('./submit-form.html');
});

app.post('/', (req, res, next) => {
  //const username = req.body.username;
  response.send(req.body.username);
});*/


/*var express = require('express');
var bodyParser = require('body-parser');
var multer = require('multer');
var upload = multer();
var app = express();

app.get('/', function(req, res){
   res.render('form');
});

app.set('view engine', 'pug');
app.set('views', './views');

// for parsing application/json
app.use(bodyParser.json()); 

// for parsing application/xwww-
app.use(bodyParser.urlencoded({ extended: true })); 
//form-urlencoded

// for parsing multipart/form-data
app.use(upload.array()); 
app.use(express.static('public'));

app.post('/', function(req, res){
   console.log(req.body);
});
app.listen(3000);*/