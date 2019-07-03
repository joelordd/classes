const express     = require('express');
const sessions    = require('./sessions.js');
const db          = require('./db.js');

const urlencodedParser = express.urlencoded({ extended: false });

const router = express.Router();

module.exports = router;

router.use(function(req, res, next) {
	sessions.filter(req, res);
	next();
});

router.post('/login', urlencodedParser, function(req, res) {
	let username = req.body['username'];
	let password = req.body['password'];
	
	db.getUser(username, password, (user) => {
	
		if (user === null || username !== 'fred' || password !== '1234') {
			res.redirect('/login.html');
		} else {
			req.session.username = username;
			res.redirect('/');
		}
	});
});

router.post('/logout', function(req, res) {
	delete req.session.username;
	res.redirect('/login.html');
});

router.use(function(req, res, next) {
	if (req.session.hasOwnProperty('username')) {
		next();
	} else {
		res.redirect('/login.html');
	}
});
