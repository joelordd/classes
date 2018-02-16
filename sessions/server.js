const http = require('http');
const qs = require('querystring');
const sessions = require('./sessions');

const server = http.createServer();
    
server.listen(8000);

server.on('request', (req, res) => {
    sessions.filter(req, res);
    
    if (req.url === '/color') {
        console.log("Processing color choice");
        handleFormColor(req, res);
    } else if (req.url === "/") {
        res.setHeader('Content-type', 'text/html');
        res.end(
            '<p style="color: #' + req.session.color + '">Please pick a color below and click submit.</p>' +
            '<form action="/color" method="post" enctype="x-www-form-urlencode">' +
            '<input type="radio" name="color" value="FF0000" /> Red   <br>'       +
            '<input type="radio" name="color" value="00FF00" /> Green <br>'       +
            '<input type="radio" name="color" value="0000FF" /> Blue  <br>'       +
            '<input type="submit" value="Submit" />'                              +
            '</form>'
        );
    } else {
        res.writeHead(404);
        res.end('Not Found');
    }
});

function handleFormColor(req, res) {
  console.log('\nForm data received:\n');
  console.log(req.headers);
  console.log('\n');
  let body = '';
  req.on('data', (chunk) => {
    body += chunk;
  });
  req.on('end', () => {
    const form = qs.parse(body);
    if (
      form.color !== 'FF0000' &&
      form.color !== '00FF00' &&   
      form.color !== '0000FF'
    ) {
      res.writeHead(400);
      res.end('Bad request');
    }
    req.session.color = form.color;
    res.writeHead(302, { 'Location': '/' });
    res.end();
  });
}

server.listen( () => {
        console.log(`Server running at http://localhost:8000/`);
});

const response = 

   'HTTP/1.1 200 OK'          + '\r\n' +
 
  'Content-length: 11'       + '\r\n' + 

   'Content-type: text/html';
