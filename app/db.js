const pg = require('pg');

process.env.PGDATABASE = 'app';
process.env.PGPASSWORD = 'password';

const pool = new pg.Pool();

pool.on('error', (err, client) => {
	console.log(err.stack);
	process.exit(-1);
});

function getUser(username, password, cb) {
	pool.connect((err, client, done) => {
		if (err) throw err;
		
		const q = "select color from users where username=$1::text and password=$2::text";
		const params = [username, password];
		
		client.query(q, params, (err, result) => {
			if (err) {
				console.log(result);
				console.log(err.stack); 
				cb(null); 
			} else if (result.rows.length === 0) {
				cb(null); 
			} else {
				let row = result.rows[0];
				cb({ color: row['color'], paragraph: row['paragraph'] });
			}
			done();
		});
	});
}

function getUserColor(username, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "select color from users where username=$1::text";
                const params = [username];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(null); 
			} else if (result.rows.length === 0) {
				cb(null); 
			} else {
                                let row = result.rows[0];
                                cb(row['color']);
                        }
                        done();
                });
        });
}

function setUserColor(username, color, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "update users set color=$1::text where username=$2::text";
                const params = [color, username];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(); 
			}
			done();
			cb();
                });
        });
}

function getUserParagraph(username, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "select paragraph from users where username=$1::text";
                const params = [username];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(null); 
			} else if (result.rows.length === 0) {
				cb(null); 
			} else {
                                let row = result.rows[0];
                                cb(row['paragraph']);
                        }
                        done();
                });
        });
}

function setUserParagraph(username, paragraph, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "update users set paragraph=$1::text where username=$2::text";
                const params = [paragraph, username];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(); 
			}
			done();
			cb();
                });
        });
}

//add admin functions
function createUser(username, password, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "insert into users values ($1::text, $2::text, '0000FF', 'This is your textbox')";
                const params = [username, password];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(); 
			}
			done();
			cb();
                });
        });
}

function deleteUser(username, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "delete from users where username=$1::text";
                const params = [username];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(); 
			}
			done();
			cb();
                });
        });
}

function changePassword(username, password, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "update users set password=$1::text where username=$2::text";
                const params = [password, username];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(); 
			}
			done();
			cb();
                });
        });
}

function getAllUsers(cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "select username from users";
                client.query(q, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(null); 
			} else if (result.rows.length === 0) {
				cb(null); 
			} else {
				const rows = result['rows'];
                                let usernames = [];
				rows.forEach(function(rows) {
					usernames.push(rows['username']);
				});
                                cb(usernames);
                        }
                        done();
                });
        });
}

function checkUser(username, cb) {
	pool.connect((err, client, done) => {
                if (err) throw err;
                const q = "select exists (select username from users where username=$1::text)";
                const params = [username];
                client.query(q, params, (err, result) => {
                        if (err) {
				console.log(err.stack); 
				cb(null); 
			} else {
				const check = result['rows'][0]['exists'];
                                cb(check);
                        }
                        done();
                });
        });

}

exports.getUser = getUser;
exports.getUserColor = getUserColor;
exports.setUserColor = setUserColor;
exports.getUserParagraph = getUserParagraph;
exports.setUserParagraph = setUserParagraph;

//export all these functions
exports.createUser = createUser;
exports.deleteUser = deleteUser;
exports.changePassword = changePassword;
exports.getAllUsers = getAllUsers;
exports.checkUser = checkUser;
