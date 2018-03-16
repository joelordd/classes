const express = require('express')
const qs = require('querystring');
const db = require('./db');
const auth = require('./auth');
const engine = require('./engine');
const sessions = require('./sessions');

const app = express();
const static = express.static('public');
const urlencodedParser = express.urlencoded({ extended: false });

app.use(static);
app.use(auth);
app.engine('html', engine);
app.set('views', './views');

//add global variable
let message = "Remember, with great power comes great responsibility";

function adminCheck(req, res, next) {
	const username = req.session.username;
	
	if(username === 'admin') {
		next();
	} else {
		res.redirect('/');
	}
}

app.get('/', function(req, res) { 
	const username = req.session.username;
	
	db.getUserColor(username, (color) => {
		db.getUserParagraph(username, (paragraph) => {
			const params =  { color: color, paragraph: paragraph };
			res.render('home.html', { params: params });
		});
	});
});

//add admin.html and userList.html
app.get('/admin.html', adminCheck, function(req, res) {
	const username = req.session.username;
	
	const params = { message: message };
	res.render('admin.html', { params: params });
	message = "Remember, with great power comes great responsibility";
});

app.get('/userList.html', function (req, res) {
	db.getAllUsers((usernames) => {
		const params = { usernames: usernames };
		res.render('userList.html', { params: params });
		//res.send(usernames);
	});
});

app.get('/color.html', function(req, res) { 
	const username = req.session.username;
	
	db.getUserColor(username, (color) => {
		const params =  { color: color, red: '', green: '', blue: '' };
        	
		switch (color) {
            		case 'FF0000':
				params.red = 'checked';
                		break;
            		case '00FF00':
				params.green = 'checked';
				break;
            		case '0000FF':
				params.blue = 'checked';
                		break;
		}

		res.render('color.html', { params: params });
	});
});

app.post('/color', urlencodedParser, (req, res) => {
	const username = req.session.username;
	const color = req.body.color;
	
	db.setUserColor(username, color, () => {
		res.redirect('/');
	}); 
});

app.post('/peragraph', urlencodedParser, (req, res) => {
	const username = req.session.username;
	const paragraph = req.body.paragraph;
	
	db.setUserParagraph(username, paragraph, () => {
		res.redirect('/');
	}); 
});

//add admin functions
app.post('/createUser', urlencodedParser, adminCheck, (req, res) => {
	const username = req.body.usernameC;
	const password = req.body.passwordC;
	message = "New User " + username + " Created";
	
	db.checkUser(username, (result) => {
		if(result === true) {
			message = "Error, user " + username + " already exists";
			res.redirect('admin.html');
		} else {
			db.createUser(username, password, () => {
				res.redirect('admin.html');
			});
		}
	});
});

app.post('/deleteUser', urlencodedParser, adminCheck, (req, res) => {
	const username = req.body.usernameD;
	message = "User " + username + " Deleted";

	db.checkUser(username, (result) => {
		if(result === false) {
			message = "Error, user " + username + " doesn't exist";
			res.redirect('admin.html');
		} else {
			db.deleteUser(username, () => {
				res.redirect('admin.html');
			});
		}
	});
});

app.post('/changePassword', urlencodedParser, adminCheck, (req, res) => {
	const username = req.body.usernameN;
	const password = req.body.passwordN;
	message = "Changed Password For " + username;

	db.checkUser(username, (result) => {
		if(result === false) {
			message = "Error, user " + username + " doesn't exist";
			res.redirect('admin.html');
		} else {
			db.changePassword(username, password, () => {
				res.redirect('admin.html');
			});
		}
	});
});


const port = 8000;
app.listen(port, () => {
        console.log(`Server running at http:localhost:${port}`);
});

