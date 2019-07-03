const http = require('http');
const st = require('st');
const qs = require('querystring');

const port = 8000;

var mount = st({
  path: 'public/',
  index: 'index.html'
});

const hi =
	"<html>" +
	"<body>" +
	"<div>You requested /hi.</div>" +
	"</body>" +
	"</html>";

function handleFormTest(req, res) {
  console.log('\nForm data received:\n');
  console.log(req.headers);
  console.log('\n');
  let body = '';
  req.on('data', (chunk) => {
    body += chunk;
  });
  req.on('end', () => {
    const form = qs.parse(body);
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/html');
    res.end('<p>Hello, ' + form.firstname + ' ' + form.lastname + '</p>\n');
  });
}

function handleFormFlavor(req, res) {
  console.log('\nForm data received:\n');
  console.log(req.headers);
  console.log('\n');
  let body = '';
  req.on('data', (chunk) => {
    body += chunk;
  });
  req.on('end', () => {
    const form = qs.parse(body);
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/html');
    
    let flavor = '<p>Flavors Checked: ';
    if(form.Vanilla) {
        flavor += 'Vanilla ';
    }
    if(form.Chocolate) {
        flavor += 'Chocolate ';
    }
    if(form.Strawberry) {
        flavor += 'Strawberry ';
    }
    res.end(flavor + '</p>\n');
  });
}

function handleFormRadio(req, res) {
  console.log('\nForm data received:\n');
  console.log(req.headers);
  console.log('\n');
  let body = '';
  req.on('data', (chunk) => {
    body += chunk;
  });
  req.on('end', () => {
    const form = qs.parse(body);
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/html');
    res.end(form.music + '</p>\n');
  });
}

const server = http.createServer((req, res) => {

    if(req.url === '/test') {
        handleFormTest(req, res);
    } else if(req.url === '/flavor') {
        handleFormFlavor(req, res);
    } else if(req.url === '/radio') {
        handleFormRadio(req, res);
    } else if (req.url === '/hi') {
    	res.end(hi);
    } else {
    	mount(req, res);
    }

});




server.listen(port, () => {
        console.log(`Server running at http://localhost:${port}/`);
});

const response = 

   'HTTP/1.1 200 OK'          + '\r\n' +
 
  'Content-length: 11'       + '\r\n' + 

   'Content-type: text/html';
