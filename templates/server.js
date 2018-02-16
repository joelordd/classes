const http = require('http');
const st = require('st');
const fs = require('fs');
const qs = require('querystring');
const sessions = require('./sessions');

const server = http.createServer();
    
server.listen(8000);

var mount = st ({
    path: 'public/'
});

const homeTemplate = fs.readFileSync('views/home.html', 'utf8');
const colorTemplate = fs.readFileSync('views/color.html', 'utf8');

server.on('request', (req, res) => {
    sessions.filter(req, res);
    
    if (req.url === '/login.html') {
        mount(req, res);
    } else if (req.url === '/login') {
        handleLogin(req, res);
    } else if(!req.session.hasOwnProperty('username')) {
        redirectLoginPage(res);
    } else if(req.url === '/logout') {
        handleLogout(req, res);
    } else if (req.url === '/color') {
        handleFormColor(req, res);
    } else if (req.url === "/") {
        handleHome(req, res);
    } else {
        res.writeHead(404);
        res.end('Not Found');
    }
});

function handleHome(req, res) {
    res.setHeader('Content-type', 'text/html');
        res.end(
            '<p style="color: #' + req.session.color + '">Please pick a color below and click submit.</p>' +
            '<form action="/color" method="POST" enctype="x-www-form-urlencode">' +
            '<input type="radio" name="color" value="FF0000" /> Red   <br>'       +
            '<input type="radio" name="color" value="00FF00" /> Green <br>'       +
            '<input type="radio" name="color" value="0000FF" /> Blue  <br>'       +
            '<input type="submit" value="Submit" />'                              +
            '</form>'                                                             +
            '<form method="POST" action="/logout">'                               +
            '<input type="submit" value="Logout" />'                              +
            '</form>'
        );
}

function redirectHomePage(res) {
    res.writeHead(302, { 'Location': '/' });
    res.end();
}

function redirectLoginPage(res) {
    res.writeHead(301, { 'Location': '/login.html'} );
    res.end();
}

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

function handleLogin(req, res) {
    console.log('\nForm data received:\n');
    console.log(req.headers);
    console.log('\n');
    let body = '';
    req.on('data', (chunk) => {
        body += chunk;
    });
    req.on('end', () => {
        const form = qs.parse(body);
        if (form.username !== 'fred' || form.password !== '1234') {
            redirectLoginPage(res);
        } else {
            req.session.username = form.username;
            redirectHomePage(res);
        }
    });
}

function handleLogout(req, res) {
    delete req.session.username;
    redirectLoginPage(res);
}

server.listen( () => {
        console.log(`Server running at http://localhost:8000/`);
});

const response = 

   'HTTP/1.1 200 OK'          + '\r\n' +
 
  'Content-length: 11'       + '\r\n' + 

   'Content-type: text/html';
