const http = require('http');
const fs = require('fs');
const qs = require('querystring');
const sessions = require('./sessions');

const server = http.createServer();
    
server.listen(8000);

var message = '';

const homeTemplate = fs.readFileSync('views/home.html', 'utf8');
const colorTemplate = fs.readFileSync('views/color.html', 'utf8');
const loginTemplate = fs.readFileSync('views/login.html', 'utf8');

server.on('request', (req, res) => {
    sessions.filter(req, res);
    
    if (req.url === '/login.html') {
        handleLoginPage(req, res);
    } else if (req.url === '/login') {
        handleLogin(req, res);
    } else if(!req.session.hasOwnProperty('username')) {
        redirectLoginPage(res);
    } else if(req.url === '/logout') {
        handleLogout(req, res);
    } else if (req.url === '/color') {
        handleFormColor(req, res);
    } else if (req.url === '/color.html') {
        handleColorPage(req, res);
    } else if (req.url === "/") {
        handleHomePage(req, res);
    } else {
        res.writeHead(404);
        res.end('Not Found');
    }
});

function handleHomePage(req, res) {
    res.setHeader('Content-type', 'text/html');
    const html = homeTemplate.replace('#color#', req.session.color);
    res.end(html);
}

function handleColorPage(req, res) {
    res.setHeader('Content-type', 'text/html');
    var html;
    switch (req.session.color) {
        case 'FF0000':
            html = colorTemplate.replace('#red#'   , 'checked');
            html = html         .replace('#green#' , '       ');
            html = html         .replace('#blue#'  , '       ');
            break;
        case '00FF00':
            html = colorTemplate.replace('#red#'   , '       ');
            html = html         .replace('#green#' , 'checked');
            html = html         .replace('#blue#'  , '       ');
            break;
        case '0000FF':
            html = colorTemplate.replace('#red#'   , '       ');
            html = html         .replace('#green#' , '       ');
            html = html         .replace('#blue#'  , 'checked');
            break;
        default:
            html = colorTemplate.replace('#red#'   , '       ');
            html = html         .replace('#green#' , '       ');
            html = html         .replace('#blue#'  , '       ');
            break;
    }
    res.end(html);
}

function handleLoginPage(req, res) {
    res.setHeader('Content-type', 'text/html');
    const html = loginTemplate.replace('#message#', message);
    res.end(html);
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
        if (form.color !== 'FF0000' && form.color !== '00FF00' && form.color !== '0000FF') {
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
        if (form.username !== 'fred') {
            message = 'Bad Username'
            redirectLoginPage(res);
        } else if (form.password !== '1234'){
            message = 'Bad Password';
            redirectLoginPage(res);
        } else {
            req.session.username = form.username;
            redirectHomePage(res);
        }
    });
}

function handleLogout(req, res) {
    delete req.session.username;
    message = '';
    redirectLoginPage(res);
}

server.listen( () => {
        console.log(`Server running at http://localhost:8000/`);
});

const response = 

   'HTTP/1.1 200 OK'          + '\r\n' +
 
  'Content-length: 11'       + '\r\n' + 

   'Content-type: text/html';
