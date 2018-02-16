const http = require('http');
const st = require('st');

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

const server = http.createServer((req, res) => {

 if (req.url === '/hi') {
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